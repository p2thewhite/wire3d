#include "WireInputStateBuffer.h"
#include "WireSystem.h"

namespace Wire
{

InputStateBuffer::InputStateBuffer(UInt keyBufferSize) : mkeyBufferSize(keyBufferSize),
	mMouseX(0), mMouseY(0), mLeftMouseButton(false), mRightMouseButton(false)
{
	mpKeyBuffer = new Bool[mkeyBufferSize];
	System::Memset(mpKeyBuffer, 0, 255);
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

Bool InputStateBuffer::GetLeftMouseButtonDown() const
{
	return mLeftMouseButton;
}

Bool InputStateBuffer::GetRightMouseButtonDown() const
{
	return mRightMouseButton;
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

void InputStateBuffer::SetLeftMouseButtonUp()
{
	mLeftMouseButton = false;
}

void InputStateBuffer::SetLeftMouseButtonDown()
{
	mLeftMouseButton = true;
}

void InputStateBuffer::SetRightMouseButtonUp()
{
	mRightMouseButton = false;
}

void InputStateBuffer::SetRightMouseButtonDown()
{
	mRightMouseButton = true;
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
	mLeftMouseButton = other->mLeftMouseButton;
	mRightMouseButton = other->mRightMouseButton;

	System::Memcpy(mpKeyBuffer, 255, other->mpKeyBuffer, 255);
}

}
