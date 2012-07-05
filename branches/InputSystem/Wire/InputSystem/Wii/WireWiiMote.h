#ifndef WIIMOTE_H_
#define WIIMOTE_H_

#include "WireMainInputDevice.h"
#include "WireRtti.h"
#include "WireTypes.h"

namespace Wire
{

class WiiMote : public MainInputDevice
{
	WIRE_DECLARE_RTTI;

public:
	WiiMote(Int channel);
	virtual ~WiiMote();

	UInt GetChannel() const;
	virtual void SetUp();

private:
	UInt mChannel;

};

}

#endif // WIIMOTE_H_
