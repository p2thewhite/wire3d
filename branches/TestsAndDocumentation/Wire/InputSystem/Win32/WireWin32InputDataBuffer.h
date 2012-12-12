// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32INPUTDATABUFFER_H
#define WIREWIN32INPUTDATABUFFER_H

#include "WireInputDataBuffer.h"
#include "WireTypes.h"

namespace Wire
{

/**
 * Win32InputDataBuffer is the realization of the InputDataBuffer interface on the Windows platform.<br>
 * It stores data from keyboard and mouse read using the Win32 API (therefore the Win32 prefix).
 *
 * See also: Win32InputSystem#Capture() and Win32InputSystem#OnSystemMessage(UInt, UInt, Long).
 **/
class Win32InputDataBuffer : public InputDataBuffer
{

public:
	Win32InputDataBuffer();
	virtual ~Win32InputDataBuffer();

	/// Returns true if the keyboard key is down and false otherwise.
	Bool GetKey(UInt key) const;
	/// Returns the mouse X coordinate.
	Float GetMouseX() const;
	/// Returns the mouse Y coordinate.
	Float GetMouseY() const;
	/// Returns the mouse wheel button scroll position.
	Float GetMouseWheel() const;
	/// Sets the keyboard key to down. Used by the Win32InputSystem and should not be called directly by the user.
	void SetKeyDown(UInt key);
	/// Sets the keyboard key to up. Used by the Win32InputSystem and should not be called directly by the user.
	void SetKeyUp(UInt key);
	/// Sets the mouse X coordinate. Used by the Win32InputSystem and should not be called directly by the user.
	void SetMouseX(Float mouseX);
	/// Sets the mouse Y coordinate. Used by the Win32InputSystem and should not be called directly by the user.
	void SetMouseY(Float mouseY);
	/// Copies all input data from other win32 input data buffer to this win32 input data buffer. Used by the Win32InputSystem and should not be called directly by the user.
	void CopyFrom(const Win32InputDataBuffer* pOther);
	/// Increments the mouse wheel scroll position. Used by the Win32InputSystem and should not be called directly by the user.
	void IncrementMouseWheel(Float delta);
private:
	/// The mouse X coordinate.
	Float mMouseX;
	/// The mouse Y coordinate.
	Float mMouseY;
	/// The mouse wheel scroll position.
	Float mMouseWheel;
	/// The keyboard key states.
	Bool* mpKeys;

};

}

#endif
