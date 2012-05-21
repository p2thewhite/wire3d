#include "WireUnsupportedInputCapabilityException.h"
#include <string>

using namespace std;

namespace Wire
{

UnsupportedInputCapabilityException::UnsupportedInputCapabilityException(InputCapability capability) :
		mCapability(capability)
{

}

const char* UnsupportedInputCapabilityException::what() const throw ()
{
	string exceptionMessage;

	exceptionMessage += "Unsupported input capability: ";

	switch (mCapability)
	{
	case BUTTONS:
		exceptionMessage += "BUTTONS";
		break;
	case IR_AXIS:
		exceptionMessage += "IR_AXIS";
		break;
	case DIGITAL_AXIS:
		exceptionMessage += "DIGITAL_AXIS";
		break;
	case ANALOG_AXIS:
		exceptionMessage += "ANALOG_AXIS";
		break;
	}

	return exceptionMessage.c_str();
}

}
