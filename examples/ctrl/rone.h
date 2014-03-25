#ifndef RONE_H
#define RONE_H

#include "stage.hh"
#include <sstream>

class ROne
{
private:

	std::string makeName(const char* prefix, int idx)
	{
		std::stringstream stringStream;
		stringStream << prefix << ":" << idx;
		return stringStream.str();
	}
protected:
//	std::string name;

	std::string makeBumperName(int index)
	{
		return makeName("bumper", index);
	}

	std::string makeIRCommName(int index)
	{
		return makeName("ircomm", index);
	}

public:
	ROne(Stg::ModelPosition* pos);
	virtual ~ROne() {}

	Stg::ModelPosition* pos;
	Stg::ModelAccelerometer* acc;
	Stg::ModelIRComm* irSenders[8];
	Stg::ModelIRComm* irReceivers[8];
	Stg::ModelBumper* bumpers[8];

	bool isValidROne()
	{
//		if(!acc) return false;
		for(int i = 0; i < 8; i++)
		{
			if(!bumpers[i]) return false;
			if(!irReceivers[i]) return false;
			if(!irSenders[i]) return false;
		}
		return true;
	}

	virtual void update() {}
};

#endif
