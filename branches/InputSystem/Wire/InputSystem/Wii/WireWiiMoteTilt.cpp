#include "WireWiiMoteTilt.h"
#include "WireWiiMoteButtons.h"
#include "WireWiiMote.h"
#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(WiiMoteTilt, Tilt);
	
WiiMoteTilt::WiiMoteTilt(const InputDevice* pParent)
	: Tilt(pParent)
{
}

WiiMoteTilt::~WiiMoteTilt()
{
}

Float WiiMoteTilt::GetBackward() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return pData->orient.pitch;
}

Float WiiMoteTilt::GetForward() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return -pData->orient.pitch;
}

Float WiiMoteTilt::GetLeft() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return pData->orient.roll;
}

Float WiiMoteTilt::GetRight() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return 0;
	}

	return -pData->orient.roll;
}

}