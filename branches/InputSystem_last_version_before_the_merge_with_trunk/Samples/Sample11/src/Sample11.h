#pragma once
#ifndef SAMPLE11_H
#define SAMPLE11_H

#include "WireApplication.h"
#include "WireInputSystemListener.h"
#include <sstream>

using namespace std;
using namespace Wire;

class Sample11 : public WIREAPPLICATION, public InputSystemListener
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Sample11();
	virtual Bool OnInitialize();
	virtual void OnIdle();
	virtual void OnInputCapture();

	enum CursorMode
	{
		CM_POINTING,
		CM_PRIMARY_BUTTON_PRESSED,
		CM_SECONDARY_BUTTON_PRESSED,
		CM_OFF
	};

	void SetCursor(Float x, Float y, CursorMode mode = CM_POINTING,
		UInt playerNo = 0, Float zRollInRadian = 0);

	virtual void OnDevicesChange();

private:
	void InitCursors();
	Geometry* CreateCursor(Float uOffset, Float vOffset);

	// utilitarian
	void PrintAndClear(stringstream& message);

	// debug output
	void PrintInputDevicesInformations();

	Bool mInputDevicesStateChanged;

	Culler mCuller;
	CameraPtr mspGuiCamera;

	NodePtr mspCursors;
	MaterialPtr mspMaterial;
	StateAlphaPtr mspCursorsAlpha;

};

WIRE_REGISTER_INITIALIZE(Sample11);

#endif
