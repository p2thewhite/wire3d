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

class WiiInputDataBuffer : public InputDataBuffer
{
public:
	WiiInputDataBuffer();
	virtual ~WiiInputDataBuffer();

	void SetData(WPADWrapperData* pData);
	const WPADWrapperData* GetData() const;
private:
	WPADWrapperData* mpData;
};

}

#endif
