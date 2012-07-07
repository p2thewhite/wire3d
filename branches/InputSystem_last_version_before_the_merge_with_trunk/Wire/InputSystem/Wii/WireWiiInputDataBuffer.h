#ifndef WIIINPUTDATABUFFER_H_
#define WIIINPUTDATABUFFER_H_

#include "WireInputDataBuffer.h"
#include "WireTypes.h"
#include <map>
#include <wiiuse/wpad.h>

namespace Wire
{

class WiiInputDataBuffer : public InputDataBuffer
{
public:
	WiiInputDataBuffer();
	virtual ~WiiInputDataBuffer();

	void SetData(WPADData* pData);
	const WPADData* GetData() const;
private:
	WPADData* mpData;
};

}

#endif