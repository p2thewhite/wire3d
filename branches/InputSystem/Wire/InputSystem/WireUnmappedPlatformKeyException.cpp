#include "WireUnmappedPlatformKeyException.h"
#include <string>

using namespace std;

namespace Wire
{

UnmappedPlatformKeyException::UnmappedPlatformKeyException(UInt key) :
		mKey(key)
{
}

const char* UnmappedPlatformKeyException::what() const throw()
{
	string exceptionMessage;

	exceptionMessage += "Unmapped platform key: ";
	exceptionMessage += mKey;

	return exceptionMessage.c_str();
}

}
