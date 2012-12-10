#pragma once
#ifndef WIREINPUTCAPABILITYSTUB_H
#define WIREINPUTCAPABILITYSTUB_H

#include "WireInputCapability.h"

namespace Wire
{

class InputCapabilityStub : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	InputCapabilityStub(const InputDevice* pParent)
		: InputCapability(pParent)
	{
	}

	virtual ~InputCapabilityStub()
	{
	}

	inline const InputDevice* GetParent_TEST()
	{
		return GetParent();
	}

};

}

#endif