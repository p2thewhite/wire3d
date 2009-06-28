//----------------------------------------------------------------------------
template <class Real>
Vector2<Real>::Vector2()
{
	// uninitialized for performance in array construction
}

//----------------------------------------------------------------------------
template <class Real>
Vector2<Real>::Vector2(Real x, Real y)
{
	mTuple[0] = x;
	mTuple[1] = y;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector2<Real>::operator Real* ()
{
	return mTuple;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector2<Real>::operator const Real* () const
{
	return mTuple;
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector2<Real>::X() const
{
	return mTuple[0];
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector2<Real>::X()
{
	return mTuple[0];
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector2<Real>::Y() const
{
	return mTuple[1];
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector2<Real>::Y()
{
	return mTuple[1];
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector2<Real>::Length() const
{
	return Math<Real>::Sqrt(
		mTuple[0] * mTuple[0] +
		mTuple[1] * mTuple[1]);
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Vector2<Real>::SquaredLength() const
{
	return
		mTuple[0] * mTuple[0] +
		mTuple[1] * mTuple[1];
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector2<Real> Vector2<Real>::operator+ (const Vector2& rVector)
{
	return Vector2(
		mTuple[0] + rVector.mTuple[0],
		mTuple[1] + rVector.mTuple[1]);
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector2<Real> Vector2<Real>::operator- (const Vector2& rVector)
{
	return Vector2(
		mTuple[0] - rVector.mTuple[0],
		mTuple[1] - rVector.mTuple[1]);
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector2<Real> Vector2<Real>::operator* (Real scalar)
{
	return Vector2(
		scalar * mTuple[0],
		scalar * mTuple[1]);
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector2<Real> Vector2<Real>::operator/ (Real scalar)
{
	Vector2 quot;

	if (scalar != static_cast<Real>(0.0))
	{
		Real invScalar = (static_cast<Real>(1.0)) / scalar;
		quot.mTuple[0] = invScalar * mTuple[0];
		quot.mTuple[1] = invScalar * mTuple[1];
	}
	else
	{
		quot.mTuple[0] = Math<Real>::MAX_REAL;
		quot.mTuple[1] = Math<Real>::MAX_REAL;
	}

	return quot;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector2<Real> Vector2<Real>::operator- () const
{
	return Vector2(
		-mTuple[0],
		-mTuple[1]);
}
