#ifndef WIIINPUTSYSTEM_H_
#define WIIINPUTSYSTEM_H_

#include "WireInputSystem.h"
#include "WireWiiInputDataBuffer.h"
#include "WireWiiMote.h"
#include "WireTypes.h"
#include <map>
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiInputSystem : public InputSystem
{

public:
	WiiInputSystem();
	virtual ~WiiInputSystem();

	virtual void Capture();

protected:
	virtual void DoDevicesDiscovery();
	virtual void AfterDevicesDiscovery();

private:
	static const UInt FIRST_CHANNEL;
	static const UInt LAST_CHANNEL;

	static UInt s_mEventCounter;

	std::map<Int, WiiInputDataBuffer*> mDataBufferByChannel;
	Int mLastDiscoveredChannel;
	Bool mChanged;

	void UpdateCurrentlyConnectedChannels();
	void ProbePreviouslyDisconnectedChannels();
	void DiscoverWiiMoteExpansions(WiiMote* pWiiMote);
	void NotifyDevicesChangeIfNecessary();
	Bool IsWiiMoteConnectedToChannel(UInt channel);
	WiiInputDataBuffer* GetChannelDataBuffer(UInt channel);

	static void ReadWPADPendingEventsCallback(Int channel, const WPADData* pData);

};

}

#endif // WIIINPUTSYSTEM_H_
