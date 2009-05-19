#include "WireVertexBuffer.h"

using namespace Wire;

//-------------------------------------------------------------------------
VertexBuffer::VertexBuffer(const VertexAttributes& rAttributes,
	UInt vertexQuantity, Bool isInterleaved)
	:
	mAttributes(rAttributes),
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
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	Float* pChannel = mpChannel + mVertexSize/sizeof(Float) * i;
	return *(reinterpret_cast<Vector3f*>(pChannel));
}

//-------------------------------------------------------------------------
Vector3f VertexBuffer::Position3(Int i) const
{
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	const Float* pChannel = mpChannel + mVertexSize/sizeof(Float) * i;
	return *(reinterpret_cast<Vector3f*>(const_cast<Float*>(pChannel)));
}
