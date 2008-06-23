
/** @defgroup world Worlds

  Stage simulates a 'world' composed of `models', defined in a `world
  file'. 

API: Stg::StgWorld

<h2>Worldfile properties</h2>

@par Summary and default values

@verbatim

title "[defaults to worldfile filename]"
interval_real   100
interval_sim    100
gui_interval    100
resolution      0.02

@endverbatim

@par Details
- name [string]
- the name of the world, as displayed in the window title bar. Defaults to the worldfile file name.
- interval_sim [milliseconds]
- the length of each simulation update cycle in milliseconds.
- interval_real [milliseconds]
- the amount of real-world (wall-clock) time the siulator will attempt to spend on each simulation cycle.
- gui_interval [milliseconds]
- the amount of real-world time between GUI updates
- resolution [meters]
- specifies the resolution of the underlying bitmap model. Larger values speed up raytracing at the expense of fidelity in collision detection and sensing. 
@par More examples

The Stage source distribution contains several example world files in
<tt>(stage src)/worlds</tt> along with the worldfile properties
described on the manual page for each model type.

 */

#define _GNU_SOURCE

//#define DEBUG 

#include <stdlib.h>
#include <assert.h>
#include <string.h> // for strdup(3)
#include <locale.h> 
#include <glib-object.h> // fior g_type_init() used by GDKPixbuf objects
#include <limits.h>

#include "stage_internal.hh"
#include "region.hh"


// static data members
unsigned int StgWorld::next_id = 0;
bool StgWorld::quit_all = false;
GList* StgWorld::world_list = NULL;

static guint PointIntHash( stg_point_int_t* pt )
{
	return( pt->x + (pt->y<<16 ));
}

static gboolean PointIntEqual( stg_point_int_t* p1, stg_point_int_t* p2 )
{
	return( memcmp( p1, p2, sizeof(stg_point_int_t) ) == 0 );
}


SuperRegion* StgWorld::CreateSuperRegion( int32_t x, int32_t y )
{
	SuperRegion* sr = new SuperRegion( x, y );
	g_hash_table_insert( superregions, &sr->origin, sr );

	dirty = true; // force redraw
	return sr;
}

void StgWorld::DestroySuperRegion( SuperRegion* sr )
{
	g_hash_table_remove( superregions, &sr->origin );
	delete sr;
}

void StgWorld::UpdateAll()
{  
  for( GList* it = StgWorld::world_list; it; it=it->next )
	 ((StgWorld*)it->data)->Update();
}

StgWorld::StgWorld( const char* token, 
						  stg_msec_t interval_sim,
						  double ppm )
{
  Initialize( token, interval_sim, ppm );
}

void StgWorld::Initialize( const char* token, 
		stg_msec_t interval_sim, 
		double ppm ) 
{
	if( ! Stg::InitDone() )
	{
		PRINT_WARN( "Stg::Init() must be called before a StgWorld is created." );
		exit(-1);
	}
	StgWorld::world_list = g_list_append( StgWorld::world_list, this );
	
	this->ray_list = NULL;
	this->quit_time = 0;

	assert(token);
	this->token = (char*)g_malloc(Stg::TOKEN_MAX);
	snprintf( this->token, Stg::TOKEN_MAX, "%s", token );//this->id );

	this->quit = false;
	this->updates = 0;
	this->wf = NULL;
	this->graphics = false; // subclasses that provide GUIs should
	// change this

	//this->models_by_id = g_hash_table_new( g_direct_hash, g_direct_equal );
	this->models_by_name = g_hash_table_new( g_str_hash, g_str_equal );
	this->sim_time = 0;
	this->interval_sim = (stg_usec_t)thousand * interval_sim;
	this->ppm = ppm; // this is the raytrace resolution

	this->real_time_start = RealTimeNow();
	//this->real_time_next_update = 0;

	this->update_list = NULL;
	this->velocity_list = NULL;

	this->superregions = g_hash_table_new( (GHashFunc)PointIntHash, 
			(GEqualFunc)PointIntEqual );

	this->total_subs = 0;
	this->destroy = false;

	// store a global table of all blocks, so they can be rendered all
	// at once.
	//this->blocks = g_hash_table_new( NULL, NULL );

	bzero( &this->extent, sizeof(this->extent));

	this->real_time_now = 0;
}

StgWorld::~StgWorld( void )
{
	PRINT_DEBUG2( "destroying world %d %s", id, token );

	if( wf ) delete wf;
	//if( bgrid ) delete bgrid;

	//g_hash_table_destroy( models_by_id );
	g_hash_table_destroy( models_by_name );

	g_free( token );

	world_list = g_list_remove( world_list, this );
}


void StgWorld::RemoveModel( StgModel* mod )
{
  //g_hash_table_remove( models_by_id, mod );
	g_hash_table_remove( models_by_name, mod );
}


// wrapper to startup all models from the hash table
void init_models( gpointer dummy1, StgModel* mod, gpointer dummy2 )
{
	mod->Init();
}

// delete a model from the hash table
void destroy_model( gpointer dummy1, StgModel* mod, gpointer dummy2 )
{
	free(mod);
}

// delete a model from the hash table
void destroy_sregion( gpointer dummy1, SuperRegion* sr, gpointer dummy2 )
{
	free(sr);
}


void StgWorld::Load( const char* worldfile_path )
{
  GHashTable* entitytable = g_hash_table_new( g_direct_hash, g_direct_equal );

	printf( " [Loading %s]", worldfile_path );
	fflush(stdout);

	stg_usec_t load_start_time = RealTimeNow();

	this->wf = new Worldfile();
	wf->Load( worldfile_path );
	PRINT_DEBUG1( "wf has %d entitys", wf->GetEntityCount() );

	// end the output line of worldfile components
	//puts("");

	int entity = 0;

	this->token = (char*)
		wf->ReadString( entity, "name", token );

	this->interval_sim = (stg_usec_t)thousand * 
	  wf->ReadInt( entity, "interval_sim", 
		       (int)(this->interval_sim/thousand) );

	if( wf->PropertyExists( entity, "quit_time" ) )
		this->quit_time = (stg_usec_t)million * 
			wf->ReadInt( entity, "quit_time", 0 );

	if( wf->PropertyExists( entity, "resolution" ) )
		this->ppm = 
		  1.0 / wf->ReadFloat( entity, "resolution", 1.0 / this->ppm );

	//_stg_disable_gui = wf->ReadInt( entity, "gui_disable", _stg_disable_gui );

	// Iterate through entitys and create client-side models
	for( entity = 1; entity < wf->GetEntityCount(); entity++ )
	{
		char *typestr = (char*)wf->GetEntityType(entity);      	  

		// don't load window entries here
		if( strcmp( typestr, "window" ) == 0 )
			continue;

		int parent_entity = wf->GetEntityParent( entity );

		PRINT_DEBUG2( "wf entity %d parent entity %d\n", 
				entity, parent_entity );

		StgModel *mod, *parent;
		
		parent = (StgModel*)g_hash_table_lookup( entitytable, 
															  (gpointer)parent_entity );
		
		// find the creator function pointer in the hash table. use the
		// vanilla model if the type is NULL.
		stg_creator_t creator;
		
		//printf( "creating model of type %s\n", typestr );

		if( typestr ) // look up the string in the typetable
		  for( int i=0; i<MODEL_TYPE_COUNT; i++ )
			 if( strcmp( typestr, typetable[i].token ) == 0 )
				{
				  creator = typetable[i].creator;
				  break;
				}
		
		  //creator = (stg_creator_t)g_hash_table_lookup( Stg::Typetable(), typestr );      
		  //else 
		  //creator = NULL;
	
	// if we found a creator function, call it
		if( creator )
		{
			//printf( "creator fn: %p\n", creator );
			mod = (*creator)( this, parent );
		}
		else
		{
			PRINT_ERR1( "Unknown model type %s in world file.", 
					typestr );
			exit( 1 );
		}

		//printf( "created model %s\n", mod->Token() );

		// configure the model with properties from the world file
		mod->SetWorldfile( wf, entity );
		mod->Load();
		
		// record the model we created for this worlfile entry
		g_hash_table_insert( entitytable, (gpointer)entity, mod );
	}

	// warn about unused WF linesa
	wf->WarnUnused();

	// run through the loaded models and initialise them
	g_hash_table_foreach( entitytable, (GHFunc)init_models, NULL );
	
	// now we're done with the worldfile entry lookup
	g_hash_table_destroy( entitytable );

	stg_usec_t load_end_time = RealTimeNow();

	printf( "[Load time %.3fsec]", 
			  (load_end_time - load_start_time) / 1000000.0 );
}

void StgWorld::UnLoad()
{
	if( wf ) delete wf;
	//if( bgrid ) delete bgrid;

	g_list_foreach( children, (GFunc)destroy_model, NULL );
	g_list_free( children );
	children = NULL;
	
	//g_hash_table_remove_all( child_types ); // small memory leak	
	
	//g_hash_table_remove_all( models_by_id );
		
	g_hash_table_remove_all( models_by_name );
	
	g_list_free( velocity_list );
	velocity_list = NULL;
	
	g_list_free( update_list );
	update_list = NULL;
	
	g_list_free( ray_list );
	ray_list = NULL;
	
	g_hash_table_foreach( superregions, (GHFunc)destroy_sregion, NULL );
	g_hash_table_remove_all( superregions );

	g_free( token );
	this->token = (char*)g_malloc(Stg::TOKEN_MAX);  // necessary?
	
}

stg_usec_t StgWorld::RealTimeNow()
{
	struct timeval tv;
	gettimeofday( &tv, NULL );  // slow system call: use sparingly

	return( tv.tv_sec*1000000 + tv.tv_usec );
}

stg_usec_t StgWorld::RealTimeSinceStart()
{
	stg_usec_t timenow = RealTimeNow();

	// subtract the start time from the current time to get the elapsed
	// time

	return timenow - real_time_start;
}

#define DEBUG 1

bool StgWorld::Update()
{
  PRINT_DEBUG( "StgWorld::Update()" );
  
  // update any models that are due to be updated
  for( GList* it=this->update_list; it; it=it->next )
	 ((StgModel*)it->data)->UpdateIfDue();
  
  // update any models with non-zero velocity
  for( GList* it=this->velocity_list; it; it=it->next )
	 ((StgModel*)it->data)->UpdatePose();

  this->sim_time += this->interval_sim;
  this->updates++;
  
  // if we've run long enough, set the quit flag
  if( (quit_time > 0) && (sim_time >= quit_time) )
	 quit = true;
  
  return true;
}

void StgWorld::AddModel( StgModel*  mod  )
{
	g_hash_table_insert( this->models_by_name, (gpointer)mod->Token(), mod );
}

StgModel* StgWorld::GetModel( const char* name )
{
	PRINT_DEBUG1( "looking up model name %s in models_by_name", name );
	StgModel* mod = (StgModel*)g_hash_table_lookup( this->models_by_name, name );

	if( mod == NULL )
		PRINT_WARN1( "lookup of model name %s: no matching name", name );

	return mod;
}

void StgWorld::RecordRay( double x1, double y1, double x2, double y2 )
{  
	float* drawpts = new float[4];
	drawpts[0] = x1;
	drawpts[1] = y1;
	drawpts[2] = x2;
	drawpts[3] = y2;
	ray_list = g_list_append( ray_list, drawpts );
}

void StgWorld::ClearRays()
{
	for( GList* it = ray_list; it; it=it->next )
	{
		float* pts = (float*)it->data;
		delete [] pts;
	}

	g_list_free(ray_list );
	ray_list = NULL;
}


void StgWorld::Raytrace( stg_pose_t pose, // global pose
		stg_meters_t range,
		stg_radians_t fov,
		stg_block_match_func_t func,
		StgModel* model,			 
		const void* arg,
		stg_raytrace_sample_t* samples, // preallocated storage for samples
		uint32_t sample_count,
		bool ztest )  // number of samples
{
	pose.a -= fov/2.0; // direction of first ray
	stg_radians_t angle_incr = fov/(double)sample_count;

	for( uint32_t s=0; s < sample_count; s++ )
	{
		Raytrace( pose, range, func, model, arg, &samples[s], ztest );
		pose.a += angle_incr;
	}
}



void StgWorld::Raytrace( stg_pose_t pose, // global pose
		stg_meters_t range,
		stg_block_match_func_t func,
		StgModel* mod,		
		const void* arg,
		stg_raytrace_sample_t* sample,
		bool ztest ) 
{
	// initialize the sample
	memcpy( &sample->pose, &pose, sizeof(stg_pose_t)); // pose stays fixed
	sample->range = range; // we might change this below
	sample->block = NULL; // we might change this below

	// find the global integer bitmap address of the ray  
	int32_t x = (int32_t)(pose.x*ppm);
	int32_t y = (int32_t)(pose.y*ppm);
	int32_t z = 0;

	int32_t xstart = x;
	int32_t ystart = y;

	// and the x and y offsets of the ray
	int32_t dx = (int32_t)(ppm*range * cos(pose.a));
	int32_t dy = (int32_t)(ppm*range * sin(pose.a));
	int32_t dz = 0;

	//   if( finder->debug )
	//     RecordRay( pose.x, 
	// 	       pose.y, 
	// 	       pose.x + range.max * cos(pose.a),
	// 	       pose.y + range.max * sin(pose.a) );

	// fast integer line 3d algorithm adapted from Cohen's code from
	// Graphics Gems IV
	int n, sx, sy, sz, exy, exz, ezy, ax, ay, az, bx, by, bz;
	sx = sgn(dx);  sy = sgn(dy);  sz = sgn(dz);
	ax = abs(dx);  ay = abs(dy);  az = abs(dz);
	bx = 2*ax;	 by = 2*ay;	bz = 2*az;
	exy = ay-ax;   exz = az-ax;	ezy = ay-az;
	n = ax+ay+az;

	//  printf( "Raytracing from (%d,%d,%d) steps (%d,%d,%d) %d\n",
	//  x,y,z,  dx,dy,dz, n );

	// superregion coords
	stg_point_int_t lastsup;
	lastsup.x = INT_MAX; // an unlikely first raytrace
	lastsup.y = INT_MAX;

	stg_point_int_t lastreg = {0,0};
	lastsup.x = INT_MAX; // an unlikely first raytrace
	lastsup.y = INT_MAX;

	SuperRegion* sr = NULL;
	Region* r = NULL;

	//puts( "RAYTRACE" );

	while ( n-- ) 
	{         
		// superregion coords
		stg_point_int_t sup;
		sup.x = x >> SRBITS;
		sup.y = y >> SRBITS;

		//  printf( "pixel [%d %d]\tS[ %d %d ]\t",
		//      x, y, sup.x, sup.y );

		if( ! (sup.x == lastsup.x && sup.y == lastsup.y )) 
		{
			sr = (SuperRegion*)g_hash_table_lookup( superregions, (void*)&sup );
			lastsup = sup; // remember these coords
		}

		if( sr )
		{	  
			// find the region coords inside this superregion
			stg_point_int_t reg;
			reg.x = (x - ( sup.x << SRBITS)) >> RBITS;
			reg.y = (y - ( sup.y << SRBITS)) >> RBITS;

			//  printf( "R[ %d %d ]\t", reg.x, reg.y );

			if( ! (reg.x == lastreg.x && reg.y == lastreg.y ))
			{
				r = sr->GetRegion( reg.x, reg.y );
				lastreg = reg;
			}

			if( r && r->count )
			{
				// compute the pixel offset inside this region
				stg_point_int_t cell;
				cell.x = x - ((sup.x << SRBITS) + (reg.x << RBITS));
				cell.y = y - ((sup.y << SRBITS) + (reg.y << RBITS));

				//  printf( "C[ %d %d ]\t", cell.x, cell.y );

				for( GSList* list = r->GetCell( cell.x, cell.y )->list;
						list;
						list = list->next )      
				{	      	      
					StgBlock* block = (StgBlock*)list->data;
					assert( block );

					// if this block does not belong to the searching model and it
					// matches the predicate and it's in the right z range
					if( //block && (block->Model() != finder) && 
							(ztest ? block->IntersectGlobalZ( pose.z ) : true) &&
							(*func)( block, mod, arg ) )
					{
						// a hit!
						sample->block = block;
						sample->range = hypot( (x-xstart)/ppm, (y-ystart)/ppm );
						return;
					}
				}
			}
		}      

		//      printf( "\t step %d n %d   pixel [ %d, %d ] block [ %d %d ] index [ %d %d ] \n", 
		//      //coarse [ %d %d ]\n",
		//      count++, n, x, y, blockx, blocky, b_dx, b_dy );

		// increment our pixel in the correct direction
		if ( exy < 0 ) {
			if ( exz < 0 ) {
				x += sx; exy += by; exz += bz;
			}
			else  {
				z += sz; exz -= bx; ezy += by;
			}
		}
		else {
			if ( ezy < 0 ) {
				z += sz;
				exz -= bx; ezy += by;
			}
			else  {
				y += sy; exy -= bx; ezy -= bz;
			}
		}
		//     puts("");
	}

	// hit nothing
	return;
}

static void _save_cb( gpointer key, gpointer data, gpointer user )
{
	((StgModel*)data)->Save();
}

bool StgWorld::Save( const char *filename )
{
	// ask every model to save itself
	//g_hash_table_foreach( this->models_by_id, stg_model_save_cb, NULL );

	ForEachModel( _save_cb, NULL );

	return this->wf->Save( filename );
}

static void _reload_cb( gpointer key, gpointer data, gpointer user )
{
	((StgModel*)data)->Load();
}

// reload the current worldfile
void StgWorld::Reload( void )
{
	ForEachModel( _reload_cb, NULL );
}

void StgWorld::StartUpdatingModel( StgModel* mod )
{
	if( g_list_find( this->update_list, mod ) == NULL )
		this->update_list = g_list_append( this->update_list, mod );
}

void StgWorld::StopUpdatingModel( StgModel* mod )
{
	this->update_list = g_list_remove( this->update_list, mod );
}

// int32_t StgWorld::MetersToPixels( stg_meters_t m )
// {
//   return (int32_t)floor(m * ppm);
// }

int StgWorld::AddBlockPixel( int x, int y, int z,
		stg_render_info_t* rinfo )
{
	// superregion coords
	stg_point_int_t sup;
	sup.x = x >> SRBITS;
	sup.y = y >> SRBITS;

	//printf( "ADDBLOCKPIXEL pixel [%d %d]  S[ %d %d ]\t",
	//  x, y, sup.x, sup.y );

	SuperRegion* sr = (SuperRegion*)
		g_hash_table_lookup( rinfo->world->superregions, (void*)&sup );

	if( sr == NULL ) // no superregion exists here
	{
		//printf( "Creating super region [ %d %d ]\n", sup.x, sup.y );
		sr = rinfo->world->CreateSuperRegion( sup.x, sup.y );

		// the bounds of the world have changed
		stg_point3_t pt;
		pt.x = (sup.x << SRBITS) / rinfo->world->ppm;
		pt.y = (sup.y << SRBITS) / rinfo->world->ppm;
		pt.z = 0;
		rinfo->world->Extend( pt ); // lower left corner of the new superregion

		pt.x = ((sup.x+1) << SRBITS) / rinfo->world->ppm;
		pt.y = ((sup.y+1) << SRBITS) / rinfo->world->ppm;
		pt.z = 0;
		rinfo->world->Extend( pt ); // top right corner of the new superregion
	}

	assert(sr);

	// find the pixel coords inside this superregion
	stg_point_int_t cell;
	cell.x = x - ( sup.x << SRBITS);
	cell.y = y - ( sup.y << SRBITS);

	//printf( "C[ %d %d]", cell.x, cell.y );

	sr->AddBlock( rinfo->block, cell.x, cell.y );

	return FALSE;
}

void StgWorld::Extend( stg_point3_t pt )
{
	extent.x.min = MIN( extent.x.min, pt.x);
	extent.x.max = MAX( extent.x.max, pt.x );
	extent.y.min = MIN( extent.y.min, pt.y );
	extent.y.max = MAX( extent.y.max, pt.y );
	extent.z.min = MIN( extent.z.min, pt.z );
	extent.z.max = MAX( extent.z.max, pt.z );
}