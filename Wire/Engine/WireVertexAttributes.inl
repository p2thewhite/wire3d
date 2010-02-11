//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetChannelQuantity() const
{
	return mChannelQuantity;
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetColorChannelQuantity() const
{
	return mColorChannels.GetQuantity();
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetTCoordChannelQuantity() const
{
	return mTCoordChannels.GetQuantity();
}

//----------------------------------------------------------------------------
inline void VertexAttributes::SetInterleaved(Bool isChannelInterleaved)
{
	mIsChannelInterleaved = isChannelInterleaved;
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetPositionChannels() const
{
	return mPositionChannels;
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetColorChannels(UInt unit) const
{
	WIRE_ASSERT(unit < mColorChannels.GetQuantity());
	return mColorChannels[unit];
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetNormalChannels() const
{
	return mNormalChannels;
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetTCoordChannels(UInt unit) const
{
	WIRE_ASSERT(unit < mTCoordChannels.GetQuantity());
	return mTCoordChannels[unit];
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetPositionOffset() const
{
	return mPositionOffset;
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetColorOffset(UInt unit) const
{
	WIRE_ASSERT(unit < mColorOffset.GetQuantity());
	return mColorOffset[unit];
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetNormalOffset() const
{
	return mNormalOffset;
}

//----------------------------------------------------------------------------
inline UInt VertexAttributes::GetTCoordOffset(UInt unit) const
{
	WIRE_ASSERT(unit < mTCoordOffset.GetQuantity());
	return mTCoordOffset[unit];
}
