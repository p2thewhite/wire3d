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
	Center(rCenter),
	Radius(radius)
{
}
//----------------------------------------------------------------------------
template <class Real>
Sphere3<Real>::Sphere3(const Sphere3& rSphere)
	:
	Center(rSphere.Center),
	Radius(rSphere.Radius)
{
}
//----------------------------------------------------------------------------
template <class Real>
Sphere3<Real>& Sphere3<Real>::operator= (const Sphere3& rSphere)
{
	Center = rSphere.Center;
	Radius = rSphere.Radius;
	return *this;
}
