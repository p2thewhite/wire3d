#ifndef INPUTCAPABILITY_H_
#define INPUTCAPABILITY_H_

#include "WireObject.h"
#include "WireInputDevice.h"

namespace Wire
{

class InputCapability : public Object
{
	WIRE_DECLARE_RTTI;

public:
	InputCapability(const InputDevice* pParent);
	virtual ~InputCapability();

protected:
	const InputDevice* GetParent() const;

private:
	const InputDevice* mpParent;

};

}

#endif // INPUTCAPABILITY_H_
