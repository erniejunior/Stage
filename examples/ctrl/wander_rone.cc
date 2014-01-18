#include "stage.hh"
using namespace Stg;

typedef struct
{
  ModelPosition* pos;
  ModelBumper* bumper;
  ModelIRComm* ircomm [8];
} robot_t;

bool currentDirection = true;

int PositionUpdate( ModelPosition* mod, robot_t* robot );
int FiducialUpdate( ModelFiducial* fid, robot_t* robot );

// Stage calls this when the model starts up
extern "C" int Init( Model* mod, CtrlArgs* args )
{
  // local arguments
  /*  printf( "\nWander controller initialised with:\n"
      "\tworldfile string \"%s\"\n" 
      "\tcmdline string \"%s\"",
      args->worldfile.c_str(),
      args->cmdline.c_str() );
  */

  robot_t* robot = new robot_t;
  
  robot->pos = (ModelPosition*)mod;
  robot->pos->Subscribe();

//  robot->bumper = (ModelBumper*) mod->GetChild("bumper:0");
//  robot->bumper->Subscribe();
//
  	int cntr = 0;
    for(int i = 0; i < 16; i++)
    {
    	ModelIRComm * ir = (ModelIRComm* )mod->GetUnsubscribedModelOfType("ircomm");
    	ir->Subscribe();
    	if(ir->isSender())
    	{
    		robot->ircomm[cntr] = ir;
    		cntr++;
    	}
    }

//  robot->pos->Say("Hello!");

  robot->pos->SetXSpeed(0.1);
  mod->GetChild("accelerometer:0")->Subscribe();

  robot->pos->AddCallback( Model::CB_UPDATE, (model_callback_t)PositionUpdate, robot );
//  robot->ircomm->AddCallback(Model::CB_UPDATE, (model_callback_t)FiducialUpdate, robot);
 
//  robot->pos->GetChild( "ranger:0" )->Subscribe();
 
//  robot->laser = (ModelRanger*)mod->GetChild( "ranger:1" );
//  robot->laser->AddCallback( Model::CB_UPDATE, (model_callback_t)LaserUpdate, robot );
//  robot->laser->Subscribe(); // starts the ranger updates
  
  return 0; //ok
}

int counter = 0;

int PositionUpdate( ModelPosition* mod, robot_t* robot )
{

//  if(robot->bumper->hit())
//  {
//	  currentDirection = !currentDirection;
//	  robot->pos->SetXSpeed(currentDirection ? 0.2 : -0.2);
//	  robot->pos->SetTurnSpeed(0.2);
//  }

//	std::cout << "Vel X:" << mod->GetVelocity().x << std::endl;
//	std::cout << "Accel Y: " << ((ModelAccelerometer*)mod->GetChild("accelerometer:0"))->GetAccY() << std::endl;
//	std::cout << "Global Accel Y: " << ((ModelAccelerometer*)mod->GetChild("accelerometer:0"))->GetGlobalAccY() << std::endl;
//	std::cout << std::endl;

  counter++;
  if(counter % 10 == 0)
  {
	  for(int i = 0; i < 8; i++)
		  robot->ircomm[i]->sendMessage("Some Data");
  }

  if(counter % 50 == 0)
    {
  	  for(int i = 0; i < 8; i++)
  		  robot->ircomm[i]->sendMessage("");
    }




//  printf( "Pose: [%.2f %.2f %.2f %.2f]\n",
//	  pose.x, pose.y, pose.z, pose.a );

  return 0; // run again
}

int FiducialUpdate( ModelFiducial* fid, robot_t* robot )
{
	return 0;
}

