#ifndef WIREVIRTUALINPUTDEVICE_H_
#define WIREVIRTUALINPUTDEVICE_H_

#include "WireTypes.h"
#include "WireButton.h"
#include "WireAxis.h"
#include "WireInputCapability.h"
//#include "WireTHashSet.h"
#include <set>

using namespace std;

namespace Wire
{

class VirtualInputDevice
{
public:
	virtual ~VirtualInputDevice();

	virtual void Capture() = 0;
	virtual const char* GetName() const = 0;
	virtual Bool GetButton(Button button) const = 0;
	virtual Float GetIRAxis(IRAxis axis) const = 0;
	virtual Float GetDigitalAxis(DigitalAxis axis) const = 0;
	virtual Float GetAnalogAxis(AnalogAxis axis) const = 0;
	virtual Bool HasCapability(InputCapability capability) const = 0;
	//virtual const THashSet<InputCapability>& GetCapabilities() const = 0;
	virtual const set<InputCapability>& GetCapabilities() const = 0;
protected:
	VirtualInputDevice();
};

}

#endif
