#ifndef NUNCHUKTILT_H_
#define NUNCHUKTILT_H_

#include "WireTilt.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"

namespace Wire
{

class NunchukTilt : public Tilt
{
	WIRE_DECLARE_RTTI;

public:
	NunchukTilt(const InputDevice* pParent);
	virtual ~NunchukTilt();

	virtual Float GetBackward() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;

};

}

#endif // NUNCHUKTILT_H_
