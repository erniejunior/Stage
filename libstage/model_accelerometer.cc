///////////////////////////////////////////////////////////////////////////
//
// File: model_accelerometer.c
// Author: Maximilian Ernestus
// Date: 13 February 2013
//
///////////////////////////////////////////////////////////////////////////

#include <sys/time.h>

#include "stage.hh"
#include "option.hh"
#include "worldfile.hh"
using namespace Stg;

/**
   @ingroup model
   @defgroup model_accelerometer Accelerometer model

   Attach this acceleration sensor to any model to measure its acceleration.
   Linear acceleration is in \f$m/s^2\f$ while rotational acceleration is in \f$rad/s^2\f$.

   API: Stg::ModelAccelerometer

   Two kinds of acceleration are computed: *global* acceleration and *local* acceleration.


   Global acceleration
   -------------------
   Is the acceleration as seen "from above" from a global point of view.

   Most importantly, if your robot drives in a different direction the
   sensed acceleration will be in a different direction

   Local acceleration
   ------------------
   Is the acceleration as perceived from an acceleration sensor placed on your robot.

   Most importantly, if the robot is driving forward, the sensor will sense forward acceleration,
   no matter what global direction the robot is currently heading.

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

		Velocity global_velocity = Velocity((global_pose.x - prev_global_pose.x)/interval,
											(global_pose.y - prev_global_pose.y)/interval,
											(global_pose.z - prev_global_pose.z)/interval,
											(global_pose.a - prev_global_pose.a)/interval);

		global_acc_x = (global_velocity.x - prev_global_velocity.x)/interval;
		global_acc_y = (global_velocity.y - prev_global_velocity.y)/interval;
		global_acc_z = (global_velocity.z - prev_global_velocity.z)/interval;
		global_acc_a = (global_velocity.a - prev_global_velocity.a)/interval;

		//To compute the measured velocity we rotate the global velocity "back"
		double cosTheta = cos(-global_pose.a);
		double sinTheta = sin(-global_pose.a);

		acc_x = global_acc_x * cosTheta - global_acc_y * sinTheta;
		acc_y = global_acc_x * sinTheta + global_acc_y * cosTheta;
		acc_z = global_acc_z;
		acc_a = global_acc_a;

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
