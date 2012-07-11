// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIMOTEIR_H
#define WIREWIIMOTEIR_H

#include "WireIR.h"
#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiMoteIR : public IR
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteIR(const InputDevice* pParent);
	virtual ~WiiMoteIR();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

private:
	Bool IsValid(const WPADData* pData) const;

};


}

#endif
