// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIINPUTSYSTEM_H
#define WIREWIIINPUTSYSTEM_H

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

#endif
