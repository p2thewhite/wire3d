#ifndef NUNCHUK_H_
#define NUNCHUK_H_

#include "WireInputDeviceExtension.h"

namespace Wire
{

class Nunchuk : public InputDeviceExtension
{

public:
	Nunchuk();
	virtual ~Nunchuk();

protected:
	virtual void SetUp();

};

}

#endif // NUNCHUK_H_
