#ifndef MAININPUTDEVICE_H_
#define MAININPUTDEVICE_H_

#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"
#include <vector>

namespace Wire
{

class InputDeviceExtension;

class MainInputDevice : public InputDevice
{
	WIRE_DECLARE_RTTI;

public:
	MainInputDevice();
	virtual ~MainInputDevice();

	void AddExtension(InputDevice* pInputDevice);
	const InputDevice* GetExtension(UInt index) const;
	UInt GetExtensionsCount() const;
	Bool HasCapability(const Rtti& pCapabilityType, Bool lookupExtensions) const;
	const InputCapability* GetCapability(const Rtti& pCapabilityType, Bool lookupExtensions) const;

private:
	std::vector<const InputDeviceExtension*> mExtensions;

};

}

#endif // MAININPUTDEVICE_H_
