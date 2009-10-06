//----------------------------------------------------------------------------
inline void Controller::SetObject(SceneObject* pObject)
{
	mpObject = pObject;
}

//----------------------------------------------------------------------------
inline SceneObject* Controller::GetObject() const
{
	return mpObject;
}
