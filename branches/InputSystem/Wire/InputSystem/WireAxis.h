#ifndef WIREAXIS_H_
#define WIREAXIS_H_

namespace Wire
{

enum IRAxis
{
	IR_X,
	IR_Y,
	IR_Z,
	INVALID_IR
};

enum DigitalAxis
{
	DIGITAL_LEFT,
	DIGITAL_RIGHT,
	DIGITAL_UP,
	DIGITAL_DOWN,
	INVALID_DIGITAL
};

enum AnalogAxis
{
	ANALOG_X,
	ANALOG_Y,
	INVALID_ANALOG
};

const char* GetIRAxisName(IRAxis axis);
const char* GetDigitalAxisName(DigitalAxis axis);
const char* GetAnalogAxisName(AnalogAxis axis);

}

#endif
