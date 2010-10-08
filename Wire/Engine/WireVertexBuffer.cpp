#include "WireVertexBuffer.h"

#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(VertexBuffer, Object);

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
	Renderer::UnbindAll(this);

	WIRE_DELETE[] mpChannel;
}

//----------------------------------------------------------------------------
Vector3F& VertexBuffer::Position3(UInt i)
{
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	Float* pChannel = GetPosition(i);
	return *(reinterpret_cast<Vector3F*>(pChannel));
}

//----------------------------------------------------------------------------
Vector3F VertexBuffer::Position3(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetPositionChannels() == 3);
	const Float* pChannel = GetPosition(i);
	return *(reinterpret_cast<Vector3F*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
ColorRGB& VertexBuffer::Color3(UInt i, UInt unit)
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 3);
	Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<ColorRGB*>(pChannel));
}

//----------------------------------------------------------------------------
ColorRGB VertexBuffer::Color3(UInt i, UInt unit) const
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 3);
	const Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<ColorRGB*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
ColorRGBA& VertexBuffer::Color4(UInt i, UInt unit)
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 4);
	Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<ColorRGBA*>(pChannel));
}

//----------------------------------------------------------------------------
ColorRGBA VertexBuffer::Color4(UInt i, UInt unit) const
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 4);
	const Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<ColorRGBA*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
Vector3F& VertexBuffer::Normal3(UInt i)
{
	WIRE_ASSERT(mAttributes.GetNormalChannels() == 3);
	Float* pChannel = GetNormal(i);
	return *(reinterpret_cast<Vector3F*>(pChannel));
}

//----------------------------------------------------------------------------
Vector3F VertexBuffer::Normal3(UInt i) const
{
	WIRE_ASSERT(mAttributes.GetNormalChannels() == 3);
	const Float* pChannel = GetNormal(i);
	return *(reinterpret_cast<Vector3F*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
Vector2F& VertexBuffer::TCoord2(UInt i, UInt unit)
{
	WIRE_ASSERT(mAttributes.GetTCoordChannels(unit) == 2);
	Float* pChannel = GetTCoord(i, unit);
	return *(reinterpret_cast<Vector2F*>(pChannel));
}

//----------------------------------------------------------------------------
Vector2F VertexBuffer::TCoord2(UInt i, UInt unit) const
{
	WIRE_ASSERT(mAttributes.GetTCoordChannels(unit) == 2);
	const Float* pChannel = GetTCoord(i, unit);
	return *(reinterpret_cast<Vector2F*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
void VertexBuffer::GeneratePlatonicNormals()
{
	const VertexAttributes& rAttr = mAttributes;

	if (rAttr.GetNormalChannels() == rAttr.GetPositionChannels() &&
		rAttr.GetNormalChannels() == 3)
	{
		for (UInt i = 0; i < mVertexQuantity; i++)
		{	
			Normal3(i) = Position3(i);
			Normal3(i).Normalize();
		}
	}
	else
	{
		WIRE_ASSERT(false);
	}
}
