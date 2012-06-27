#ifndef WIREINPUTSYSTEMMESSAGEBROKER_H_
#define WIREINPUTSYSTEMMESSAGEBROKER_H_

#include "WireInputStateBuffer.h"
#include "WireMemory.h"
#include "WireTypes.h"

namespace Wire 
{

class InputSystemMessageBroker
{
public:
	virtual ~InputSystemMessageBroker();

	Bool OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter);
	const InputStateBuffer* GetCurrentInputStateBufferAndSwap();

	static InputSystemMessageBroker* GetInstance(); 
private:
	InputSystemMessageBroker();

	// Singleton.
	static InputSystemMessageBroker* s_pInstance;

	InputStateBuffer* mpFrontBuffer;
	InputStateBuffer* mpBackBuffer;
};

}

#endif
