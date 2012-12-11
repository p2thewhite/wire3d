#pragma once
#ifndef WIREWPADWRAPPERMOCK_H
#define WIREWPADWRAPPERMOCK_H

#include "WireWPADWrapper.h"

#include <gmock/gmock.h>

namespace Wire
{

class WPADWrapperMock : public WPADWrapper
{
public:
	WPADWrapperMock() {}
	virtual ~WPADWrapperMock() {}

	MOCK_METHOD0(Init, void());
	MOCK_METHOD0(Shutdown, void());
	MOCK_METHOD3(SetVRes, void(UInt channel, UInt xResolution, UInt yResolution));
	MOCK_METHOD2(GetData, void(UInt channel, WPADWrapperData* pWPADWrapperData));
	MOCK_METHOD2(SetDataFormat, void(UInt channel,  UInt format));
	MOCK_METHOD2(GetExpansion, void(UInt channel, WPADWrapperExpansion* expansion));
	MOCK_METHOD2(Probe, UInt(UInt channel, UInt* type));

};

}

#endif