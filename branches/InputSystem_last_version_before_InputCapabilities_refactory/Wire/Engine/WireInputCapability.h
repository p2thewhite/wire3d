#ifndef WIREINPUTCAPABILITY_H_
#define WIREINPUTCAPABILITY_H_

namespace Wire
{

enum InputCapability {
	BUTTONS,
	IR_AXIS,
	DIGITAL_AXIS,
	ANALOG_AXIS,
	IR_AXIS_ROTATION,
	UNKNOWN_CAPABILITY
};

const char* GetInputCapabilityName(InputCapability inputCapability);

}

#endif
