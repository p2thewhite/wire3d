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

Double Win32InputSystem::s_MouseStagnationTolerance = 5; // 5 seconds

Win32InputSystem::Win32InputSystem() :
	mLastMouseMoveTime(0)
{
	mpFrontBuffer = WIRE_NEW Win32InputDataBuffer();
	mpBackBuffer = WIRE_NEW Win32InputDataBuffer();
}

Win32InputSystem::~Win32InputSystem()
{
	WIRE_DELETE mpFrontBuffer;
	WIRE_DELETE mpBackBuffer;
}

void Win32InputSystem::Capture()
{
	if (IsMouseStagnant())
	{
		ResetMousePosition();
	}

	// Swap input data buffers
	SwapBuffers();

	// Update the current data buffer on each input device
	for (UInt i = 0; i < mDevices.GetQuantity(); i++)
	{
		mDevices[i]->SetDataBuffer(mpFrontBuffer);
	}
}

void Win32InputSystem::DoDevicesDiscovery()
{
	// FIXME: Since there's no way to know how many keyboards and mouses are connected using Win32 api, 
	// assume there will always be 1 main device and 1 extension (1 emulated wiimote + 1 emulated nunchuk)
	if (GetDevicesCount() == 2)
	{
		return;
	}

	Win32EmulatedWiiMote* pEmulatedWiiMote = WIRE_NEW Win32EmulatedWiiMote();
	AddDevice(pEmulatedWiiMote);
	pEmulatedWiiMote->SetUp();

	Win32EmulatedNunchuk* pEmulatedNunchuk = WIRE_NEW Win32EmulatedNunchuk(pEmulatedWiiMote);
	AddDevice(pEmulatedNunchuk);
	pEmulatedWiiMote->AddExtension("Nunchuk", pEmulatedNunchuk);
	pEmulatedNunchuk->SetUp();

	NotifyDevicesChange();
}

/**
 * FIXME: needs thread safety!!!
 */
void Win32InputSystem::SwapBuffers()
{
	Win32InputDataBuffer* pOldFrontBuffer = mpFrontBuffer;
	mpFrontBuffer = mpBackBuffer;
	mpBackBuffer = pOldFrontBuffer;

	mpBackBuffer->CopyFrom(mpFrontBuffer);
}

Bool Win32InputSystem::OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter)
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
		mpBackBuffer->IncrementMouseWheel(static_cast<Float>(mouseWheelDelta)/WHEEL_DELTA);
		return true;
	case WM_MOUSEMOVE:
		mLastMouseMoveTime = System::GetTime();

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

Bool Win32InputSystem::IsMouseStagnant()
{
	return (System::GetTime() - mLastMouseMoveTime) > s_MouseStagnationTolerance;
}

void Win32InputSystem::ResetMousePosition()
{
	mpBackBuffer->SetMouseX(MathF::MAX_REAL);
	mpBackBuffer->SetMouseY(MathF::MAX_REAL);
}
