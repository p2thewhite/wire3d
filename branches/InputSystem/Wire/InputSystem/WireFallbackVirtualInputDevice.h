#ifndef WIREFALLBACKVIRTUALINPUTDEVICE_H_
#define WIREFALLBACKVIRTUALINPUTDEVICE_H_

#include "WireVirtualInputDevice.h"
#include <set>

using namespace std;

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
	virtual const set<InputCapability>& GetCapabilities() const;
private:
	set<InputCapability> mCapabilities;
};

}

#endif
