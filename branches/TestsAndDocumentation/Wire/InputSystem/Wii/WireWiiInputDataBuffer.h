// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIINPUTDATABUFFER_H
#define WIREWIIINPUTDATABUFFER_H

#include "WireInputDataBuffer.h"
#include "WireWPADWrapper.h"

namespace Wire
{

/**
 * WiiInputDataBuffer is the realization of the InputDataBuffer interface on the Wii platform.<br>
 * It only serves as a holder for the WPAD data of a given channel.
 *
 * See also: WiiInputSystem#Capture(), DefaultWPADWrapperImpl#GetData(UInt, WPADWrapperData*) and InputDataBuffer.
 **/
class WiiInputDataBuffer : public InputDataBuffer
{
public:
	WiiInputDataBuffer();
	virtual ~WiiInputDataBuffer();

	/// Returns the WPAD data associated to this input data buffer.
	WPADWrapperData* GetData();
	/// Returns the WPAD data (read-only) associated to this input data buffer.
	const WPADWrapperData* GetConstData() const;

private:
	/// The WPAD data associated to this input data buffer.
	WPADWrapperData pData;

};

}

#endif
