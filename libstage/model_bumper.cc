///////////////////////////////////////////////////////////////////////////
//
// File: model_bumper.c
// Author: Maximilian Ernestus
// Date: 14 February 2014
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

   The bumper is a virtual bump sensor which senses if he hits something
   from the surrounding world. It is red but turns blue if it hits something.

   By default it has a unit square geometry. Use the size property to resize
   the square or place a block with some geometry within the bumper to give it more
   complex shapes.

   API: Stg::ModelBumper
*/

  
ModelBumper::ModelBumper( World* world,
				  	  	  Model* parent,
				  	  	  const std::string& type ):
				  	  			  Model( world, parent, type ),
				  	  			  currentlyBumped(false)
{
  SetObstacleReturn(true);
}

Model* ModelBumper::TestCollision()
{
	Model* hitmod = Model::TestCollision();
	currentlyBumped = hitmod != NULL;
	if(hitmod) SetColor(Color("blue"));
	else SetColor(Color("red"));
	return hitmod;
}
