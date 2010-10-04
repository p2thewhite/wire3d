//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::ATan(Real value)
{
	return static_cast<Real>(atan(static_cast<Double>(value)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::ATan2(Real y, Real x)
{
    return static_cast<Real>(atan2(static_cast<Double>(y),
		static_cast<Double>(x)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Cos(Real value)
{
	return static_cast<Real>(cos(static_cast<Double>(value)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::FAbs(Real value)
{
	return static_cast<Real>(fabs(static_cast<Double>(value)));
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Floor(Real value)
{
	return static_cast<Real>(floor(static_cast<Double>(value)));
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

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Min(Real value1, Real value2)
{
	return value1 < value2 ? value1 : value2;
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Max(Real value1, Real value2)
{
	return value1 > value2 ? value1 : value2;
}

//----------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Clamp(Real min, Real value, Real max)
{
	if (value < min)
	{
		return min;
	}

	if (value > max)
	{
		return max;
	}

	return value;
}
