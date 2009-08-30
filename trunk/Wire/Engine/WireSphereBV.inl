//----------------------------------------------------------------------------
inline void SphereBV::SetCenter(const Vector3F& rCenter)
{
	mSphere.Center = rCenter;
}

//----------------------------------------------------------------------------
inline void SphereBV::SetRadius(Float radius)
{
	mSphere.Radius = radius;
}

//----------------------------------------------------------------------------
inline Vector3F SphereBV::GetCenter() const
{
	return mSphere.Center;
}

//----------------------------------------------------------------------------
inline Float SphereBV::GetRadius() const
{
	return mSphere.Radius;
}

//----------------------------------------------------------------------------
inline Sphere3F& SphereBV::Sphere()
{
	return mSphere;
}

//----------------------------------------------------------------------------
inline const Sphere3F& SphereBV::GetSphere() const
{
	return mSphere;
}
