//----------------------------------------------------------------------------
template <class Real>
Vector3<Real>::Vector3()
{
	// uninitialized for performance in array construction
}

//----------------------------------------------------------------------------
template <class Real>
Vector3<Real>::Vector3(Real x, Real y, Real z)
{
	mTuple.x = x;
	mTuple.y = y;
	mTuple.z = z;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real>::operator Real* ()
{
	return reinterpret_cast<Real*>(&mTuple);
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real>::operator const Real* () const
{
	return reinterpret_cast<Real*>(&mTuple);
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::X() const
{
	return mTuple.x;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::X()
{
	return mTuple.x;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Y() const
{
	return mTuple.y;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Y()
{
	return mTuple.y;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Z() const
{
	return mTuple.z;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Z()
{
	return mTuple.z;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Length() const
{
//	return VECMag(&mTuple);			// No OGC equivalent exists :(

	return Math<Real>::Sqrt(
		mTuple.x * mTuple.x +
		mTuple.y * mTuple.y +
		mTuple.z * mTuple.z);
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::SquaredLength() const
{
//	return VECSquareMag(&mTuple);	// No OGC equivalent exists :(

	return
		mTuple.x * mTuple.x +
		mTuple.y * mTuple.y +
		mTuple.z * mTuple.z;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator+ (const Vector3& rVector)
const
{
	Vector3 result;
	VECAdd(const_cast<Vector*>(&mTuple), const_cast<Vector*>(&rVector.
		mTuple), &result.mTuple);
	return result;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator- (const Vector3& rVector)
const
{
	Vector3 result;
	VECSubtract(const_cast<Vector*>(&mTuple), const_cast<Vector*>(&rVector.
		mTuple), &result.mTuple);
	return result;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator* (Real scalar) const
{
	Vector3 result;
	VECScale(&mTuple, &result.mTuple, scalar);
	return result;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator/ (Real scalar) const
{
	Vector3 quot;

	if (scalar != static_cast<Real>(0.0))
	{
		Real invScalar = (static_cast<Real>(1.0)) / scalar;
		VECScale(&mTuple, &quot.mTuple, invScalar);
	}
	else
	{
		quot.mTuple.x = Math<Real>::MAX_REAL;
 		quot.mTuple.y = Math<Real>::MAX_REAL;
 		quot.mTuple.z = Math<Real>::MAX_REAL;
	}

	return quot;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator- () const
{
	return Vector3(
		-mTuple.x,
		-mTuple.y,
		-mTuple.z);
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::Cross(const Vector3& rVector) const
{
	Vector3 result;
	VECCrossProduct(const_cast<Vector*>(&mTuple), 
		const_cast<Vector*>(&rVector.mTuple), &result.mTuple);
	return result;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Dot(const Vector3& rVector) const
{
	return VECDotProduct(const_cast<Vector*>(&mTuple), 
		const_cast<Vector*>(&rVector.mTuple));
}
