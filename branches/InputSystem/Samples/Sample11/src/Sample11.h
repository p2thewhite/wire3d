#pragma once
#ifndef SAMPLE11_H
#define SAMPLE11_H

#include "WireApplication.h"
#include "Cursors.h"
#include <sstream>

using namespace Wire;

class Sample11 : public WIREAPPLICATION, public InputSystemListener
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Sample11();

	virtual Bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();
	virtual void OnInput();
	virtual void OnDevicesChange();

private:
	// utilitarian
	void PrintAndClear(std::stringstream& message);

	// debug output
	void PrintInputDevicesInformations();

	Bool mInputDevicesStateChanged;

	Culler mCuller;
	CameraPtr mspGuiCamera;
	NodePtr mspGuiRoot;

	Cursors* mpCursors;
};

WIRE_REGISTER_INITIALIZE(Sample11);

#endif
