//----------------------------------------------------------------------------
template <class Real>
Sphere3<Real>::Sphere3()
{
	// uninitialized
}
//----------------------------------------------------------------------------
template <class Real>
Sphere3<Real>::Sphere3(const Vector3<Real>& rCenter, Real radius)
	:
	mCenter(rCenter),
	mRadius(radius)
{
}
//----------------------------------------------------------------------------
template <class Real>
Sphere3<Real>::Sphere3(const Sphere3& rSphere)
	:
	mCenter(rSphere.mCenter),
	mRadius(rSphere.mRadius)
{
}
//----------------------------------------------------------------------------
template <class Real>
Sphere3<Real>& Sphere3<Real>::operator= (const Sphere3& rSphere)
{
	mCenter = rSphere.mCenter;
	mRadius = rSphere.mRadius;
	return *this;
}
