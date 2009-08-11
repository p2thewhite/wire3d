#include "WireVertexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
VertexBuffer::VertexBuffer(const VertexAttributes& rAttributes,
	UInt vertexQuantity)
	:
	mAttributes(rAttributes),
	mVertexQuantity(vertexQuantity),
	mpChannel(NULL)
{
	mpChannel = WIRE_NEW Float[mVertexQuantity * mAttributes.
		GetChannelQuantity()];
}

//----------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	WIRE_DELETE[] mpChannel;
}

//----------------------------------------------------------------------------
Vector3f& VertexBuffer::Position3(UInt i)
{
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetPositionOffset();
	return *(reinterpret_cast<Vector3f*>(pChannel));
}

//----------------------------------------------------------------------------
Vector3f VertexBuffer::Position3(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetPositionOffset();
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<Vector3f*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
ColorRGB& VertexBuffer::Color3(UInt i)
{
	WIRE_ASSERT(mAttributes.GetColorChannels() == 3);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetColorOffset();
	return *(reinterpret_cast<ColorRGB*>(pChannel));
}

//----------------------------------------------------------------------------
ColorRGB VertexBuffer::Color3(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetColorChannels() == 3);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetColorOffset();
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<ColorRGB*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
Vector3f& VertexBuffer::Normal3(UInt i)
{
	WIRE_ASSERT(mAttributes.GetNormalChannels() == 3);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetNormalOffset();
	return *(reinterpret_cast<Vector3f*>(pChannel));
}

//----------------------------------------------------------------------------
Vector3f VertexBuffer::Normal3(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetNormalChannels() == 3);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetNormalOffset();
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<Vector3f*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
Vector2f& VertexBuffer::TCoord2(UInt i)
{
	WIRE_ASSERT(mAttributes.GetTCoordChannels() == 2);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetTCoordOffset();
	return *(reinterpret_cast<Vector2f*>(pChannel));
}

//----------------------------------------------------------------------------
Vector2f VertexBuffer::TCoord2(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetTCoordChannels() == 2);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetTCoordOffset();
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<Vector2f*>(const_cast<Float*>(pChannel)));
}
