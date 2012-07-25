// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireWin32KeyMappedAnalogPad.h"
#include "WireWin32InputDataBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Win32KeyMappedAnalogPad, AnalogPad);

Win32KeyMappedAnalogPad::Win32KeyMappedAnalogPad(const InputDevice* pParent)
	: AnalogPad(pParent)
{
}

Win32KeyMappedAnalogPad::~Win32KeyMappedAnalogPad()
{
}

Float Win32KeyMappedAnalogPad::GetDown() const
{
	return 0;
}

Float Win32KeyMappedAnalogPad::GetLeft() const
{
	return 0;
}

Float Win32KeyMappedAnalogPad::GetRight() const
{
	return 0;
}

Float Win32KeyMappedAnalogPad::GetUp() const
{
	return 0;
}
