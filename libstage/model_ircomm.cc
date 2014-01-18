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

#define DEBUG

#include <sys/time.h>

#include "stage.hh"
#include "option.hh"
#include "worldfile.hh"

using namespace Stg;


/**
   @ingroup model
   @defgroup model_bumper Bumper model

   BUMPERDESCRIPTIONHERETODOTODO!!!

   API: Stg::ModelBumper

*/

  
ModelIRComm::ModelIRComm( World* world,
				  Model* parent,
				  const std::string& type ):
  ModelFiducial(world, parent, type), is_sender(1), is_receiver(1)
{
  PRINT_DEBUG1( "Constructing ModelFiducial %d \n", id );
  SetFiducialReturn(1);
  ignore_zloc = 1;
  max_range_anon = 3;
  max_range_id = 3;
//  key = 123213123;
}

void ModelIRComm::Load( void )
{
	ModelFiducial::Load();
	is_sender = wf->ReadInt( wf_entity, "is_sender",	1);
	is_receiver = wf->ReadInt( wf_entity, "is_receiver",	1);

	SetFiducialReturn(1);
	ignore_zloc = 1;
	min_range = 0;
	max_range_anon = std::max(max_range_anon, max_range_id);
	max_range_anon = std::max(max_range_anon, max_range_id);
	SetFiducialKey(12341234);
	key = 12341234;
}

void ModelIRComm::sendMessage(const std::string& data)
{
	if(is_sender)
	{
//		std::cout << "I send data to " << GetFiducials().size() << " others!" << std::endl;

		FOR_EACH( it, GetFiducials() )
		{
//			std::cout << "hit other fiducial!" << std::endl;

			ModelFiducial::Fiducial* other = &(*it);
			ModelIRComm* otherIR = (ModelIRComm*)other->mod;
			otherIR->pushMessage(data);
		}
	}
}

void ModelIRComm::pushMessage(const std::string& data)
{
	if(is_receiver)
	{
		message_queue.push(data);
		std::cout << "I received: " << data << std::endl;
	}
}

bool ModelIRComm::isMessageAvailable()
{
	return message_queue.size() > 0;
}

std::string ModelIRComm::getNextMessage()
{
	if(isMessageAvailable())
	{
		std::string message = message_queue.front();
		message_queue.pop();
		return message;
	}
	else
		return NULL;
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


