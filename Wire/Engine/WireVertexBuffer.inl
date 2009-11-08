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
