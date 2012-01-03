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
	Node* LoadAndInitLogo();
	Node* LoadAndInitScene1();
	Double mLastTime;

	NodePtr mspLogo;
	TArray<CameraPtr> mLogoCameras;
	Culler mLogoCuller;

	NodePtr mspScene1;
	TArray<CameraPtr> mScene1Cameras;
	CullerSorting mScene1Culler;
};

WIRE_REGISTER_INITIALIZE(Demo);

#endif
