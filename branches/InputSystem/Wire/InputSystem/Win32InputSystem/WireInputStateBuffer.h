#ifndef WIREINPUTSTATEBUFFER_H_
#define WIREINPUTSTATEBUFFER_H_

#include "WireTypes.h"

namespace Wire
{

class InputStateBuffer
{
public:
	InputStateBuffer(UInt keyBufferSize);
	virtual ~InputStateBuffer();

	void SetMouseX(Int mouseX);
	void SetMouseY(Int mouseY);
	void SetLeftMouseButtonUp();
	void SetLeftMouseButtonDown();
	void SetRightMouseButtonUp();
	void SetRightMouseButtonDown();
	void SetKeyDown(UInt key);
	void SetKeyUp(UInt key);

	Int GetMouseX() const;
	Int GetMouseY() const;
	Bool GetLeftMouseButtonDown() const;
	Bool GetRightMouseButtonDown() const;
	Bool GetKeyDown(UInt key) const;

	void CopyFrom(const InputStateBuffer* other);
private:
	Int mMouseX;
	Int mMouseY;
	Bool mLeftMouseButton;
	Bool mRightMouseButton;
	Bool* mpKeyBuffer;
	UInt mkeyBufferSize;
};

}

#endif
