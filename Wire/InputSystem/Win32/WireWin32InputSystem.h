// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32INPUTSYSTEM_H
#define WIREWIN32INPUTSYSTEM_H

#include "WireInputSystem.h"
#include "WireWin32InputDataBuffer.h"

namespace Wire
{

/**
 * Win32InputSystem is the specialization of the InputSystem on the Windows platform.
 * It handles all platform dependent operations for the input data capturing and device discovery.
 *
 * See also: InputSystem
 **/
class Win32InputSystem : public InputSystem
{

public:
	Win32InputSystem();
	virtual ~Win32InputSystem();

	/// Tries to discover all available input devices.
	virtual void Capture();
	/// Consolidates all input data at his disposal.
	virtual Bool DiscoverDevices();
	/// Decodes Win32 system messages and stores input data in the back input data buffer. Used by the application layer and should not be called directly by the user.
	Bool OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter);

protected:
	Win32InputDataBuffer* mpFrontInputDataBuffer;
	Win32InputDataBuffer* mpBackInputDataBuffer;

	void SwapInputDataBuffers();

};

}

#endif
