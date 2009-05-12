#include "WireVertexBuffer.h"

#include "../Foundation/WireFoundation.h"

using namespace Wire;

//-------------------------------------------------------------------------
VertexBuffer::VertexBuffer(UInt vertexQuantity)
	:
	mVertexSize(0),
	mVertexQuantity(vertexQuantity),
	mpChannel(NULL)
{
	mVertexSize = sizeof(Vector3f);
	mpChannel = WIRE_NEW Float[mVertexQuantity * 3];
}

//-------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	WIRE_DELETE[] mpChannel;
}

//-------------------------------------------------------------------------
Vector3f& VertexBuffer::Position3(Int i)
{
	Float* pChannel = mpChannel + mVertexSize/sizeof(Float) * i;
	return *(reinterpret_cast<Vector3f*>(pChannel));
}

//-------------------------------------------------------------------------
Vector3f VertexBuffer::Position3(Int i) const
{
	const Float* pChannel = mpChannel + mVertexSize/sizeof(Float) * i;
	return *(reinterpret_cast<Vector3f*>(const_cast<Float*>(pChannel)));
}
