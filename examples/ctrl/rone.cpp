#include "stage.hh"
#include "rone.h"

int robotUpdateWrapper(Stg::Model* model, ROne* robot)
{
	robot->update();
	return 0;
}

ROne::ROne(Stg::ModelPosition* pos) : pos(pos)
{
	pos->Subscribe();
	pos->AddCallback(Stg::Model::CB_UPDATE, (Stg::model_callback_t)robotUpdateWrapper, this);

	std::string nameProperty = "name";
	std::vector<Stg::Model*> children = pos->GetChildren();
	for(int i = 0; i < children.size(); i++)
		std::cout << "There is a Child called " << children[i]->GetProperty(nameProperty) << std::endl;

	acc = (Stg::ModelAccelerometer* ) pos->GetChild("accelerometer:0");
	acc->Subscribe();

	for(int i = 0; i < 8; i++)
	{
		irSenders[i] = (Stg::ModelIRComm *) pos->GetChild(makeIRCommName(i*2));
		irReceivers[i] = (Stg::ModelIRComm *) pos->GetChild(makeIRCommName(i*2+1));
		bumpers[i] = (Stg::ModelBumper *) pos->GetChild(makeBumperName(i));

		irSenders[i]->Subscribe();
		irReceivers[i]->Subscribe();
		bumpers[i]->Subscribe();
	}
}
