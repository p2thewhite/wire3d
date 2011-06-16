// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
template <class Real>
Matrix34<Real>::Matrix34(const Vector3<Real>& rAxis, Real angle)
{
	FromAxisAngle(rAxis, angle);
}

//----------------------------------------------------------------------------
template <class Real>
Matrix34<Real>::Matrix34(const Vector3<Real>& rU, const Vector3<Real>& rV,
	const Vector3<Real>& rW, Bool isColumn)
{
	mEntry[0][3] = static_cast<Real>(0.0);
	mEntry[1][3] = static_cast<Real>(0.0);
	mEntry[2][3] = static_cast<Real>(0.0);

	if (isColumn)
	{
		mEntry[0][0] = rU[0];
		mEntry[0][1] = rV[0];
		mEntry[0][2] = rW[0];
		mEntry[1][0] = rU[1];
		mEntry[1][1] = rV[1];
		mEntry[1][2] = rW[1];
		mEntry[2][0] = rU[2];
		mEntry[2][1] = rV[2];
		mEntry[2][2] = rW[2];
	}
	else
	{
		mEntry[0][0] = rU[0];
		mEntry[0][1] = rU[1];
		mEntry[0][2] = rU[2];
		mEntry[1][0] = rV[0];
		mEntry[1][1] = rV[1];
		mEntry[1][2] = rV[2];
		mEntry[2][0] = rW[0];
		mEntry[2][1] = rW[1];
		mEntry[2][2] = rW[2];
	}
}

//----------------------------------------------------------------------------
template <class Real>
Matrix34<Real>::Matrix34(const Matrix3<Real>& rM, const Vector3<Real>& rV)
{
	mEntry[0][0] = rM[0][0];
	mEntry[0][1] = rM[0][1];
	mEntry[0][2] = rM[0][2];
	mEntry[0][3] = rV[0];
	mEntry[1][0] = rM[1][0];
	mEntry[1][1] = rM[1][1];
	mEntry[1][2] = rM[1][2];
	mEntry[1][3] = rV[1];
	mEntry[2][0] = rM[2][0];
	mEntry[2][1] = rM[2][1];
	mEntry[2][2] = rM[2][2];
	mEntry[2][3] = rV[2];
}

//----------------------------------------------------------------------------			 
template <class Real>
void Matrix34<Real>::SetMatrix3(const Matrix3<Real>& rM)
{
	mEntry[0][0] = rM[0][0];
	mEntry[0][1] = rM[0][1];
	mEntry[0][2] = rM[0][2];
	mEntry[1][0] = rM[1][0];
	mEntry[1][1] = rM[1][1];
	mEntry[1][2] = rM[1][2];
	mEntry[2][0] = rM[2][0];
	mEntry[2][1] = rM[2][1];
	mEntry[2][2] = rM[2][2];
}

//----------------------------------------------------------------------------
template <class Real>
void Matrix34<Real>::MakeZero()
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
}

//----------------------------------------------------------------------------
template <class Real>
void Matrix34<Real>::MakeIdentity()
{
	MTXIdentity(mEntry);
}

//----------------------------------------------------------------------------
template <class Real>
void Matrix34<Real>::FromAxisAngle(const Vector3<Real>& rAxis,
	Real angle)
{
	Vector3<Real>* pAxis = const_cast<Vector3<Real>*>(&rAxis);
	MTXRotAxisRad(mEntry, reinterpret_cast<Vec*>(pAxis), angle);
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real>::operator Real4* ()
{
	return mEntry;
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real>::operator const Real* () const
{
	return mEntry;
}

//----------------------------------------------------------------------------
template <class Real>
inline const Real* Matrix34<Real>::operator[] (Int row) const
{
	return &mEntry[row][0];
}

//----------------------------------------------------------------------------
template <class Real>
inline Real* Matrix34<Real>::operator[] (Int row)
{
	return &mEntry[row][0];
}

//----------------------------------------------------------------------------
template <class Real>
inline Real Matrix34<Real>::operator() (UInt row, UInt col) const
{
	return mEntry[row][col];
}

//----------------------------------------------------------------------------
template <class Real>
inline Real& Matrix34<Real>::operator() (UInt row, UInt col)
{
	return mEntry[row][col];
}

//----------------------------------------------------------------------------
template <class Real>
void Matrix34<Real>::SetRow(UInt row, const Vector3<Real>& rV)
{
	mEntry[row][0] = rV[0];
	mEntry[row][1] = rV[1];
	mEntry[row][2] = rV[2];
}

//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> Matrix34<Real>::GetRow(UInt row) const
{
	return Vector3<Real>(mEntry[row][0], mEntry[row][1], mEntry[row][1]);
}

//----------------------------------------------------------------------------
template <class Real>
void Matrix34<Real>::SetColumn(UInt col, const Vector3<Real>& rV)
{
	mEntry[0][col] = rV.X();
	mEntry[1][col] = rV.Y();
	mEntry[2][col] = rV.Z();
}

//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> Matrix34<Real>::GetColumn(UInt col) const
{
	return Vector3<Real>(mEntry[0][col], mEntry[1][col], mEntry[2][col]);
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real> Matrix34<Real>::operator* (const Matrix34& rMatrix)
const
{
	Matrix34<Real> result;
	MTXConcat(const_cast<Real4*>(mEntry), const_cast<Matrix34&>(rMatrix),
		result);
	return result;
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real> Matrix34<Real>::operator* (Real scalar) const
{
	return Matrix34<Real>(
		scalar * mEntry[0][0], scalar * mEntry[0][1], scalar * mEntry[0][2],
			mEntry[0][3],
		scalar * mEntry[1][0], scalar * mEntry[1][1], scalar * mEntry[1][2],
			mEntry[1][3],
		scalar * mEntry[2][0], scalar * mEntry[2][1], scalar * mEntry[2][2],
			mEntry[2][3]);
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> Matrix34<Real>::operator* (const Vector3<Real>& rV) const
{
	Vector3<Real> result;
	Vector3<Real>* pResult = const_cast<Vector3<Real>*>(&result);
	Vector3<Real>* pVector = const_cast<Vector3<Real>*>(&rV);
	MTXMultVecSR(const_cast<Real4*>(mEntry), reinterpret_cast<Vec*>(pVector),
		reinterpret_cast<Vec*>(pResult));
	return result;
}

//----------------------------------------------------------------------------
template <class Real>
Matrix34<Real> Matrix34<Real>::TimesDiagonal(const Vector3<Real>& rDiag) const
{
	return Matrix34<Real>(
		mEntry[0][0]*rDiag[0], mEntry[0][1]*rDiag[1], mEntry[0][2]*rDiag[2],
			mEntry[0][3],
		mEntry[1][0]*rDiag[0], mEntry[1][1]*rDiag[1], mEntry[1][2]*rDiag[2],
			mEntry[1][3],
		mEntry[2][0]*rDiag[0], mEntry[2][1]*rDiag[1], mEntry[2][2]*rDiag[2],
			mEntry[2][3]);
}


//----------------------------------------------------------------------------
template <class Real>
Matrix34<Real> Matrix34<Real>::Inverse() const
{
	// Invert a 3x3 using cofactors. This is faster than using a generic
	// Gaussian elimination because of the loop overhead of such a method.

	Matrix34 inverse;

	inverse.mEntry[0][0] = mEntry[1][1]*mEntry[2][2] - 
		mEntry[1][2]*mEntry[2][1];
	inverse.mEntry[0][1] = mEntry[0][2]*mEntry[2][1] - 
		mEntry[0][1]*mEntry[2][2];
	inverse.mEntry[0][2] = mEntry[0][1]*mEntry[1][2] - 
		mEntry[0][2]*mEntry[1][1];
	inverse.mEntry[1][0] = mEntry[1][2]*mEntry[2][0] - 
		mEntry[1][0]*mEntry[2][2];
	inverse.mEntry[1][1] = mEntry[0][0]*mEntry[2][2] - 
		mEntry[0][2]*mEntry[2][0];
	inverse.mEntry[1][2] = mEntry[0][2]*mEntry[1][0] - 
		mEntry[0][0]*mEntry[1][2];
	inverse.mEntry[2][0] = mEntry[1][0]*mEntry[2][1] - 
		mEntry[1][1]*mEntry[2][0];
	inverse.mEntry[2][1] = mEntry[0][1]*mEntry[2][0] - 
		mEntry[0][0]*mEntry[2][1];
	inverse.mEntry[2][2] = mEntry[0][0]*mEntry[1][1] - 
		mEntry[0][1]*mEntry[1][0];

	Real det =
		mEntry[0][0]*inverse.mEntry[0][0] +
		mEntry[0][1]*inverse.mEntry[1][0] +
		mEntry[0][2]*inverse.mEntry[2][0];

	if (Math<Real>::FAbs(det) <= Math<Real>::ZERO_TOLERANCE)
	{
		return ZERO;
	}

	Real invDet = (static_cast<Real>(1.0)) / det;
	inverse.mEntry[0][0] *= invDet;
	inverse.mEntry[0][1] *= invDet;
	inverse.mEntry[0][2] *= invDet;
	inverse.mEntry[1][0] *= invDet;
	inverse.mEntry[1][1] *= invDet;
	inverse.mEntry[1][2] *= invDet;
	inverse.mEntry[2][0] *= invDet;
	inverse.mEntry[2][1] *= invDet;
	inverse.mEntry[2][2] *= invDet;

	return inverse;
}

//----------------------------------------------------------------------------
template <class Real>
inline Vector3<Real> operator* (const Vector3<Real>& rV,
	const Matrix34<Real>& rM)
{
	return Vector3<Real>(
		rV[0]*rM[0][0] + rV[1]*rM[1][0] + rV[2]*rM[2][0],
		rV[0]*rM[0][1] + rV[1]*rM[1][1] + rV[2]*rM[2][1],
		rV[0]*rM[0][2] + rV[1]*rM[1][2] + rV[2]*rM[2][2]);
}
