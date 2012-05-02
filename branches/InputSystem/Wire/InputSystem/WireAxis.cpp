#include "WireAxis.h"

namespace Wire
{

const char* GetIRAxisName(IRAxis axis)
{
	switch (axis)
	{
	case IR_X:
		return "IR_X";
	case IR_Y:
		return "IR_Y";
	case IR_Z:
		return "IR_Z";
	}

	return 0;
}

const char* GetDigitalAxisName(DigitalAxis axis)
{
	switch (axis)
	{
	case DIGITAL_LEFT:
		return "DIGITAL_LEFT";
	case DIGITAL_RIGHT:
		return "DIGITAL_RIGHT";
	case DIGITAL_UP:
		return "DIGITAL_UP";
	case DIGITAL_DOWN:
		return "DIGITAL_DOWN";
	}

	return 0;
}

const char* GetAnalogAxisName(AnalogAxis axis)
{
	switch (axis)
	{
	case ANALOG_X:
		return "ANALOG_X";
	case ANALOG_Y:
		return "ANALOG_Y";
	}

	return 0;
}

}
