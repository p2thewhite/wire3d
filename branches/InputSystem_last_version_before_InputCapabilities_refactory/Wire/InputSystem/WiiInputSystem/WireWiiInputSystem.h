#ifndef WIREWIIINPUTSYSTEM_H_
#define WIREWIIINPUTSYSTEM_H_

#include "WireInputSystem.h"
#include "WireTypes.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiInputSystem: public InputSystem
{
public:
	WiiInputSystem(Float screenWidth, Float screenHeight);
	virtual ~WiiInputSystem();

	virtual void Capture();
protected:
	virtual void AfterInputDevicesDiscovery();
private:
	static UInt mEventCounter;

	Float mScreenWidth;
	Float mScreenHeight;

	static void ReadWPADPendingEventsCallback(int channel, const WPADData* data);
};

}

#endif
