#ifndef WIREUNMAPPEDPLATFORMKEYEXCEPTION_H_
#define WIREUNMAPPEDPLATFORMKEYEXCEPTION_H_

#include "WireTypes.h"
#include <exception>

using namespace std;

namespace Wire
{

class UnmappedPlatformKeyException: public exception
{
public:
	UnmappedPlatformKeyException(UInt key);
	virtual const char* what() const throw();
private:
	UInt mKey;
};

}

#endif
