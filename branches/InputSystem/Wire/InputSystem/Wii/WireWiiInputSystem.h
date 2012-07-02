#ifndef WIIINPUTSYSTEM_H_
#define WIIINPUTSYSTEM_H_

#include "WireInputSystem.h"
#include "WireTypes.h"

namespace Wire
{

class WiiInputSystem : public InputSystem
{

public:
	WiiInputSystem();
	virtual ~WiiInputSystem();

	virtual void Capture();

protected:
	virtual void AfterInputDeviceDiscovery();

private:
	static UInt s_mEventCounter;

	static void ReadWPADPendingEventsCallback(Int channel, const WPADData* pData);

};

}

#endif // WIIINPUTSYSTEM_H_
