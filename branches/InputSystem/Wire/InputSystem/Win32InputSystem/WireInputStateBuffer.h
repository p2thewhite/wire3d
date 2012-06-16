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

	void SetMouseX(Int mouseX);
	void SetMouseY(Int mouseY);
	void IncrementMouseWheelByDelta(Int delta);
	void SetKeyDown(UInt key);
	void SetKeyUp(UInt key);

	Int GetMouseX() const;
	Int GetMouseY() const;
	Int GetMouseWheel() const;
	Bool GetKeyDown(UInt key) const;

	void CopyFrom(const InputStateBuffer* other);
private:
	static const UInt NUMBER_OF_VIRTUAL_KEYS;

	Int mMouseX;
	Int mMouseY;
	Int mMouseWheel;
	Bool* mpKeyBuffer;
};

}

#endif
