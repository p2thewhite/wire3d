//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::ATan(Real value)
{
	return static_cast<Real>(atan(static_cast<Double>(value)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Cos(Real value)
{
	return static_cast<Real>(cos(static_cast<Double>(value)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::FMod(Real x, Real y)
{
	return static_cast<Real>(
		fmod(static_cast<Double>(x), static_cast<Double>(y)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Sin(Real value)
{
	return static_cast<Real>(sin(static_cast<Double>(value)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Sqrt(Real value)
{
	return static_cast<Real>(sqrt(static_cast<Double>(value)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Tan(Real value)
{
	return static_cast<Real>(tan(static_cast<Double>(value)));
}
