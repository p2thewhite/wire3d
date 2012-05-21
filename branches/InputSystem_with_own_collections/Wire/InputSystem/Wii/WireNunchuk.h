#ifndef WIRENUNCHUK_H_
#define WIRENUNCHUK_H_

#include "WireWiiExtension.h"
#include "WireTypes.h"
#include "WireButton.h"
#include "WireAxis.h"
#include <wiiuse/wpad.h>

using namespace std;

namespace Wire
{

class Nunchuk: public WiiExtension
{
	WIRE_DECLARE_RTTI;
public:
	Nunchuk(const PlatformKeyMapper* pPlatformKeyMapper);
	virtual ~Nunchuk();

	virtual void SetData(WPADData* data);
	virtual const char* GetName() const;
	virtual Bool GetButton(Button button) const;
	virtual Float GetIRAxis(IRAxis axis) const;
	virtual Bool GetDigitalAxis(DigitalAxis axis) const;
	virtual Float GetAnalogAxis(AnalogAxis axis) const;
private:
	WPADData* mpData;
};

}

#endif
