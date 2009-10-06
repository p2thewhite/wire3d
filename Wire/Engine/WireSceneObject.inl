//----------------------------------------------------------------------------
inline UInt SceneObject::GetControllerQuantity() const
{
	return mControllers.GetQuantity();
}

//----------------------------------------------------------------------------
inline Controller* SceneObject::GetController(UInt i) const
{
	WIRE_ASSERT(i < mControllers.GetQuantity());
	return mControllers[i];
}
