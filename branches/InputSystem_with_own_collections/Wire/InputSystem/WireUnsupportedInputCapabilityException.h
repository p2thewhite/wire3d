#ifndef WIREUNSUPPORTEDINPUTCAPABILITYEXCEPTION_H_
#define WIREUNSUPPORTEDINPUTCAPABILITYEXCEPTION_H_

#include <exception>
#include "WireInputCapability.h"

using namespace std;

namespace Wire
{

class UnsupportedInputCapabilityException: public exception
{
public:
	UnsupportedInputCapabilityException(InputCapability capability);
	virtual const char* what() const throw();
private:
	InputCapability mCapability;
};

}

#endif
