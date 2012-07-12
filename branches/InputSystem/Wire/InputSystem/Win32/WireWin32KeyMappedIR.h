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

class Win32KeyMappedIR : public IR
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedIR(const InputDevice* pParent);
	virtual ~Win32KeyMappedIR();

	virtual Float GetBackward() const;
	virtual Float GetDown() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;
	virtual Float GetUp() const;

};

}

#endif
