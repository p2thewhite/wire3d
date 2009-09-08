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

// TODO: test
//----------------------------------------------------------------------------
template <class Real>
Sphere3<Real> MergeSpheres (const Sphere3<Real>& rkSphere0,
							const Sphere3<Real>& rkSphere1)
{
	Vector3<Real> kCDiff = rkSphere1.Center - rkSphere0.Center;
	Real fLSqr = kCDiff.SquaredLength();
	Real fRDiff = rkSphere1.Radius - rkSphere0.Radius;
	Real fRDiffSqr = fRDiff*fRDiff;

	if (fRDiffSqr >= fLSqr)
	{
		return ( fRDiff >= (Real)0.0 ? rkSphere1 : rkSphere0 );
	}

	Real fLength = Math<Real>::Sqrt(fLSqr);
	Sphere3<Real> kSphere;

	if (fLength > Math<Real>::ZERO_TOLERANCE)
	{
		Real fCoeff = (fLength + fRDiff)/(((Real)2.0)*fLength);
		kSphere.Center = rkSphere0.Center + fCoeff*kCDiff;
	}
	else
	{
		kSphere.Center = rkSphere0.Center;
	}

	kSphere.Radius = ((Real)0.5)*(fLength + rkSphere0.Radius +
		rkSphere1.Radius);

	return kSphere;
}
