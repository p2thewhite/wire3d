#include "WireWiiMoteDigitalPad.h"
#include "WireWiiMote.h"
#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(WiiMoteDigitalPad, DigitalPad);

WiiMoteDigitalPad::WiiMoteDigitalPad(const InputDevice* pParent)
	: DigitalPad(pParent)
{
}

WiiMoteDigitalPad::~WiiMoteDigitalPad()
{
}

Bool WiiMoteDigitalPad::GetDown() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return false;
	}

	return (pData->btns_h & WPAD_BUTTON_DOWN) != 0;
}

Bool WiiMoteDigitalPad::GetLeft() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return false;
	}

	return (pData->btns_h & WPAD_BUTTON_LEFT) != 0;
}

Bool WiiMoteDigitalPad::GetRight() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return false;
	}

	return (pData->btns_h & WPAD_BUTTON_RIGHT) != 0;
}

Bool WiiMoteDigitalPad::GetUp() const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return false;
	}

	return (pData->btns_h & WPAD_BUTTON_UP) != 0;
}

}