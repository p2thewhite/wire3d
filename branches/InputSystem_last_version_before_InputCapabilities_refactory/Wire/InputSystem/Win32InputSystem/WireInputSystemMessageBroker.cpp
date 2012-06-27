#include "WireInputSystemMessageBroker.h"
#include <Windows.h>
#include <WinUser.h>
#include <WindowsX.h>

namespace Wire
{

InputSystemMessageBroker* InputSystemMessageBroker::s_pInstance = NULL;

InputSystemMessageBroker::InputSystemMessageBroker()
{
	mpFrontBuffer = WIRE_NEW InputStateBuffer();
	mpBackBuffer = WIRE_NEW InputStateBuffer();
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
	Int mouseWheelDelta;

	switch (messageType)
	{
	case WM_MBUTTONDOWN:
		mpBackBuffer->SetKeyDown(VK_MBUTTON);
		return true;
	case WM_MBUTTONUP:
		mpBackBuffer->SetKeyUp(VK_MBUTTON);
		return true;
	case WM_RBUTTONDOWN:
		mpBackBuffer->SetKeyDown(VK_RBUTTON);
		return true;
	case WM_RBUTTONUP:
		mpBackBuffer->SetKeyUp(VK_RBUTTON);
		return true;
	case WM_LBUTTONDOWN:
		mpBackBuffer->SetKeyDown(VK_LBUTTON);
		return true;
	case WM_LBUTTONUP:
		mpBackBuffer->SetKeyUp(VK_LBUTTON);
		return true;
	case WM_MOUSEWHEEL:
		mouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wordParameter);
		mpBackBuffer->IncrementMouseWheelByDelta(mouseWheelDelta);
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
