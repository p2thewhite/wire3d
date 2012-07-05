#include "WireWiiMoteIR.h"
#include "WireWiiMoteButtons.h"
#include "WireWiiMote.h"
#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(WiiMoteIR, IR);
	
WiiMoteIR::WiiMoteIR(const InputDevice* pParent)
	: IR(pParent)
{
}

WiiMoteIR::~WiiMoteIR()
{
}

Float WiiMoteIR::GetBackward() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return 0;
	}

	return -pData->ir.z;
}

Float WiiMoteIR::GetDown() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return 0;
	}

	return -pData->ir.y;
}

Float WiiMoteIR::GetForward() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return 0;
	}

	return pData->ir.z;
}

Float WiiMoteIR::GetLeft() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return 0;
	}

	return -pData->ir.x;
}

Float WiiMoteIR::GetRight() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return 0;
	}

	return pData->ir.x;
}

Float WiiMoteIR::GetUp() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (!IsValid(pData)) 
	{
		return 0;
	}

	return pData->ir.y;
}

Bool WiiMoteIR::IsValid(const WPADData* pData) const
{
	if (pData == NULL) return false;
	else return pData->ir.valid;
}

}