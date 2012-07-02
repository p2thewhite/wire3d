#ifndef WIIMOTE_H_
#define WIIMOTE_H_

#include "WireMainInputDevice.h"
#include "WireTypes.h"

namespace Wire
{

class WiiMote : public MainInputDevice<WPADData>
{

public:
	WiiMote();
	virtual ~WiiMote();

	UInt GetChannel() const;

protected:
	virtual void SetUp();

private:
	UInt mChannel;

};

}

#endif // WIIMOTE_H_
