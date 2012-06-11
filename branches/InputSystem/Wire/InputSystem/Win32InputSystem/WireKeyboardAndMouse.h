#ifndef WIREKEYBOARDANDMOUSE_H_
#define WIREKEYBOARDANDMOUSE_H_

#include "WireVirtualInputDevice.h"
#include "WireInputSystemMessageBroker.h"
#include "WireInputStateBuffer.h"
#include "WirePlatformKeyMapper.h"
#include "WireAxis.h"
#include "WireButton.h"
#include "WireInputCapability.h"
#include <set>

using namespace std;

namespace Wire
{

class KeyboardAndMouse: public VirtualInputDevice
{
public:
	KeyboardAndMouse(const PlatformKeyMapper* pPlatformKeyMapper);
	virtual ~KeyboardAndMouse();

	virtual void Capture();
	virtual const char* GetName() const;
	virtual Bool GetButton(Button button) const;
	virtual Float GetIRAxis(IRAxis axis) const;
	virtual Float GetDigitalAxis(DigitalAxis axis) const;
	virtual Float GetAnalogAxis(AnalogAxis axis) const;
	virtual Bool HasCapability(InputCapability capability) const;
	virtual const set<InputCapability>& GetCapabilities() const;

	void SetCurrentInputStateBuffer(const InputStateBuffer* pCurrentInputStateBuffer);
private:
	const PlatformKeyMapper* mpPlatformKeyMapper;
	const InputStateBuffer* mpCurrentInputStateBuffer;
	set<InputCapability> mCapabilities;
};

}

#endif
