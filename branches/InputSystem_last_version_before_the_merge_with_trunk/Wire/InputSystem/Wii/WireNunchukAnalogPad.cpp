#include "WireNunchukAnalogPad.h"
#include "WireWiiInputDataBuffer.h"
#include <wiiuse/wpad.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(NunchukAnalogPad, AnalogPad);
	
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

}