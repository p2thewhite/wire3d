#ifndef WIREFALLBACKVIRTUALINPUTDEVICE_H_
#define WIREFALLBACKVIRTUALINPUTDEVICE_H_

#include "WireVirtualInputDevice.h"
#include "WireTHashSet.h"

namespace Wire
{

class FallbackVirtualInputDevice: public VirtualInputDevice
{
public:
	FallbackVirtualInputDevice();
	virtual ~FallbackVirtualInputDevice();

	virtual void Capture();
	virtual const char* GetName() const;
	virtual Bool GetButton(Button button) const;
	virtual Float GetIRAxis(IRAxis axis) const;
	virtual Float GetDigitalAxis(DigitalAxis axis) const;
	virtual Float GetAnalogAxis(AnalogAxis axis) const;
	virtual Bool HasCapability(InputCapability capability) const;
	virtual const THashSet<InputCapability>& GetCapabilities() const;
private:
	// An always empty set of capabilities.
	THashSet<InputCapability> mCapabilities;
};

}

#endif
