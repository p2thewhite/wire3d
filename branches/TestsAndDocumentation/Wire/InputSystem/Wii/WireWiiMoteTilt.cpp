// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireWiiMoteTilt.h"

#include "WireWiiInputDataBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteTilt, Tilt);
	
WiiMoteTilt::WiiMoteTilt(const InputDevice* pParent)
	: Tilt(pParent)
{
}

WiiMoteTilt::~WiiMoteTilt()
{
}

Float WiiMoteTilt::GetBackward() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return pData->orient.pitch;
}

Float WiiMoteTilt::GetForward() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return -pData->orient.pitch;
}

Float WiiMoteTilt::GetLeft() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return pData->orient.roll;
}

Float WiiMoteTilt::GetRight() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return -pData->orient.roll;
}
