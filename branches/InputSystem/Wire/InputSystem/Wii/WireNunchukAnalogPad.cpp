// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNunchukAnalogPad.h"
#include "WireWiiInputDataBuffer.h"
#include <wiiuse/wpad.h>

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, NunchukAnalogPad, AnalogPad);
	
NunchukAnalogPad::NunchukAnalogPad(const InputDevice* pParent)
	: AnalogPad(pParent)
{
}

NunchukAnalogPad::~NunchukAnalogPad()
{
}

Float NunchukAnalogPad::GetDown() const
{
	if (GetParent()->GetDataBuffer() == NULL)
	{
		return false;
	}

	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return -pData->exp.nunchuk.js.pos.y;
}

Float NunchukAnalogPad::GetLeft() const
{
	if (GetParent()->GetDataBuffer() == NULL)
	{
		return false;
	}

	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return pData->exp.nunchuk.js.pos.x;
}

Float NunchukAnalogPad::GetRight() const
{
	if (GetParent()->GetDataBuffer() == NULL)
	{
		return false;
	}

	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return -pData->exp.nunchuk.js.pos.x;
}

Float NunchukAnalogPad::GetUp() const
{
	if (GetParent()->GetDataBuffer() == NULL)
	{
		return false;
	}

	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return pData->exp.nunchuk.js.pos.y;
}
