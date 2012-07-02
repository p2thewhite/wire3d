#ifndef WIIMOTETILT_H_
#define WIIMOTETILT_H_

#include "WireTilt.h"
#include "WireTypes.h"

namespace Wire
{

class WiiMoteTilt : public Tilt
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteTilt();
	virtual ~WiiMoteTilt();

	virtual Float GetBackward() const;
	virtual Float GetForward() const;
	virtual Float GetLeft() const;
	virtual Float GetRight() const;

};

}

#endif // WIIMOTETILT_H_
