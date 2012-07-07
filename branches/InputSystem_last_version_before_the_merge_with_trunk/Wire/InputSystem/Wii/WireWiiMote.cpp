#include "WireWiiMote.h"
#include "WireMemory.h"
#include "WireWiiMoteButtons.h"
#include "WireWiiMoteDigitalPad.h"
#include "WireWiiMoteIR.h"
#include "WireWiiMoteShake.h"
#include "WireWiiMoteSwing.h"
#include "WireWiiMoteTilt.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(WiiMote, MainInputDevice);

WiiMote::WiiMote(Int channel)
	: mChannel(channel)
{
}

WiiMote::~WiiMote()
{
}

UInt WiiMote::GetChannel() const
{
	return mChannel;
}

void WiiMote::SetUp()
{
	RegisterCapability(WIRE_NEW WiiMoteButtons(this));
	RegisterCapability(WIRE_NEW WiiMoteDigitalPad(this));
	RegisterCapability(WIRE_NEW WiiMoteIR(this));
	RegisterCapability(WIRE_NEW WiiMoteShake(this));
	RegisterCapability(WIRE_NEW WiiMoteSwing(this));
	RegisterCapability(WIRE_NEW WiiMoteTilt(this));
}

}