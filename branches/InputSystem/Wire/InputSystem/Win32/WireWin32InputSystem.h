#ifndef WIN32INPUTSYSTEM_H_
#define WIN32INPUTSYSTEM_H_

#include "WireInputSystem.h"

namespace Wire
{

class Win32InputSystem : public InputSystem
{

public:
	Win32InputSystem();
	virtual ~Win32InputSystem();

	virtual void Capture();

};

}

#endif // WIN32INPUTSYSTEM_H_
