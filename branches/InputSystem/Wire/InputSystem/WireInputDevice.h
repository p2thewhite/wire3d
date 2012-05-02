#ifndef WIREINPUTDEVICE_H_
#define WIREINPUTDEVICE_H_

#include "WireObject.h"
#include "WireTypes.h"
#include "WireButton.h"
#include "WireAxis.h"
#include "WireInputCapability.h"
#include "WirePlatformKeyMapper.h"
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

namespace Wire
{

class InputDevice: public Object
{
WIRE_DECLARE_RTTI
	;

public:
	virtual ~InputDevice();

	virtual const char* GetName() const = 0;
	virtual Bool GetButton(Button button) const = 0;
	virtual Float GetIRAxis(IRAxis axis) const = 0;
	virtual Bool GetDigitalAxis(DigitalAxis axis) const = 0;
	virtual Float GetAnalogAxis(AnalogAxis axis) const = 0;

	inline const set<InputCapability>& GetCapabilities() const;
	inline Bool HasCapability(InputCapability capability) const;
protected:
	InputDevice(const PlatformKeyMapper* pPlatformKeyMapper);

	set<InputCapability> mCapabilities;
	const PlatformKeyMapper* mpPlatformKeyMapper;
};

#include "WireInputDevice.inl"

}

#endif
