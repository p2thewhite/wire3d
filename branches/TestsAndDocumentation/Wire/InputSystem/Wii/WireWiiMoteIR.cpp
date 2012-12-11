// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireWiiMoteIR.h"

#include "WireWiiInputDataBuffer.h"
#include "WireMath.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteIR, IR);
	
WiiMoteIR::WiiMoteIR(const InputDevice* pParent)
	: IR(pParent)
{
}

WiiMoteIR::~WiiMoteIR()
{
}

Float WiiMoteIR::GetBackward() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return -MathF::MAX_REAL;
	}

	return -pData->ir.z;
}

Float WiiMoteIR::GetDown() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return -MathF::MAX_REAL;
	}

	return -pData->ir.y;
}

Float WiiMoteIR::GetForward() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return MathF::MAX_REAL;
	}

	return pData->ir.z;
}

Float WiiMoteIR::GetLeft() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return -MathF::MAX_REAL;
	}

	return -pData->ir.x;
}

Float WiiMoteIR::GetRight() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return MathF::MAX_REAL;
	}

	return pData->ir.x;
}

Float WiiMoteIR::GetUp() const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return MathF::MAX_REAL;
	}

	return pData->ir.y;
}

Bool WiiMoteIR::IsValid(const WPADWrapperData* pData) const
{
	if (pData == NULL) return false;
	else return pData->ir.valid;
}
