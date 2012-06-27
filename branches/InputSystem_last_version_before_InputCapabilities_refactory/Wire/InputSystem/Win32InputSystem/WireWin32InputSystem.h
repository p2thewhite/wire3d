#ifndef WIREWIN32INPUTSYSTEM_H_
#define WIREWIN32INPUTSYSTEM_H_

#include "WireInputSystem.h"
#include "WireTypes.h"

namespace Wire
{

class InputSystemMessageBroker;

class Win32InputSystem: public InputSystem
{
public:
	Win32InputSystem();
	virtual ~Win32InputSystem();

	virtual void Capture();
private:
	InputSystemMessageBroker* mpInputSystemMessageBroker;
};

}

#endif
