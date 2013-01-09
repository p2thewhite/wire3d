// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32KEYMAPPEDSHAKE_H
#define WIREWIN32KEYMAPPEDSHAKE_H

#include "WireShake.h"

namespace Wire
{

class Win32KeyMappedShake : public Shake
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedShake(const InputDevice* pParent);
	virtual ~Win32KeyMappedShake();

	virtual Float GetX() const;
	virtual Float GetY() const;
	virtual Float GetZ() const;

};

}

#endif
