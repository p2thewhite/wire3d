//----------------------------------------------------------------------------
inline UInt IndexBuffer::GetIndexQuantity() const
{
	return mQuantity;
}

//----------------------------------------------------------------------------
inline UInt* IndexBuffer::GetData()
{
	return mpIndices;
}

//----------------------------------------------------------------------------
inline const UInt* IndexBuffer::GetData() const
{
	return mpIndices;
}

//----------------------------------------------------------------------------
inline UInt IndexBuffer::operator[] (UInt i) const
{
	WIRE_ASSERT(i < mQuantity);
	return mpIndices[i];
}
//----------------------------------------------------------------------------
inline UInt& IndexBuffer::operator[] (UInt i)
{
	WIRE_ASSERT(i < mQuantity);
	return mpIndices[i];
}
