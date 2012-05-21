#ifndef WIREWIIMOTE_H_
#define WIREWIIMOTE_H_

#include "WireInputDevice.h"
#include "WireTypes.h"
#include "WireButton.h"
#include "WireAxis.h"
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiMote: public InputDevice
{
	WIRE_DECLARE_RTTI;
public:
	WiiMote(const PlatformKeyMapper* pPlatformKeyMapper);
	virtual ~WiiMote();

	virtual const char* GetName() const;
	virtual void SetData(WPADData* pData);
	virtual Bool GetButton(Button button) const;
	virtual Float GetIRAxis(IRAxis axis) const;
	virtual Bool GetDigitalAxis(DigitalAxis axis) const;
	virtual Float GetAnalogAxis(AnalogAxis axis) const;
private:
	WPADData* mpData;
};

}

#endif
