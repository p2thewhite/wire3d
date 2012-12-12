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

/**
 * WiiInputSystem is the specialization of the InputSystem on the Wii platform.
 * It handles all platform dependent operations for the input data capturing and device discovery.
 *
 * See also: InputSystem
 **/
class WiiInputSystem : public InputSystem
{

public:
	/// The index of the first controller channel.
	static const UInt FIRST_CHANNEL;
	/// The index of the last controller channel.
	static const UInt LAST_CHANNEL;

	WiiInputSystem();
	virtual ~WiiInputSystem();

	/// Sets the WPADWrapper implementation for all WiiInputSystem instances (usually only one) to use. Used mainly for mocking purposes.
	static void SetWPADWrapper(WPADWrapper* pWPADWrapper);

	/// Tries to discover all available input devices.
	virtual void Capture();
	/// Consolidates all input data at his disposal.
	virtual Bool DiscoverDevices();
	/// Returns true if a channel has an error-free connection and false otherwise.
	Bool IsChannelCorrectlyConnected(UInt channel);

private:
	/// The WPADWrapper implementation used by all WiiInputSystem instances (usually only one).
	static WPADWrapper* s_mpWPADWrapper;

	/// A table containing a WiiInputDataBuffer instance by channel.
	THashTable<Int, WiiInputDataBuffer*> mInputDataBufferByChannel;
	/// An array containing the status of the connection of all channels.
	Bool mChannelsConnectionStatus[MAXIMUM_NUMBER_OF_CHANNELS];
	Bool mChanged;

	/// Configures properly recently connected channels.
	void ConfigureConnectedChannels();
	/// Tries to discover all WiiMote expansions of the passed WiiMote.
	void DiscoverWiiMoteExpansions(WiiMote* pWiiMote);
	/// Returns a pointer to the WiiInputDataBuffer of the passed channel.
	WiiInputDataBuffer* GetChannelInputDataBuffer(UInt channel);
	/// Returns a pointer to the WiiMote connected to the passed channel.
	WiiMote* GetWiiMoteByChannel(UInt channel);
	/// Inspects each channel looking for changes in their connection state.
	Bool PollChannelsForChange();

};

}

#endif
