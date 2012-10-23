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
	WIRE_ASSERT(vertexQuantity < 65536 /* only 16bit indices on Wii */);
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
void VertexBuffer::ApplyForward(const Transformation& rTransformation,
	Float* pDst)
{
	const UInt quantity = GetQuantity();
	const UInt channelQuantity = mAttributes.GetChannelQuantity();
	Bool needsTrafo = mAttributes.HasPosition() || mAttributes.HasNormal();
	if (rTransformation.IsIdentity() || !needsTrafo)
	{
		if (GetData() != pDst)
		{
			size_t size = quantity * channelQuantity * sizeof(Float);
			System::Memcpy(pDst, size, GetData(), size);
			return;
		}
	}

	// TODO: optimize for dynamic runtime batching
	const VertexAttributes& rAttr = mAttributes;
	const Vector3F& translate = rTransformation.GetTranslate();
	const Matrix34F& rotate = rTransformation.GetMatrix();

	for (UInt i = 0; i < GetQuantity(); i++)
	{
		if (rAttr.GetPositionChannels() == 3)
		{
			const Float* const pPosition = GetPosition(i);
			Vector3F v(pPosition[0], pPosition[1], pPosition[2]);

			if (rTransformation.IsRSMatrix())
			{
				const Vector3F& scale = rTransformation.GetScale();
				v = Vector3F(scale.X() * v.X(), scale.Y() * v.Y(),
					scale.Z() * v.Z());
			}

			v = rotate * v + translate;

			*pDst++ = v.X();
			*pDst++ = v.Y();
			*pDst++ = v.Z();
		}
		else
		{
			WIRE_ASSERT(false /* implement transform for non 3d pos? */);
			const Float* const pPosition = GetPosition(i);
			for (UInt k = 0; k < rAttr.GetPositionChannels(); k++)
			{
				*pDst++ = pPosition[k];
			}
		}

		if (rAttr.GetNormalChannels() == 3)
		{
			const Float* const pNormal = GetNormal(i);
			Vector3F n(pNormal[0], pNormal[1], pNormal[2]);

			if (rTransformation.IsRSMatrix())
			{
				const Vector3F& scale = rTransformation.GetScale();
				n = Vector3F(scale.X() * n.X(), scale.Y() * n.Y(),
					scale.Z() * n.Z());
			}

			n = rotate * n;

			*pDst++ = n.X();
			*pDst++ = n.Y();
			*pDst++ = n.Z();
		}
		else if (rAttr.GetNormalChannels() > 0)
		{
			const Float* const pNormal = GetNormal(i);
			for (UInt k = 0; k < rAttr.GetNormalChannels(); k++)
			{
				*pDst++ = pNormal[k];
			}
		}

		UInt colorChannelQuantity = rAttr.GetColorChannelQuantity();
		for (UInt unit = 0; unit < colorChannelQuantity; unit++)
		{
			if (rAttr.GetColorChannels(unit) > 0)
			{
				UInt* pColor = reinterpret_cast<UInt*>(GetColor(i, unit));
				*(reinterpret_cast<UInt*>(pDst++)) = *pColor;
			}
		}

		UInt tCoordChannelQuantity = rAttr.GetTCoordChannelQuantity();
		for (UInt unit = 0; unit < tCoordChannelQuantity; unit++)
		{
			UInt channels = rAttr.GetTCoordChannels(unit);
			if (channels > 0)
			{
				const Float* const pTCoords = GetTCoord(i, unit);
				for (UInt k = 0; k < channels; k++)
				{
					*pDst++ = pTCoords[k];
				}
			}
		}
	}
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
