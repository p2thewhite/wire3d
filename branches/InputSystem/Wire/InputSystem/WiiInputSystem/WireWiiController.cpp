#include "WireWiiController.h"
#include "WireSystem.h"
#include <wiiuse/wpad.h>

namespace Wire
{

const UInt WiiController::WIIMOTE_INDEX = 0;
const UInt WiiController::WIIEXTENSION_INDEX = 1;

WiiController::WiiController(UInt channel) :
		BaseVirtualInputDevice(),
		mChannel(channel)
{
	// Register expected data format on WPAD lib.
	WPAD_SetDataFormat(mChannel, WPAD_FMT_BTNS_ACC_IR);
}

WiiController::~WiiController()
{
}

void WiiController::Capture()
{
	WPADData* pData = WPAD_Data(mChannel);

	GetWiiMote()->SetData(pData);

	if (HasWiiExtension())
	{
		WiiExtension* pWiiExtension = GetWiiExtension();
		pWiiExtension->SetData(pData);
	}
}

void WiiController::SetIRCaptureArea(Float x, Float y, Float width, Float height)
{
	// TODO: What about the x and y offsets?
	WPAD_SetVRes(mChannel, width, height);
}

}
