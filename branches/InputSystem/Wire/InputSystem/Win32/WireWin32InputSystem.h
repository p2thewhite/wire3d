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

namespace Wire
{

class Win32InputSystem : public InputSystem
{

public:
	Win32InputSystem();
	virtual ~Win32InputSystem();

	virtual void Capture();
protected:
	virtual void DoDevicesDiscovery();

};

}

#endif
