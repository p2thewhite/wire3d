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
#include "WireWPADWrapper.h"

namespace Wire
{

/**
 * WiiMoteIR enables the reading of a WiiMote IR tracking mechanism.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and IR.
 **/
class WiiMoteIR : public IR
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteIR(const InputDevice* pParent);
	virtual ~WiiMoteIR();

	/// Returns the Z-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetBackward() const;
	/// Returns the Y-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetDown() const;
	/// Returns the Z-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetForward() const;
	/// Returns the X-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetLeft() const;
	/// Returns the X-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetRight() const;
	/// Returns the Y-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetUp() const;

private:
	Bool IsValid(const WPADWrapperData* pData) const;

};

}

#endif
