//----------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::Matrix4(Bool zero)
{
	if (zero)
	{
		MakeZero();
	}
	else
	{
		MakeIdentity();
	}
}

//----------------------------------------------------------------------------
template <class Real>
Matrix4<Real>::Matrix4(
	Real m00, Real m01, Real m02, Real m03,
	Real m10, Real m11, Real m12, Real m13,
	Real m20, Real m21, Real m22, Real m23,
	Real m30, Real m31, Real m32, Real m33)
{
	mEntry[0][0] = m00;
	mEntry[0][1] = m01;
	mEntry[0][2] = m02;
	mEntry[0][3] = m03;

	mEntry[1][0] = m10;
	mEntry[1][1] = m11;
	mEntry[1][2] = m12;
	mEntry[1][3] = m13;

	mEntry[2][0] = m20;
	mEntry[2][1] = m21;
	mEntry[2][2] = m22;
	mEntry[2][3] = m23;

	mEntry[3][0] = m30;
	mEntry[3][1] = m31;
	mEntry[3][2] = m32;
	mEntry[3][3] = m33;
}

//----------------------------------------------------------------------------
template <class Real>
Matrix4<Real>& Matrix4<Real>::MakeZero()
{
	mEntry[0][0] = static_cast<Real>(0.0);
	mEntry[0][1] = static_cast<Real>(0.0);
	mEntry[0][2] = static_cast<Real>(0.0);
	mEntry[0][3] = static_cast<Real>(0.0);

	mEntry[1][0] = static_cast<Real>(0.0);
	mEntry[1][1] = static_cast<Real>(0.0);
	mEntry[1][2] = static_cast<Real>(0.0);
	mEntry[1][3] = static_cast<Real>(0.0);

	mEntry[2][0] = static_cast<Real>(0.0);
	mEntry[2][1] = static_cast<Real>(0.0);
	mEntry[2][2] = static_cast<Real>(0.0);
	mEntry[2][3] = static_cast<Real>(0.0);

	mEntry[3][0] = static_cast<Real>(0.0);
	mEntry[3][1] = static_cast<Real>(0.0);
	mEntry[3][2] = static_cast<Real>(0.0);
	mEntry[3][3] = static_cast<Real>(0.0);

	return *this;
}

//----------------------------------------------------------------------------
template <class Real>
Matrix4<Real>& Matrix4<Real>::MakeIdentity()
{
	mEntry[0][0] = static_cast<Real>(1.0);
	mEntry[0][1] = static_cast<Real>(0.0);
	mEntry[0][2] = static_cast<Real>(0.0);
	mEntry[0][3] = static_cast<Real>(0.0);

	mEntry[1][0] = static_cast<Real>(0.0);
	mEntry[1][1] = static_cast<Real>(1.0);
	mEntry[1][2] = static_cast<Real>(0.0);
	mEntry[1][3] = static_cast<Real>(0.0);

	mEntry[2][0] = static_cast<Real>(0.0);
	mEntry[2][1] = static_cast<Real>(0.0);
	mEntry[2][2] = static_cast<Real>(1.0);
	mEntry[2][3] = static_cast<Real>(0.0);

	mEntry[3][0] = static_cast<Real>(0.0);
	mEntry[3][1] = static_cast<Real>(0.0);
	mEntry[3][2] = static_cast<Real>(0.0);
	mEntry[3][3] = static_cast<Real>(1.0);

	return *this;
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix4<Real>::operator Real4* ()
{
	return mEntry;
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix4<Real>::operator const Real4* () const
{
	return mEntry;
}
