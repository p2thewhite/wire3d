#include "WireVertexAttributes.h"

using namespace Wire;

//-------------------------------------------------------------------------
VertexAttributes::VertexAttributes()
	:
	mChannelQuantity(0),
	mPositionChannels(0),
	mColorChannels(0),
	mNormalChannels(0),
	mTCoordChannels(0)
{
	ResetOffsets();
}

//-------------------------------------------------------------------------
VertexAttributes::~VertexAttributes()
{
}

//-------------------------------------------------------------------------
void VertexAttributes::ResetOffsets()
{
	mPositionOffset = -1;
	mColorOffset = -1;
	mNormalOffset = -1;
	mTCoordOffset = -1;
}

//-------------------------------------------------------------------------
void VertexAttributes::SetPositionChannels(UInt positionChannels)
{
	WIRE_ASSERT(0 <= positionChannels && positionChannels <= 3);

	mPositionChannels = positionChannels;
	UpdateOffsets();
}

//-------------------------------------------------------------------------
void VertexAttributes::SetColorChannels(UInt colorChannels)
{
	WIRE_ASSERT(0 <= colorChannels && colorChannels <= 4);

	mColorChannels = colorChannels;
	UpdateOffsets();
}

//-------------------------------------------------------------------------
void VertexAttributes::SetNormalChannels(UInt normalChannels)
{
	WIRE_ASSERT(0 <= normalChannels && normalChannels <= 4);

	mNormalChannels = normalChannels;
	UpdateOffsets();
}

//-------------------------------------------------------------------------
void VertexAttributes::SetTCoordChannels(UInt tCoordChannels)
{
	WIRE_ASSERT(0 <= tCoordChannels && tCoordChannels <= 4);

	mTCoordChannels = tCoordChannels;
	UpdateOffsets();
}

//-------------------------------------------------------------------------
void VertexAttributes::UpdateOffsets()
{
	mChannelQuantity = 0;

	ResetOffsets();	

	if (mPositionChannels > 0)
	{
		mPositionOffset = mChannelQuantity;
		mChannelQuantity += mPositionChannels;
	}

	if (mColorChannels > 0)
	{
		mColorOffset = mChannelQuantity;
		mChannelQuantity += mColorChannels;
	}

	if (mNormalChannels > 0)
	{
		mNormalOffset = mChannelQuantity;
		mChannelQuantity += mNormalChannels;
	}

	if (mTCoordChannels > 0)
	{
		mTCoordOffset = mChannelQuantity;
		mChannelQuantity += mTCoordChannels;
	}
}
