#pragma once
#ifndef SAMPLE11_H
#define SAMPLE11_H

#include "WireApplication.h"
#include "WireInputDeviceDiscoveryListener.h"
#include <sstream>

using namespace std;
using namespace Wire;

class Sample11 : public WIREAPPLICATION, public InputDeviceDiscoveryListener
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Sample11();
	virtual Bool OnInitialize();
	virtual void OnIdle();

	enum CursorMode
	{
		CM_POINTING,
		CM_PRIMARY_BUTTON_PRESSED,
		CM_SECONDARY_BUTTON_PRESSED,
		CM_OFF
	};

	void SetCursor(Int x, Int y, CursorMode mode = CM_POINTING,
		UInt playerNo = 0, Float zRollInRadian = 0);

	virtual void Update();

private:
	void InitCursors();
	Geometry* CreateCursor(Float uOffset, Float vOffset);
	void PrintAndClear(stringstream& message);
	void PrintInputDevicesInformation();
	void PrintKeyStates();

	Bool mInputDevicesStateChanged;

	Culler mCuller;
	CameraPtr mspGuiCamera;

	NodePtr mspCursors;
	MaterialPtr mspMaterial;
	StateAlphaPtr mspCursorsAlpha;

};

WIRE_REGISTER_INITIALIZE(Sample11);

#endif
