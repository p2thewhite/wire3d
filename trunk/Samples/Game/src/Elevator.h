#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "WireController.h"

class Elevator : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	Elevator();
	virtual ~Elevator();

//	virtual Bool Update(Double appTime);
};

#endif
