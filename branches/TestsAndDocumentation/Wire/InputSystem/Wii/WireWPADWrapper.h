#pragma once
#ifndef WIREWPADWRAPPER_H
#define WIREWPADWRAPPER_H

#include "WireTypes.h"

namespace Wire
{

enum WPADWrapperChannel
{
	WPAD_WRAPPER_CHAN_ALL = -1,
	WPAD_WRAPPER_CHAN_0,
	WPAD_WRAPPER_CHAN_1,
	WPAD_WRAPPER_CHAN_2,
	WPAD_WRAPPER_CHAN_3,
	WPAD_WRAPPER_BALANCE_BOARD,
	WPAD_WRAPPER_MAX_WIIMOTES,
};

enum WPADWrapperFormat
{
	WPAD_WRAPPER_FMT_BTNS = 0,
	WPAD_WRAPPER_FMT_BTNS_ACC,
	WPAD_WRAPPER_FMT_BTNS_ACC_IR
};

enum WPADWrapperExpansions
{
	WPAD_WRAPPER_EXP_NONE = 0,
	WPAD_WRAPPER_EXP_NUNCHUK
};

#define WPAD_WRAPPER_ERR_NONE							0
#define WPAD_WRAPPER_ERR								1

#define WPAD_WRAPPER_BUTTON_2							0x0001
#define WPAD_WRAPPER_BUTTON_1							0x0002
#define WPAD_WRAPPER_BUTTON_B							0x0004
#define WPAD_WRAPPER_BUTTON_A							0x0008
#define WPAD_WRAPPER_BUTTON_MINUS						0x0010
#define WPAD_WRAPPER_BUTTON_HOME						0x0080
#define WPAD_WRAPPER_BUTTON_LEFT						0x0100
#define WPAD_WRAPPER_BUTTON_RIGHT						0x0200
#define WPAD_WRAPPER_BUTTON_DOWN						0x0400
#define WPAD_WRAPPER_BUTTON_UP							0x0800
#define WPAD_WRAPPER_BUTTON_PLUS						0x1000
#define WPAD_WRAPPER_BUTTON_Z							(0x0001<<16)
#define WPAD_WRAPPER_BUTTON_C							(0x0002<<16)

typedef struct
{
	Float x;
	Float y;
	Float z;
	Int valid;
} WPADWrapperIR;

typedef struct
{
	UChar x;
	UChar y;
} WPADWrapperVec2b;

typedef struct
{
	WPADWrapperVec2b max;
	WPADWrapperVec2b min;
	WPADWrapperVec2b pos;
} WPADWrapperJoystick;

typedef struct
{
	Float roll;
	Float pitch;
} WPADWrapperOrient;

typedef struct
{
	WPADWrapperJoystick js;
} WPADWrapperNunchuk;

typedef struct
{
	int type;

	union
	{
		WPADWrapperNunchuk nunchuk;
	};
} WPADWrapperExpansion;

typedef struct
{
	UInt btns_h;
	UInt btns_l;
	UInt btns_d;
	UInt btns_u;
	WPADWrapperIR ir;
	WPADWrapperExpansion exp;
	WPADWrapperOrient orient;
} WPADWrapperData;

class WPADWrapper
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void SetVRes(UInt channel, UInt xResolution, UInt yResolution) = 0;
	virtual void GetData(UInt channel, WPADWrapperData* pWPADWrapperData) = 0;
	virtual void SetDataFormat(UInt channel,  UInt format) = 0;
	virtual void GetExpansion(UInt channel, WPADWrapperExpansion* expansion) = 0;
	virtual UInt Probe(UInt channel, UInt* type) = 0;

protected:
	WPADWrapper() {}
	virtual ~WPADWrapper() {}

};

}

#endif