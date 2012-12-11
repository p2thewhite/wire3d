#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WiiInputDataBuffer::WiiInputDataBuffer()
{
}

WiiInputDataBuffer::~WiiInputDataBuffer()
{
}

void WiiInputDataBuffer::SetData(WPADWrapperData* pData)
{
	mpData = pData;
}

const WPADWrapperData* WiiInputDataBuffer::GetData() const
{
	return mpData;
}

}
