#pragma once
#ifndef WIREWIN32INPUTSYSTEMSTUB_H
#define WIREWIN32INPUTSYSTEMSTUB_H

#include <Win32/WireWin32InputSystem.h>

namespace Wire
{

class Win32InputSystemStub : public Win32InputSystem
{
public:
	Win32InputSystemStub();
	virtual ~Win32InputSystemStub();

	inline Win32InputDataBuffer* GetBackInputDataBuffer_TEST()
	{
		return mpBackInputDataBuffer;
	}

};

}

#endif