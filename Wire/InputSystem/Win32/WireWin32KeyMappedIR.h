// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32KEYMAPPEDIR_H
#define WIREWIN32KEYMAPPEDIR_H

#include "WireIR.h"

namespace Wire
{

/**
 * Win32KeyMappedIR enables the reading of an emulated WiiMote IR tracking mechanism.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and IR.
 **/
class Win32KeyMappedIR : public IR
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedIR(const InputDevice* pParent);
	virtual ~Win32KeyMappedIR();

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

};

}

#endif
