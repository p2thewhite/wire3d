#include "WireWin32InputDataBuffer.h"
#include "WireMemory.h"
#include "WireSystem.h"

namespace Wire
{

Win32InputDataBuffer::Win32InputDataBuffer() 
	: mMouseX(0), mMouseY(0)
{
	mpKeys = WIRE_NEW Bool[256];
	System::Memset(mpKeys, 0, 256);
}

Win32InputDataBuffer::~Win32InputDataBuffer()
{
	WIRE_DELETE mpKeys;
}

Bool Win32InputDataBuffer::GetKey(UInt key) const
{
	return mpKeys[key];
}

Float Win32InputDataBuffer::GetMouseX() const
{
	return mMouseX;
}

Float Win32InputDataBuffer::GetMouseY() const
{
	return mMouseY;
}

void Win32InputDataBuffer::SetKeyDown(UInt key)
{
	mpKeys[key] = true;
}

void Win32InputDataBuffer::SetKeyUp(UInt key)
{
	mpKeys[key] = false;
}

void Win32InputDataBuffer::SetMouseX(Float mouseX)
{
	mMouseX = mouseX;
}

void Win32InputDataBuffer::SetMouseY(Float mouseY)
{
	mMouseY = mouseY;
}

void Win32InputDataBuffer::CopyFrom(const Win32InputDataBuffer* pOther)
{
	mMouseX = pOther->mMouseX;
	mMouseY = pOther->mMouseY;

	System::Memcpy(mpKeys, 256, pOther->mpKeys, 256);
}

}