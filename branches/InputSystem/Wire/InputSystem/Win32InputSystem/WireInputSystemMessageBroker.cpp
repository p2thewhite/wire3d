#include "WireInputSystemMessageBroker.h"
#include <Windows.h>
#include <WinUser.h>
#include <WindowsX.h>

namespace Wire
{

InputSystemMessageBroker* InputSystemMessageBroker::s_pInstance = NULL;

InputSystemMessageBroker::InputSystemMessageBroker()
{
	// TODO: MAGIC NUMBERS: Should be something like MAX_WIN_32_VIRTUAL_KEY...
	mpFrontBuffer = WIRE_NEW InputStateBuffer(255);
	mpBackBuffer = WIRE_NEW InputStateBuffer(255);
}

InputSystemMessageBroker::~InputSystemMessageBroker()
{
	WIRE_DELETE mpFrontBuffer;
	WIRE_DELETE mpBackBuffer;
}

InputSystemMessageBroker* InputSystemMessageBroker::GetInstance()
{
	if (s_pInstance == NULL) {
		s_pInstance = WIRE_NEW InputSystemMessageBroker();
	}

	return s_pInstance;
}

Bool InputSystemMessageBroker::OnSystemMessage(UInt messageType, UInt wordParameter, Long longParameter)
{
	Int x; 
	Int y;

	switch (messageType)
	{
	case WM_RBUTTONDOWN:
		mpBackBuffer->SetRightMouseButtonDown();
		return true;
	case WM_RBUTTONUP:
		mpBackBuffer->SetRightMouseButtonUp();
		return true;
	case WM_LBUTTONDOWN:
		mpBackBuffer->SetLeftMouseButtonDown();
		return true;
	case WM_LBUTTONUP:
		mpBackBuffer->SetLeftMouseButtonUp();
		return true;
	case WM_MOUSEMOVE:
		x = GET_X_LPARAM(longParameter); 
		y = GET_Y_LPARAM(longParameter);

		mpBackBuffer->SetMouseX(x);
		mpBackBuffer->SetMouseY(y);

		return true;
	case WM_KEYDOWN:
		mpBackBuffer->SetKeyDown((UInt) wordParameter);
		return true;
	case WM_KEYUP:
		mpBackBuffer->SetKeyUp((UInt) wordParameter);
		return true;
	}

	return false;
}

/**
 * FIXME: Needs thread safety!!!
 */
const InputStateBuffer* InputSystemMessageBroker::GetCurrentInputStateBufferAndSwap()
{
	InputStateBuffer* pInputStateBuffer = mpFrontBuffer;

	mpFrontBuffer = mpBackBuffer;
	mpBackBuffer = pInputStateBuffer;

	mpBackBuffer->CopyFrom(mpFrontBuffer);

	return mpFrontBuffer;
}

}
