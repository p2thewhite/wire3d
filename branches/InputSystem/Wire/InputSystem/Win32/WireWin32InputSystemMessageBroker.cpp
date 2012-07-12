// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireWin32InputSystemMessageBroker.h"
#include "WireMemory.h"
#include <Windows.h>
#include <WinUser.h>
#include <Windowsx.h>

using namespace Wire;

Win32InputSystemMessageBroker* Win32InputSystemMessageBroker::s_pInstance = NULL;

Win32InputSystemMessageBroker::Win32InputSystemMessageBroker()
{
	mpFrontBuffer = WIRE_NEW Win32InputDataBuffer();
	mpBackBuffer = WIRE_NEW Win32InputDataBuffer();
}

Win32InputSystemMessageBroker::~Win32InputSystemMessageBroker()
{
	WIRE_DELETE mpFrontBuffer;
	WIRE_DELETE mpBackBuffer;
}

const Win32InputDataBuffer* Win32InputSystemMessageBroker::GetCurrentInputDataBuffer() const
{
	return mpFrontBuffer;
}

Win32InputSystemMessageBroker* Win32InputSystemMessageBroker::GetInstance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = WIRE_NEW Win32InputSystemMessageBroker();
	}

	return s_pInstance;
}

/**
 * FIXME: needs thread safety!!!
 */
void Win32InputSystemMessageBroker::SwapBuffers()
{
	Win32InputDataBuffer* pOldFrontBuffer = mpFrontBuffer;
	mpFrontBuffer = mpBackBuffer;
	mpBackBuffer = pOldFrontBuffer;

	mpBackBuffer->CopyFrom(mpFrontBuffer);
}

Bool Win32InputSystemMessageBroker::OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter)
{
	Int x; 
	Int y;
	Int mouseWheelDelta;

	switch (messageType)
	{
	case WM_MBUTTONDOWN:
		mpBackBuffer->SetKeyDown(VK_MBUTTON);
		return true;
	case WM_MBUTTONUP:
		mpBackBuffer->SetKeyUp(VK_MBUTTON);
		return true;
	case WM_RBUTTONDOWN:
		mpBackBuffer->SetKeyDown(VK_RBUTTON);
		return true;
	case WM_RBUTTONUP:
		mpBackBuffer->SetKeyUp(VK_RBUTTON);
		return true;
	case WM_LBUTTONDOWN:
		mpBackBuffer->SetKeyDown(VK_LBUTTON);
		return true;
	case WM_LBUTTONUP:
		mpBackBuffer->SetKeyUp(VK_LBUTTON);
		return true;
	case WM_MOUSEWHEEL:
		mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wordParameter);
		mpBackBuffer->IncrementMouseWheel(static_cast<Float>(mouseWheelDelta)/
			WHEEL_DELTA);
		return true;
	case WM_MOUSEMOVE:
		x = GET_X_LPARAM(longParameter); 
		y = GET_Y_LPARAM(longParameter);

		mpBackBuffer->SetMouseX(static_cast<Float>(x));
		mpBackBuffer->SetMouseY(static_cast<Float>(y));

		return true;
	case WM_KEYDOWN:
		mpBackBuffer->SetKeyDown((UInt) wordParameter);
		return true;
	case WM_KEYUP:
		mpBackBuffer->SetKeyUp((UInt) wordParameter);
		return true;
	}

	return false;
}
