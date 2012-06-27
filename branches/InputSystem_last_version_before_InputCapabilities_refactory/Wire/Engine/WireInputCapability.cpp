#include "WireInputCapability.h"

namespace Wire
{

const char* GetInputCapabilityName(InputCapability inputCapability)
{
	switch (inputCapability)
	{
	case BUTTONS:
		return "BUTTONS";
	case IR_AXIS:
		return "IR_AXIS";
	case DIGITAL_AXIS:
		return "DIGITAL_AXIS";
	case ANALOG_AXIS:
		return "ANALOG_AXIS";
	case IR_AXIS_ROTATION:
		return "IR_AXIS_ROTATION";
	default:
		return "UKNOWN_CAPABILITY";
	}
}

}
