#include "WireVertexAttributes.h"

using namespace Wire;

//----------------------------------------------------------------------------
VertexAttributes::VertexAttributes()
	:
	mChannelQuantity(0),
	mPositionChannels(0),
	mNormalChannels(0)
{
	ResetOffsets();
}

//----------------------------------------------------------------------------
VertexAttributes::~VertexAttributes()
{
}

//----------------------------------------------------------------------------
void VertexAttributes::ResetOffsets()
{
	mPositionOffset = -1;
	mNormalOffset = -1;
}

//----------------------------------------------------------------------------
void VertexAttributes::SetPositionChannels(UInt positionChannels)
{
	WIRE_ASSERT(0 <= positionChannels && positionChannels <= 3);

	mPositionChannels = positionChannels;
	UpdateOffsets();
}

//----------------------------------------------------------------------------
void VertexAttributes::SetColorChannels(UInt colorChannels, UInt unit)
{
	WIRE_ASSERT(0 <= colorChannels && colorChannels <= 4);

	if (mColorChannels.GetQuantity() <= unit)
	{
		mColorChannels.SetElement(unit, colorChannels);
	}
	else
	{
		mColorChannels[unit] = colorChannels;
	}

	UpdateOffsets();
}

//----------------------------------------------------------------------------
void VertexAttributes::SetNormalChannels(UInt normalChannels)
{
	WIRE_ASSERT(0 <= normalChannels && normalChannels <= 4);

	mNormalChannels = normalChannels;
	UpdateOffsets();
}

//----------------------------------------------------------------------------
void VertexAttributes::SetTCoordChannels(UInt tCoordChannels, UInt unit)
{
	WIRE_ASSERT(0 <= tCoordChannels && tCoordChannels <= 4);

	if (mTCoordChannels.GetQuantity() <= unit)
	{
		mTCoordChannels.SetElement(unit, tCoordChannels);
	}
	else
	{
		mTCoordChannels[unit] = tCoordChannels;
	}

	UpdateOffsets();
}

//----------------------------------------------------------------------------
void VertexAttributes::UpdateOffsets()
{
	mChannelQuantity = 0;

	ResetOffsets();	

	if (mPositionChannels > 0)
	{
		mPositionOffset = mChannelQuantity;
		mChannelQuantity += mPositionChannels;
	}

	if (mNormalChannels > 0)
	{
		mNormalOffset = mChannelQuantity;
		mChannelQuantity += mNormalChannels;
	}

	if (mColorChannels.GetQuantity() > 0)
	{
		for (UInt i = 0; i < mColorChannels.GetQuantity(); i++)
		{
			if (mColorChannels[i] > 0)
			{
				mColorOffset.SetElement(i, mChannelQuantity);
				mChannelQuantity += mColorChannels[i];
			}
		}
	}

	if (mTCoordChannels.GetQuantity() > 0)
	{
		for (UInt i = 0; i < mTCoordChannels.GetQuantity(); i++)
		{
			if (mTCoordChannels[i] > 0)
			{
				mTCoordOffset.SetElement(i, mChannelQuantity);
				mChannelQuantity += mTCoordChannels[i];
			}
		}
	}
}
