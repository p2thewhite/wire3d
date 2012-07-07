#ifndef INPUTSYSTEMLISTENER_H_
#define INPUTSYSTEMLISTENER_H_

namespace Wire
{

class InputSystemListener
{

public:
	InputSystemListener();
	virtual ~InputSystemListener();

	virtual void OnDevicesChange() = 0;

};

}

#endif // INPUTSYSTEMLISTENER_H_
