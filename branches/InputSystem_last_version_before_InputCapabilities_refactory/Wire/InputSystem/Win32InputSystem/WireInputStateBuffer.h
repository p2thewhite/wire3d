#ifndef WIREINPUTSTATEBUFFER_H_
#define WIREINPUTSTATEBUFFER_H_

#include "WireTypes.h"

namespace Wire
{

class InputStateBuffer
{
public:
	InputStateBuffer();
	virtual ~InputStateBuffer();

	inline void SetMouseX(Int mouseX);
	inline void SetMouseY(Int mouseY);
	inline void IncrementMouseWheelByDelta(Int delta);
	inline void SetKeyDown(UInt key);
	inline void SetKeyUp(UInt key);
	inline Int GetMouseX() const;
	inline Int GetMouseY() const;
	inline Int GetMouseWheel() const;
	inline Bool GetKeyDown(UInt key) const;

	void CopyFrom(const InputStateBuffer* other);
private:
	static const UInt NUMBER_OF_VIRTUAL_KEYS;

	Int mMouseX;
	Int mMouseY;
	Int mMouseWheel;
	Bool* mpKeyBuffer;
};

#include "WireInputStateBuffer.inl"

}

#endif
