#ifndef WIREUNMAPPEDVIRTUALKEYEXCEPTION_H_
#define WIREUNMAPPEDVIRTUALKEYEXCEPTION_H_

#include "WireButton.h"
#include "WireAxis.h"
#include <exception>

using namespace std;

namespace Wire
{

class UnmappedVirtualKeyException: public exception
{
public:
	UnmappedVirtualKeyException(Button button);
	UnmappedVirtualKeyException(IRAxis axis);
	UnmappedVirtualKeyException(DigitalAxis axis);
	UnmappedVirtualKeyException(AnalogAxis axis);
	virtual const char* what() const throw();
private:
	Button* mpButton;
	IRAxis *mpIRAxis;
	DigitalAxis *mpDigitalAxis;
	AnalogAxis *mpAnalogAxis;
};

}

#endif
