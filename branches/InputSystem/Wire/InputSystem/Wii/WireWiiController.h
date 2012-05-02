#ifndef WIREWIICONTROLLER_H_
#define WIREWIICONTROLLER_H_

#include "WireBaseVirtualInputDevice.h"
#include "WireWiiMote.h"
#include "WireWiiExtension.h"

namespace Wire
{

class WiiControllerDiscoveryStrategy;

class WiiController: public BaseVirtualInputDevice
{
public:
	WiiController(UInt channel);
	virtual ~WiiController();

	void SetIRCaptureArea(Float x, Float y, Float width, Float height);

	virtual void Capture();

	friend class WiiControllerDiscoveryStrategy;
private:
	static const UInt WIIMOTE_INDEX;
	static const UInt WIIEXTENSION_INDEX;

	UInt mChannel;

	inline void SetWiiMote(WiiMote* pWiiMote);
	inline void SetWiiExtension(WiiExtension* pWiiExtension);
	inline WiiMote* GetWiiMote();
	inline WiiExtension* GetWiiExtension();
	inline Bool HasWiiExtension();
};

#include "WireWiiController.inl"

}

#endif
