#pragma once
#ifndef DEMO_H
#define DEMO_H

#include "WireApplication.h"

using namespace Wire;

class Demo : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Node* LoadAndInitScene1();
	Double mLastTime;

	CameraPtr mspCamera;
	CullerSorting mCuller;

	NodePtr mspScene1;
	TArray<CameraPtr> mCameras;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
