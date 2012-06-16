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
