#include "WireBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Buffer, Object);

//----------------------------------------------------------------------------
Buffer::Buffer(UsageType usage)
	:
	mUsage(usage)
{
}

//----------------------------------------------------------------------------
Buffer::~Buffer()
{
}
