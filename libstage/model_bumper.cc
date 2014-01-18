///////////////////////////////////////////////////////////////////////////
//
// File: model_blobfinder.c
// Author: Richard Vaughan
// Date: 10 June 2004
//
// CVS info:
//  $Source: /home/tcollett/stagecvs/playerstage-cvs/code/stage/libstage/model_blobfinder.cc,v $
//  $Author: rtv $
//  $Revision$
//
///////////////////////////////////////////////////////////////////////////

#include <sys/time.h>

#include "stage.hh"
#include "option.hh"
#include "worldfile.hh"
using namespace Stg;

static const watts_t DEFAULT_BUMPERWATTS = 2.0;
static const unsigned int DEFAULT_BLOBFINDERINTERVAL_MS = 100;

/**
   @ingroup model
   @defgroup model_bumper Bumper model

   BUMPERDESCRIPTIONHERETODOTODO!!!

   API: Stg::ModelBumper

*/

  
ModelBumper::ModelBumper( World* world,
				  Model* parent,
				  const std::string& type ):
  Model( world, parent, type ), currentlyBumped(false)
//  vis( world )
{
  PRINT_DEBUG1( "Constructing ModelBumper %d \n", id );
//  ClearBlocks();

  SetObstacleReturn(true);
//  AddVisualizer( &this->vis, true );
}




void ModelBumper::Startup(  void )
{ 
  Model::Startup();

  PRINT_DEBUG( "bumper startup" );

  // start consuming power
  SetWatts( DEFAULT_BUMPERWATTS );
}

Model* ModelBumper::TestCollision()
{
	Model* hitmod = Model::TestCollision();
	currentlyBumped = hitmod != NULL;
	if(hitmod) SetColor(Color("blue"));
	else SetColor(Color("red"));
	return hitmod;
}

void ModelBumper::Shutdown( void )
{ 

  PRINT_DEBUG( "blobfinder shutdown" );

  // stop consuming power
  SetWatts( 0 );

  // clear the data - this will unrender it too
  Model::Shutdown();
}

bool ModelBumper::hit()
{
    return currentlyBumped;
}

//******************************************************************************
// visualization

//TODO make instance attempt to register an option (as customvisualizations do)
// Option ModelBlobfinder::showBlobData( "Show Blobfinder", "show_blob", "", true, NULL );

//ModelBlobfinder::Vis::Vis( World* world )
//  : Visualizer( "Blobfinder", "blobfinder_vis" )
//{
  
  //world->RegisterOption( &showArea );
  //world->RegisterOption( &showStrikes );
  //world->RegisterOption( &showFov );
  //world->RegisterOption( &showBeams );		  
//}

//void ModelBlobfinder::Vis::Visualize( Model* mod, Camera* cam )
//{
//  ModelBlobfinder* bf( dynamic_cast<ModelBlobfinder*>(mod) );
  
//  if( bf->debug )
//    {
//      // draw the FOV
//      GLUquadric* quadric = gluNewQuadric();
	  
//      bf->PushColor( 0,0,0,0.2  );
	  
//      gluQuadricDrawStyle( quadric, GLU_SILHOUETTE );
//      gluPartialDisk( quadric,
//		      0,
//		      bf->range,
//		      20, // slices
//		      1, // loops
//		      rtod( M_PI/2.0 + bf->fov/2.0 - bf->pan), // start angle
//		      rtod(-bf->fov) ); // sweep angle
	  
//      gluDeleteQuadric( quadric );
//      bf->PopColor();
//    }
  
//  if( bf->subs < 1 )
//    return;
  
//  glPushMatrix();

//  // return to global rotation frame
//  Pose gpose( bf->GetGlobalPose() );
//  glRotatef( rtod(-gpose.a),0,0,1 );
  
//  // place the "screen" a little away from the robot
//  glTranslatef( -2.5, -1.5, 0.5 );
  
//  // rotate to face screen
//  float yaw, pitch;
//  pitch = - cam->pitch();
//  yaw = - cam->yaw();
//  float robotAngle = -rtod(bf->pose.a);
//  glRotatef( robotAngle - yaw, 0,0,1 );
//  glRotatef( -pitch, 1,0,0 );
  
//  // convert blob pixels to meters scale - arbitrary
//  glScalef( 0.025, 0.025, 1 );
  
//  // draw a white screen with a black border
//  bf->PushColor( 1,1,1,1 );
//  glRectf( 0,0, bf->scan_width, bf->scan_height );
//  bf->PopColor();
  
//  glTranslatef(0,0,0.01 );
  
//  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//  bf->PushColor( 1,0,0,1 );
//  glRectf( 0,0, bf->scan_width, bf->scan_height );
//  bf->PopColor();
//  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  
//  // draw the blobs on the screen
//  for( unsigned int s=0; s<bf->blobs.size(); s++ )
//    {
//      Blob* b = &bf->blobs[s];
//      //blobfinder_blob_t* b =
//      //&g_array_index( blobs, blobfinder_blob_t, s);
		
//      bf->PushColor( b->color );
//      glRectf( b->left, b->top, b->right, b->bottom );

//      //printf( "%u l %u t%u r %u b %u\n", s, b->left, b->top, b->right, b->bottom );
//      bf->PopColor();
//    }
  
//  glPopMatrix();
//}


