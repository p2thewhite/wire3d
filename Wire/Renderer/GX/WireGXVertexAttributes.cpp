// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGXVertexAttributes.h"

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexAttributes::PdrVertexAttributes(Renderer*, const VertexAttributes&
	rAttributes)
	:
	mVertexSize(0)
{
	VertexElement element;

	UInt channels = rAttributes.GetPositionChannels();
	if (channels > 0)
	{
		WIRE_ASSERT(channels == 3);
		element.Offset = mVertexSize;
		mVertexSize += channels * sizeof(Float);
		element.Attr = GX_VA_POS;
		element.CompCnt = GX_POS_XYZ;
		element.CompType = GX_F32;
		mDeclaration.Append(element);
	}

	channels = rAttributes.GetNormalChannels();
	if (channels > 0)
	{
		WIRE_ASSERT(channels == 3);
		element.Offset = mVertexSize;
		mVertexSize += channels * sizeof(Float);
		element.Attr = GX_VA_NRM;
		element.CompCnt = GX_NRM_XYZ;
		element.CompType = GX_F32;
		mDeclaration.Append(element);
	}

	for (UInt unit = 0; unit < rAttributes.GetColorChannelQuantity(); unit++)
	{
		if (rAttributes.GetColorChannels(unit) > 0)
		{
			element.Offset = mVertexSize;
			mVertexSize += sizeof(UInt);
			element.Attr = GX_VA_CLR0 + unit;
			element.CompCnt = GX_CLR_RGBA;
			element.CompType = GX_RGBA8;
			mDeclaration.Append(element);
		}
	}

	for (UInt unit = 0; unit < rAttributes.GetTCoordChannelQuantity(); unit++)
	{
		channels = rAttributes.GetTCoordChannels(unit);
		if (channels > 0)
		{
			WIRE_ASSERT(channels == 2);
			element.Offset = mVertexSize;
			mVertexSize += channels * sizeof(Float);
			element.Attr = GX_VA_TEX0 + unit;
			element.CompCnt = GX_TEX_ST;
			element.CompType = GX_F32;
			mDeclaration.Append(element);
		}
	}
}
