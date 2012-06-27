#include "WireInputStateBuffer.h"
#include "WireSystem.h"

namespace Wire
{

const UInt InputStateBuffer::NUMBER_OF_VIRTUAL_KEYS = 254;

InputStateBuffer::InputStateBuffer() :
	mMouseX(0), mMouseY(0), mMouseWheel(0)
{
	mpKeyBuffer = new Bool[NUMBER_OF_VIRTUAL_KEYS];
	System::Memset(mpKeyBuffer, 0, NUMBER_OF_VIRTUAL_KEYS);
}

InputStateBuffer::~InputStateBuffer()
{
}

void InputStateBuffer::CopyFrom(const InputStateBuffer* other)
{
	mMouseX = other->mMouseX;
	mMouseY = other->mMouseY;
	mMouseWheel = other->mMouseWheel;

	System::Memcpy(mpKeyBuffer, NUMBER_OF_VIRTUAL_KEYS, other->mpKeyBuffer, NUMBER_OF_VIRTUAL_KEYS);
}

}
