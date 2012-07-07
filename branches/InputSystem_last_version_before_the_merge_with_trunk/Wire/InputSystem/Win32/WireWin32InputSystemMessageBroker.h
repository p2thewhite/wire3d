#ifndef WIN32INPUTSYSTEMMESSAGEBROKER_H_
#define WIN32INPUTSYSTEMMESSAGEBROKER_H_

#include "WireWin32InputDataBuffer.h"
#include "WireTypes.h"

namespace Wire
{

class Win32InputSystemMessageBroker
{

public:
	Win32InputSystemMessageBroker();
	virtual ~Win32InputSystemMessageBroker();
	const Win32InputDataBuffer* GetCurrentInputDataBuffer() const;
	static Win32InputSystemMessageBroker* GetInstance();
	void SwapBuffers();
	Bool OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter);

private:
	static Win32InputSystemMessageBroker* s_pInstance;

	Win32InputDataBuffer* mpFrontBuffer;
	Win32InputDataBuffer* mpBackBuffer;
};

}

#endif // WIN32INPUTSYSTEMMESSAGEBROKER_H_
