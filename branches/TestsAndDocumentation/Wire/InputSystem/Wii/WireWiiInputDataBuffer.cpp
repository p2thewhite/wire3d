#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WiiInputDataBuffer::WiiInputDataBuffer()
{
}

WiiInputDataBuffer::~WiiInputDataBuffer()
{
}

WPADWrapperData* WiiInputDataBuffer::GetData()
{
	return &pData;
}

const WPADWrapperData* WiiInputDataBuffer::GetConstData() const
{
	return &pData;
}


}
