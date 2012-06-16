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

Int InputStateBuffer::GetMouseX() const
{
	return mMouseX;
}

Int InputStateBuffer::GetMouseY() const
{
	return mMouseY;
}

Int InputStateBuffer::GetMouseWheel() const
{
	return mMouseWheel;
}

Bool InputStateBuffer::GetKeyDown(UInt key) const
{
	return mpKeyBuffer[key];
}

void InputStateBuffer::SetMouseX(Int mouseX)
{
	mMouseX = mouseX;
}

void InputStateBuffer::SetMouseY(Int mouseY)
{
	mMouseY = mouseY;
}

void InputStateBuffer::IncrementMouseWheelByDelta(Int delta)
{
	mMouseWheel += delta;
}

void InputStateBuffer::SetKeyDown(UInt key)
{
	mpKeyBuffer[key] = true;
}

void InputStateBuffer::SetKeyUp(UInt key)
{
	mpKeyBuffer[key] = false;
}

void InputStateBuffer::CopyFrom(const InputStateBuffer* other)
{
	mMouseX = other->mMouseX;
	mMouseY = other->mMouseY;
	mMouseWheel = other->mMouseWheel;

	System::Memcpy(mpKeyBuffer, NUMBER_OF_VIRTUAL_KEYS, other->mpKeyBuffer, NUMBER_OF_VIRTUAL_KEYS);
}

}
