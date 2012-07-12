// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREINPUTDEVICE_H
#define WIREINPUTDEVICE_H

#include "WireObject.h"
#include "WireInputDataBuffer.h"
#include <vector>
#include <map>

namespace Wire
{

class InputCapability;

class InputDevice : public Object
{
	WIRE_DECLARE_RTTI;

public:
	InputDevice();
	virtual ~InputDevice();

	Bool HasCapability(const Rtti& pCapabilityType) const;
	const InputDataBuffer* GetDataBuffer() const;
	void SetDataBuffer(const InputDataBuffer* pInputData);
	const std::vector<const InputCapability*>& GetCapabilities() const;
	const InputCapability* GetCapability(const Rtti& pCapabilityType) const;
	virtual void SetUp() = 0;

protected:
	std::map<const Rtti*, InputCapability*> mCapabilitiesByType;
	std::vector<const InputCapability*> mReadOnlyCapabilities;

	void RegisterCapability(InputCapability* pInputCapability);

private:
	const InputDataBuffer* mpDataBuffer;

};

}

#endif
