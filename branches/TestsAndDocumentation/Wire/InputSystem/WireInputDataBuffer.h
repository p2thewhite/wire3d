// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREINPUTDATABUFFER_H
#define WIREINPUTDATABUFFER_H

namespace Wire
{

/**
 * InputDataBuffer is a <a href="http://en.wikipedia.org/wiki/Marker_interface_pattern">marker interface</a>.<br>
 * It must be realized in each supported platform.<br>
 * It holds temporary input device information (i.e: button states, motion data, etc.).
 *
 * See also: InputSystem#Capture(), InputDevice#GetInputDataBuffer(), InputDevice#SetInputDataBuffer(const InputDataBuffer*), Win32InputDataBuffer and WiiInputDataBuffer
 **/
class InputDataBuffer
{
protected:
	InputDataBuffer();
	virtual ~InputDataBuffer();

};

}

#endif
