#ifndef WIREWIIEXTENSION_H_
#define WIREWIIEXTENSION_H_

#include "WireInputDevice.h"
#include "WireTypes.h"
#include "WireWiiMote.h"
#include "WirePlatformKeyMapper.h"

namespace Wire
{

class WiiExtension: public InputDevice
{
public:
	virtual ~WiiExtension();

	virtual void SetData(WPADData* pData) = 0;
protected:
	WiiExtension(const PlatformKeyMapper* pPlatformKeyMapper);
};

}

#endif
