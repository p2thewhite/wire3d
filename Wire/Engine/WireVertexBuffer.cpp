// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireVertexBuffer.h"

#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, VertexBuffer, Buffer);

//----------------------------------------------------------------------------
VertexBuffer::VertexBuffer(const VertexAttributes& rAttributes,
	UInt vertexQuantity, UsageType usage)
	:
	Buffer(usage),
	mAttributes(rAttributes),
	mVertexQuantity(vertexQuantity)
{
	mpChannel = WIRE_NEW Float[mVertexQuantity * mAttributes.
		GetChannelQuantity()];
}

//----------------------------------------------------------------------------
VertexBuffer::VertexBuffer(const VertexBuffer* pVertexBuffer)
	:
	Buffer(pVertexBuffer->GetUsage())
{
	mAttributes = pVertexBuffer->mAttributes;
	mVertexQuantity = pVertexBuffer->mVertexQuantity;
	UInt floatQuantity = mVertexQuantity * mAttributes.GetChannelQuantity();
	mpChannel = WIRE_NEW Float[floatQuantity];
	size_t size = floatQuantity*sizeof(Float);
	System::Memcpy(mpChannel, size, pVertexBuffer->mpChannel, size);
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
Color32& VertexBuffer::Color3(UInt i, UInt unit)
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 3);
	Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<Color32*>(pChannel));
}

//----------------------------------------------------------------------------
Color32 VertexBuffer::Color3(UInt i, UInt unit) const
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 3);
	const Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<Color32*>(const_cast<Float*>(pChannel)));
}

//----------------------------------------------------------------------------
Color32& VertexBuffer::Color4(UInt i, UInt unit)
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 4);
	Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<Color32*>(pChannel));
}

//----------------------------------------------------------------------------
Color32 VertexBuffer::Color4(UInt i, UInt unit) const
{
	WIRE_ASSERT(mAttributes.GetColorChannels(unit) == 4);
	const Float* pChannel = GetColor(i, unit);
	return *(reinterpret_cast<Color32*>(const_cast<Float*>(pChannel)));
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
