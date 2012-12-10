// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireWin32InputSystem.h"
#include "WireWin32EmulatedWiiMote.h"
#include "WireWin32EmulatedNunchuk.h"
#include <Windows.h>
#include <Windowsx.h>

using namespace Wire;

Win32InputSystem::Win32InputSystem()
{
	mpFrontInputDataBuffer = WIRE_NEW Win32InputDataBuffer();
	mpBackInputDataBuffer = WIRE_NEW Win32InputDataBuffer();
}

Win32InputSystem::~Win32InputSystem()
{
	WIRE_DELETE mpFrontInputDataBuffer;
	WIRE_DELETE mpBackInputDataBuffer;
}

void Win32InputSystem::Capture()
{
	// Swap input data buffers
	SwapInputDataBuffers();

	// Update the current data buffer on each input device
	for (UInt i = 0; i < mDevices.GetQuantity(); i++)
	{
		mDevices[i]->SetInputDataBuffer(mpFrontInputDataBuffer);
	}
}

Bool Win32InputSystem::DiscoverDevices()
{
	// FIXME: Since there's no way to know how many keyboards and mouses are connected using Win32 api, 
	// assume there will always be 1 main device and 1 extension (1 emulated wiimote + 1 emulated nunchuk)
	if (GetDevicesCount() == 2)
	{
		return false;
	}

	Win32EmulatedWiiMote* pEmulatedWiiMote = WIRE_NEW Win32EmulatedWiiMote();
	AddDevice(pEmulatedWiiMote);
	pEmulatedWiiMote->SetUp();

	Win32EmulatedNunchuk* pEmulatedNunchuk = WIRE_NEW Win32EmulatedNunchuk(pEmulatedWiiMote);
	AddDevice(pEmulatedNunchuk);
	pEmulatedWiiMote->AddExtension(pEmulatedNunchuk);
	pEmulatedNunchuk->SetUp();

	return true;
}

/**
 * FIXME: needs thread safety!!!
 */
void Win32InputSystem::SwapInputDataBuffers()
{
	Win32InputDataBuffer* pOldFrontBuffer = mpFrontInputDataBuffer;
	mpFrontInputDataBuffer = mpBackInputDataBuffer;
	mpBackInputDataBuffer = pOldFrontBuffer;

	mpBackInputDataBuffer->CopyFrom(mpFrontInputDataBuffer);
}

Bool Win32InputSystem::OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter)
{
	Int x; 
	Int y;
	Int mouseWheelDelta;

	switch (messageType)
	{
	case WM_MBUTTONDOWN:
		mpBackInputDataBuffer->SetKeyDown(VK_MBUTTON);
		return true;
	case WM_MBUTTONUP:
		mpBackInputDataBuffer->SetKeyUp(VK_MBUTTON);
		return true;
	case WM_RBUTTONDOWN:
		mpBackInputDataBuffer->SetKeyDown(VK_RBUTTON);
		return true;
	case WM_RBUTTONUP:
		mpBackInputDataBuffer->SetKeyUp(VK_RBUTTON);
		return true;
	case WM_LBUTTONDOWN:
		mpBackInputDataBuffer->SetKeyDown(VK_LBUTTON);
		return true;
	case WM_LBUTTONUP:
		mpBackInputDataBuffer->SetKeyUp(VK_LBUTTON);
		return true;
	case WM_MOUSEWHEEL:
		mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wordParameter);
		mpBackInputDataBuffer->IncrementMouseWheel(static_cast<Float>(mouseWheelDelta)/WHEEL_DELTA);
		return true;
	case WM_MOUSEMOVE:
		x = GET_X_LPARAM(longParameter); 
		y = GET_Y_LPARAM(longParameter);

		mpBackInputDataBuffer->SetMouseX(static_cast<Float>(x));
		mpBackInputDataBuffer->SetMouseY(static_cast<Float>(y));

		return true;
	case WM_KEYDOWN:
		mpBackInputDataBuffer->SetKeyDown((UInt) wordParameter);
		return true;
	case WM_KEYUP:
		mpBackInputDataBuffer->SetKeyUp((UInt) wordParameter);
		return true;
	}

	return false;
}
