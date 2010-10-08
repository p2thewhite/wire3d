//----------------------------------------------------------------------------
inline UInt VertexBuffer::GetVertexQuantity() const
{
	return mVertexQuantity;
}

//----------------------------------------------------------------------------
inline Float* VertexBuffer::GetData()
{
	return mpChannel;
}

//----------------------------------------------------------------------------
inline const Float* VertexBuffer::GetData() const
{
	return mpChannel;
}

//----------------------------------------------------------------------------
inline const VertexAttributes& VertexBuffer::GetAttributes() const
{
	return mAttributes;
}

//----------------------------------------------------------------------------
inline Float* VertexBuffer::GetPosition(UInt i)
{
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetPositionOffset();
	return mpChannel + index;
}

//----------------------------------------------------------------------------
inline const Float* VertexBuffer::GetPosition(UInt i) const
{
	UInt index = mAttributes.GetChannelQuantity() * i +
		mAttributes.GetPositionOffset();
	return mpChannel + index;
}

//----------------------------------------------------------------------------
inline Float* VertexBuffer::GetColor(UInt i, UInt unit)
{
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetColorOffset(unit);
	return pChannel;
}

//----------------------------------------------------------------------------
inline const Float* VertexBuffer::GetColor(UInt i, UInt unit) const
{
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetColorOffset(unit);
	return pChannel;
}

//----------------------------------------------------------------------------
inline Float* VertexBuffer::GetNormal(UInt i)
{
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetNormalOffset();
	return pChannel;
}

//----------------------------------------------------------------------------
inline const Float* VertexBuffer::GetNormal(UInt i) const
{
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetNormalOffset();
	return pChannel;
}

//----------------------------------------------------------------------------
inline Float* VertexBuffer::GetTCoord(UInt i, UInt unit)
{
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetTCoordOffset(unit);
	return pChannel;
}

//----------------------------------------------------------------------------
inline const Float* VertexBuffer::GetTCoord(UInt i, UInt unit) const
{
	Float* pChannel = mpChannel + mAttributes.GetChannelQuantity() * i +
		mAttributes.GetTCoordOffset(unit);
	return pChannel;
}
