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
	// Inform all renderers using this vertex buffer that it is being
	// destroyed. This allows the renderer to free up any associated
	// resources.
	Release();

	WIRE_DELETE[] mpChannel;
}

//----------------------------------------------------------------------------
Vector3F& VertexBuffer::Position3(UInt i)
{
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetPositionOffset();
	return *(reinterpret_cast<Vector3F*>(pChannel));
}

//----------------------------------------------------------------------------
Vector3F VertexBuffer::Position3(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetPositionOffset();
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<Vector3F*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
ColorRGB& VertexBuffer::Color3(UInt i, UInt unit)
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 3);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetColorOffset(unit);
	return *(reinterpret_cast<ColorRGB*>(pChannel));
}

//----------------------------------------------------------------------------
ColorRGB VertexBuffer::Color3(UInt i, UInt unit) const
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 3);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetColorOffset(unit);
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<ColorRGB*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
Vector3F& VertexBuffer::Normal3(UInt i)
{
	WIRE_ASSERT(mAttributes.GetNormalChannels() == 3);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetNormalOffset();
	return *(reinterpret_cast<Vector3F*>(pChannel));
}

//----------------------------------------------------------------------------
Vector3F VertexBuffer::Normal3(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetNormalChannels() == 3);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetNormalOffset();
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<Vector3F*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
Vector2F& VertexBuffer::TCoord2(UInt i, UInt unit)
{
	WIRE_ASSERT(mAttributes.GetTCoordChannels(unit) == 2);
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetTCoordOffset(unit);
	return *(reinterpret_cast<Vector2F*>(pChannel));
}

//----------------------------------------------------------------------------
Vector2F VertexBuffer::TCoord2(UInt i, UInt unit) const
{
	WIRE_ASSERT(mAttributes.GetTCoordChannels(unit) == 2);
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetTCoordOffset(unit);
	const Float* pChannel = mpChannel + index;
	return *(reinterpret_cast<Vector2F*>(const_cast<Float*>(pChannel)));
}
