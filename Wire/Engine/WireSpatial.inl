//----------------------------------------------------------------------------
inline void Spatial::SetParent(Spatial* pParent)
{
	mpParent = pParent;
}
//----------------------------------------------------------------------------
inline Spatial* Spatial::GetParent()
{
	return mpParent;
}
