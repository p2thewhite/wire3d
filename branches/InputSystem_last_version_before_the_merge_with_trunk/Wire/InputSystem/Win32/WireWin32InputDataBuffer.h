#ifndef WIN32INPUTDATABUFFER_H_
#define WIN32INPUTDATABUFFER_H_

#include "WireInputDataBuffer.h"
#include "WireTypes.h"

namespace Wire
{

class Win32InputDataBuffer : public InputDataBuffer
{

public:
	Win32InputDataBuffer();
	virtual ~Win32InputDataBuffer();

	Bool GetKey(UInt key) const;
	Float GetMouseX() const;
	Float GetMouseY() const;
	Float GetMouseWheel() const;
	void SetKeyDown(UInt key);
	void SetKeyUp(UInt key);
	void SetMouseX(Float mouseX);
	void SetMouseY(Float mouseY);
	void CopyFrom(const Win32InputDataBuffer* pOther);
	void IncrementMouseWheel(Float delta);
private:
	Float mMouseX;
	Float mMouseY;
	Float mMouseWheel;
	Bool* mpKeys;

};

}

#endif // WIN32INPUTDATA_H_
