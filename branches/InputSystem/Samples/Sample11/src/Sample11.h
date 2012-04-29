#pragma once
#ifndef SAMPLE11_H
#define SAMPLE11_H

#include "WireApplication.h"

using namespace Wire;

class Sample11 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
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

private:
	void InitCursors();
	Geometry* CreateCursor(Float uOffset, Float vOffset);

	Culler mCuller;
	CameraPtr mspGuiCamera;

	NodePtr mspCursors;
	MaterialPtr mspMaterial;
	StateAlphaPtr mspCursorsAlpha;

};

WIRE_REGISTER_INITIALIZE(Sample11);

#endif
