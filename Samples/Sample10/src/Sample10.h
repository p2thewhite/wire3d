#pragma once
#ifndef SAMPLE10_H
#define SAMPLE10_H

#include "WireApplication.h"

using namespace Wire;

class Sample10 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	GeometryPtr mspGeo;
	CameraPtr mspCamera;
	Culler mCuller;
};

WIRE_REGISTER_INITIALIZE(Sample10);

#endif
