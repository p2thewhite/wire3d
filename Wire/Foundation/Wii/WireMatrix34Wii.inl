//-------------------------------------------------------------------------
template <class Real>
Matrix34<Real>::Matrix34(Bool zero)
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

//-------------------------------------------------------------------------
template <class Real>
Matrix34<Real>::Matrix34(
	Real m00, Real m01, Real m02, Real m03,
	Real m10, Real m11, Real m12, Real m13,
	Real m20, Real m21, Real m22, Real m23)
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
}

//-------------------------------------------------------------------------
template <class Real>
Matrix34<Real>& Matrix34<Real>::MakeZero()
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

	return *this;
}

//-------------------------------------------------------------------------
template <class Real>
Matrix34<Real>& Matrix34<Real>::MakeIdentity()
{
	MTXIdentity(mEntry);
	return *this;
}

//-------------------------------------------------------------------------
template <class Real>
Matrix34<Real>& Matrix34<Real>::FromAxisAngle(const Vector3<Real>& rAxis,
	Real angle)
{
	Vector3<Real>* pAxis = const_cast<Vector3<Real>*>(&rAxis);
	MTXRotAxisRad(mEntry, reinterpret_cast<Vector*>(pAxis), angle);
	return *this;
}

//-------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real> Matrix34<Real>::operator* (Matrix34& rMatrix)
{
	Matrix34<Real> result;
	MTXConcat(mEntry, rMatrix, result);
	return result;
}

//-------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real>::operator Real4* ()
{
	return mEntry;
}

//-------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real>::operator const Real4* () const
{
	return mEntry;
}

//-------------------------------------------------------------------------
template <class Real>
void Matrix34<Real>::SetColumn(Int col, const Vector3<Real>& rVector)
{
//	WIRE_ASSERT((0 =< col) && (col < 4));
	mEntry[0][col] = rVector.X();
	mEntry[1][col] = rVector.Y();
	mEntry[2][col] = rVector.Z();
}

//-------------------------------------------------------------------------
template <class Real>
Vector3<Real> Matrix34<Real>::GetColumn(Int col) const
{
	return Vector3<Real>(mEntry[0][col], mEntry[1][col], mEntry[2][col]);
}
