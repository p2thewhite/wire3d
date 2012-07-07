#include "WireNunchuk.h"
#include "WireMemory.h"
#include "WireNunchukAnalogPad.h"
#include "WireNunchukButtons.h"
#include "WireNunchukShake.h"
#include "WireNunchukSwing.h"
#include "WireNunchukTilt.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Nunchuk, InputDeviceExtension);

Nunchuk::Nunchuk(const MainInputDevice* pMainInputDevice)
	: InputDeviceExtension(pMainInputDevice)
{
}

Nunchuk::~Nunchuk()
{
}

void Nunchuk::SetUp()
{
	RegisterCapability(WIRE_NEW NunchukAnalogPad(this));
	RegisterCapability(WIRE_NEW NunchukButtons(this));
	RegisterCapability(WIRE_NEW NunchukShake(this));
	RegisterCapability(WIRE_NEW NunchukSwing(this));
	RegisterCapability(WIRE_NEW NunchukTilt(this));
}

}