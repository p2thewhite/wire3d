//----------------------------------------------------------------------------
inline void SphereBV::SetCenter(const Vector3F& rCenter)
{
	mSphere.mCenter = rCenter;
}

//----------------------------------------------------------------------------
inline void SphereBV::SetRadius(Float radius)
{
	mSphere.mRadius = radius;
}

//----------------------------------------------------------------------------
inline Vector3F SphereBV::GetCenter() const
{
	return mSphere.mCenter;
}

//----------------------------------------------------------------------------
inline Float SphereBV::GetRadius() const
{
	return mSphere.mRadius;
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
