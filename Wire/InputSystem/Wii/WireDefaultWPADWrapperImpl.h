#pragma once
#ifndef WIREDEFAULTWPADWRAPPERIMPL_H
#define WIREDEFAULTWPADWRAPPERIMPL_H

#include "WireWPADWrapper.h"

namespace Wire
{

class DefaultWPADWrapperImpl : public WPADWrapper
{
public:
	DefaultWPADWrapperImpl();
	virtual ~DefaultWPADWrapperImpl();

	virtual void Init();
	virtual void Shutdown();
	virtual void SetVRes(UInt channel, UInt xResolution, UInt yResolution);
	virtual void GetData(UInt channel, WPADWrapperData* pWPADWrapperData);
	virtual void SetDataFormat(UInt channel, UInt format);
	virtual void GetExpansion(UInt channel, WPADWrapperExpansion* expansion);
	virtual UInt Probe(UInt channel, UInt* type);

};

}

#endif