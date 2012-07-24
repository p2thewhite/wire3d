#pragma once
#ifndef SAMPLE11_H
#define SAMPLE11_H

#include "WireApplication.h"
#include "Cursors.h"

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
	void UpdateInputDevicesInformationText();

	Bool mInputDevicesStateChanged;

	Culler mCuller;
	CameraPtr mspGuiCamera;
	NodePtr mspGuiRoot;
	TextPtr mspText;

	Cursors* mpCursors;
};

WIRE_REGISTER_INITIALIZE(Sample11);

#endif
