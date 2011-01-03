#pragma once
#ifndef WIREBUFFER_H
#define WIREBUFFER_H

#include "WireObject.h"

namespace Wire
{

class Buffer : public Object
{
	WIRE_DECLARE_RTTI;

public:

    // Usage flags for vertex buffers, index buffers and textures.
	enum UsageType
	{
		UT_STATIC,
		UT_DYNAMIC,
		UT_QUANTITY
	};

	// Locking modes for accessing video memory buffers.
	enum LockingMode
	{
		LM_READ_ONLY,
		LM_WRITE_ONLY,
		LM_READ_WRITE,
		LM_QUANTITY
	};

	inline UsageType GetUsage() const;

protected:
	Buffer(UsageType mUsage);
	virtual ~Buffer();

protected:
	UsageType mUsage;
};

typedef Pointer<Buffer> BufferPtr;

#include "WireBuffer.inl"

}

#endif
