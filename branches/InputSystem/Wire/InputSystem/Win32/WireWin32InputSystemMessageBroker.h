// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32INPUTSYSTEMMESSAGEBROKER_H
#define WIREWIN32INPUTSYSTEMMESSAGEBROKER_H

#include "WireWin32InputDataBuffer.h"
#include "WireTypes.h"

namespace Wire
{

class Win32InputSystemMessageBroker
{

public:
	Win32InputSystemMessageBroker();
	virtual ~Win32InputSystemMessageBroker();
	const Win32InputDataBuffer* GetCurrentInputDataBuffer() const;
	static Win32InputSystemMessageBroker* GetInstance();
	void SwapBuffers();
	Bool OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter);

private:
	static Win32InputSystemMessageBroker* s_pInstance;

	Win32InputDataBuffer* mpFrontBuffer;
	Win32InputDataBuffer* mpBackBuffer;
};

}

#endif
