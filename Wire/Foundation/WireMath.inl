//-------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Cos(Real value)
{
	return static_cast<Real>(cos(static_cast<double>(value)));
}

//-------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Sin(Real value)
{
	return static_cast<Real>(sinf(static_cast<double>(value)));
}

//-------------------------------------------------------------------------
template <class Real>
Real Math<Real>::Sqrt(Real value)
{
	return static_cast<Real>(sqrt(static_cast<double>(value)));
}
