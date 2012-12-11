#include "WireWPADWrapper.h"

#include <wiiuse/wpad.h>

using namespace Wire;

void CopyToWrapperType(WPADWrapperData* pWPADWrapperData, WPADData* pWPADData)
{
}

void CopyToWrapperType(WPADWrapperExpansion* pWPADWrapperExpansion, expansion_t* pExpansion)
{
}

DefaultWPADWrapperImpl::DefaultWPADWrapperImpl()
{
}

DefaultWPADWrapperImpl::~DefaultWPADWrapperImpl()
{
}

void DefaultWPADWrapperImpl::Init()
{
	PAD_Init();
	WPAD_Init();
}

void DefaultWPADWrapperImpl::Shutdown()
{
	WPAD_Shutdown();
}

void DefaultWPADWrapperImpl::SetVRes(UInt channel, UInt xResolution, UInt yResolution)
{
	WPAD_SetVRes(channel, xResolution, yResolution);
}

WPADWrapperData* DefaultWPADWrapperImpl::Data(UInt channel)
{
	WPADWrapperData* pData = new WPADWrapperData();
	CopyToWrapperType(pData, WPAD_Data(channel));
	return pData;
}

void DefaultWPADWrapperImpl::SetDataFormat(UInt channel,  UInt format)
{
	WPAD_SetDataFormat(channel, format);
}

void DefaultWPADWrapperImpl::Expansion(UInt channel, WPADWrapperExpansion* expansion)
{
	expansion_t wpadTypeExpansion;
	WPAD_Expansion(channel, &wpadTypeExpansion);
	CopyToWrapperType(expansion, &wpadTypeExpansion);
}

Bool DefaultWPADWrapperImpl::Probe(UInt channel, UInt* type)
{
	return WPAD_Probe(channel, type);
}
