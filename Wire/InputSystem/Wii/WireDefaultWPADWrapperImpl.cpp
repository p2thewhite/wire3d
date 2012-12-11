#include "WireDefaultWPADWrapperImpl.h"

#include <wiiuse/wpad.h>

using namespace Wire;

void CopyToWrapperType(WPADWrapperIR* pWPADWrapperIR, ir_t* pWPADIR)
{
	pWPADWrapperIR->x = pWPADIR->x;
	pWPADWrapperIR->y = pWPADIR->y;
	pWPADWrapperIR->z = pWPADIR->z;
	pWPADWrapperIR->valid = pWPADIR->valid;
}

void CopyToWrapperType(WPADWrapperVec2b* pWPADWrapperVec2b, vec2b_t* pWPADVec2b)
{
	pWPADWrapperVec2b->x = pWPADVec2b->x;
	pWPADWrapperVec2b->y = pWPADVec2b->y;
}

void CopyToWrapperType(WPADWrapperJoystick* pWPADWrapperJoystick, joystick_t* pWPADJoystick)
{
	CopyToWrapperType(&pWPADWrapperJoystick->max, &pWPADJoystick->max);
	CopyToWrapperType(&pWPADWrapperJoystick->min, &pWPADJoystick->min);
	CopyToWrapperType(&pWPADWrapperJoystick->pos, &pWPADJoystick->pos);
}

void CopyToWrapperType(WPADWrapperExpansion* pWPADWrapperExpansion, expansion_t* pWPADExpansion)
{
	pWPADWrapperExpansion->type = pWPADExpansion->type;
	CopyToWrapperType(&pWPADWrapperExpansion->nunchuk.js, &pWPADExpansion->nunchuk.js);
}

void CopyToWrapperType(WPADWrapperOrient* pWPADWrapperOrient, orient_t* pWPADOrient)
{
	pWPADWrapperOrient->roll = pWPADOrient->roll;
	pWPADWrapperOrient->pitch = pWPADOrient->pitch;
}

void CopyToWrapperType(WPADWrapperData* pWPADWrapperData, WPADData* pWPADData)
{
	pWPADWrapperData->btns_h = pWPADData->btns_h;
	pWPADWrapperData->btns_l = pWPADData->btns_l;
	pWPADWrapperData->btns_d = pWPADData->btns_d;
	pWPADWrapperData->btns_u = pWPADData->btns_u;
	CopyToWrapperType(&pWPADWrapperData->ir, &pWPADData->ir);
	CopyToWrapperType(&pWPADWrapperData->exp, &pWPADData->exp);
	CopyToWrapperType(&pWPADWrapperData->orient, &pWPADData->orient);
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

void DefaultWPADWrapperImpl::GetData(UInt channel, WPADWrapperData* pWPADWrapperData)
{
	CopyToWrapperType(pWPADWrapperData, WPAD_Data(channel));
}

void DefaultWPADWrapperImpl::SetDataFormat(UInt channel, UInt format)
{
	WPAD_SetDataFormat(channel, format);
}

void DefaultWPADWrapperImpl::GetExpansion(UInt channel, WPADWrapperExpansion* expansion)
{
	expansion_t wpadTypeExpansion;
	WPAD_Expansion(channel, &wpadTypeExpansion);
	CopyToWrapperType(expansion, &wpadTypeExpansion);
}

UInt DefaultWPADWrapperImpl::Probe(UInt channel, UInt* type)
{
	return WPAD_Probe(channel, type);
}
