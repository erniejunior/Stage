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

/**
   @ingroup model
   @defgroup model_bumper Bumper model

   BUMPERDESCRIPTIONHERETODOTODO!!!

   API: Stg::ModelBumper

*/


ModelAccelerometer::ModelAccelerometer( World* world,
				  Model* parent,
				  const std::string& type ):
  Model( world, parent, type ),
  acc_x(0), acc_y(0), acc_z(0), acc_a(0),
  global_acc_x(0), global_acc_y(0), global_acc_z(0), global_acc_a(0),
  prev_global_velocity()
{
  PRINT_DEBUG1( "Constructing ModelAccelerometer %d \n", id );
  ClearBlocks();
}

void ModelAccelerometer::Update()
{
	Model::Update();
	if(this->subs)
	{
		// convert usec to sec
		const double interval( (double)world->sim_interval / 1e6 );

		Pose global_pose = parent->GetGlobalPose();



		Velocity global_velocity = Velocity(global_pose.x - prev_global_pose.x,
											global_pose.y - prev_global_pose.y,
											global_pose.z - prev_global_pose.z,
											global_pose.a - prev_global_pose.a);

		global_acc_x = (global_velocity.x - prev_global_velocity.x)/interval;
		global_acc_y = (global_velocity.y - prev_global_velocity.y)/interval;
		global_acc_z = (global_velocity.z - prev_global_velocity.z)/interval;
		global_acc_a = (global_velocity.a - prev_global_velocity.a)/interval;

		//The measured velocity is the global velocity rotated
		double cosTheta = cos(-global_pose.a);
		double sinTheta = sin(-global_pose.a);

		acc_x = global_acc_x * cosTheta - global_acc_y * sinTheta;
		acc_y = global_acc_x * sinTheta + global_acc_y * cosTheta;
		acc_z = global_acc_z;
		acc_a = global_acc_a;

//		std::cout << "Global x:" << global_acc_x << " y:" << global_acc_y << std::endl;
//		std::cout << "x:" << acc_x << " y:" << acc_y << std::endl;
//		std::cout << "Global force: " << sqrt(global_acc_x*global_acc_x + global_acc_y*global_acc_y) << std::endl;
//		std::cout << "Force: " << sqrt(acc_x*acc_x + acc_y*acc_y) << std::endl;
//		std::cout << "Rot: " << global_pose.a << std::endl;
//		std::cout << std::endl;

		prev_global_velocity = global_velocity;
		prev_global_pose = global_pose;
	}
	else
	{
		acc_x = acc_y = acc_z = acc_a = 0;
		global_acc_x = global_acc_y = global_acc_z = global_acc_a = 0;
		prev_global_velocity = Velocity();
	}

}
