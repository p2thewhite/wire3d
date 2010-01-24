//----------------------------------------------------------------------------
inline Bool VisibleObject::IsDrawable() const
{
	return Object && !GlobalEffect;
}
