#pragma once
#ifndef CUBECONTROLLER_H
#define CUBECONTROLLER_H

#include "WireController.h"

using namespace Wire;

class CubeController : public Controller
{

public:
	CubeController();
	virtual ~CubeController();

	virtual Bool Update(Double appTime);

private:
	Float mAngle;
};

#endif
