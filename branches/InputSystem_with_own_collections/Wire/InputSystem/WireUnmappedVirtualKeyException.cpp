#include "WireUnmappedVirtualKeyException.h"
#include <string>

using namespace std;

namespace Wire
{

UnmappedVirtualKeyException::UnmappedVirtualKeyException(Button button) :
		mpButton(&button),
		mpIRAxis(0),
		mpDigitalAxis(0),
		mpAnalogAxis(0)
{
}

UnmappedVirtualKeyException::UnmappedVirtualKeyException(IRAxis axis) :
		mpButton(0),
		mpIRAxis(&axis),
		mpDigitalAxis(0),
		mpAnalogAxis(0)
{
}

UnmappedVirtualKeyException::UnmappedVirtualKeyException(DigitalAxis axis) :
		mpButton(0),
		mpIRAxis(0),
		mpDigitalAxis(&axis),
		mpAnalogAxis(0)
{
}

UnmappedVirtualKeyException::UnmappedVirtualKeyException(AnalogAxis axis) :
		mpButton(0),
		mpIRAxis(0),
		mpDigitalAxis(0),
		mpAnalogAxis(&axis)
{
}

const char* UnmappedVirtualKeyException::what() const throw ()
{
	string exceptionMessage;

	exceptionMessage += "Unsupported virtual key: ";

	if (mpButton != NULL) exceptionMessage += GetButtonName(*mpButton);
	else if (mpIRAxis != NULL) exceptionMessage += GetIRAxisName(*mpIRAxis);
	else if (mpDigitalAxis != NULL) exceptionMessage += GetDigitalAxisName(*mpDigitalAxis);
	else if (mpAnalogAxis != NULL) exceptionMessage += GetAnalogAxisName(*mpAnalogAxis);

	return exceptionMessage.c_str();
}

}
