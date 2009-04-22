//-------------------------------------------------------------------------
template <class Real>
Vector3<Real>::Vector3()
{
	// uninitialized for performance in array construction
}

//-------------------------------------------------------------------------
template <class Real>
Vector3<Real>::Vector3(Real x, Real y, Real z)
{
	mTuple[0] = x;
	mTuple[1] = y;
	mTuple[2] = z;
}

//-------------------------------------------------------------------------
template <class Real>
inline Vector3<Real>::operator Real* ()
{
	return mTuple;
}

//-------------------------------------------------------------------------
template <class Real>
inline Vector3<Real>::operator const Real* () const
{
	return mTuple;
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::X() const
{
	return mTuple[0];
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::X()
{
	return mTuple[0];
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Y() const
{
	return mTuple[1];
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Y()
{
	return mTuple[1];
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Z() const
{
	return mTuple[2];
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Z()
{
	return mTuple[2];
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::Length() const
{
	return Math<Real>::Sqrt(
		mTuple[0] * mTuple[0] +
		mTuple[1] * mTuple[1] +
		mTuple[2] * mTuple[2]);
}

//-------------------------------------------------------------------------
template <class Real>
inline Real Vector3<Real>::SquaredLength() const
{
	return
		mTuple[0] * mTuple[0] +
		mTuple[1] * mTuple[1] +
		mTuple[2] * mTuple[2];
}

//-------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator+ (const Vector3& rVector)
{
	return Vector3(
		mTuple[0] + rVector.mTuple[0],
		mTuple[1] + rVector.mTuple[1],
		mTuple[2] + rVector.mTuple[2]);
}

//-------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator- (const Vector3& rVector)
{
	return Vector3(
		mTuple[0] - rVector.mTuple[0],
		mTuple[1] - rVector.mTuple[1],
		mTuple[2] - rVector.mTuple[2]);
}

//-------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator* (Real scalar)
{
	return Vector3(
		scalar * mTuple[0],
		scalar * mTuple[1],
		scalar * mTuple[2]);
}

//-------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator/ (Real scalar)
{
	Vector3 quot;

	if (scalar != static_cast<Real>(0.0))
	{
		Real invScalar = (static_cast<Real>(1.0)) / scalar;
		quot.mTuple[0] = invScalar * mTuple[0];
		quot.mTuple[1] = invScalar * mTuple[1];
		quot.mTuple[2] = invScalar * mTuple[2];
	}
	else
	{
		quot.mTuple[0] = Math<Real>::MAX_REAL;
		quot.mTuple[1] = Math<Real>::MAX_REAL;
		quot.mTuple[2] = Math<Real>::MAX_REAL;
	}

	return quot;
}

//-------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Vector3<Real>::operator- () const
{
	return Vector3(
		-mTuple[0],
		-mTuple[1],
		-mTuple[2]);
}
