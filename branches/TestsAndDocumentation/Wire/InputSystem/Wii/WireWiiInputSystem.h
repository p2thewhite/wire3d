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
#include "WireWPADWrapper.h"
#include "WireTHashTable.h"

#define MAXIMUM_NUMBER_OF_CHANNELS 4

namespace Wire
{

class WiiInputSystem : public InputSystem
{

public:
	WiiInputSystem();
	virtual ~WiiInputSystem();

	static void SetWPADWrapper(WPADWrapper* pWPADWrapper);

	virtual void Capture();
	virtual Bool DiscoverDevices();

private:
	static const UInt FIRST_CHANNEL;
	static const UInt LAST_CHANNEL;

	static UInt s_mEventCounter;
	static WPADWrapper* s_mpWPADWrapper;

	static void ReadWPADPendingEventsCallback(Int channel, const WPADData* pData);

	THashTable<Int, WiiInputDataBuffer*> mInputDataBufferByChannel;
	Bool mChannelsConnectionStatus[MAXIMUM_NUMBER_OF_CHANNELS];
	Bool mChanged;

	void ConfigureConnectedChannels();
	void DiscoverWiiMoteExpansions(WiiMote* pWiiMote);
	WiiInputDataBuffer* GetChannelInputDataBuffer(UInt channel);
	WiiMote* GetWiiMoteByChannel(UInt channel);
	Bool IsChannelCorrectlyConnected(UInt channel);
	Bool PollChannelsForChange();

};

}

#endif
