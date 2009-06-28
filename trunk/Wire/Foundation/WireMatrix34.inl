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

//----------------------------------------------------------------------------
template <class Real>
Matrix34<Real>& Matrix34<Real>::MakeIdentity()
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
	return *this;
}

//----------------------------------------------------------------------------
template <class Real>
Matrix34<Real>& Matrix34<Real>::FromAxisAngle(const Vector3<Real>& rAxis,
	Real angle)
{
	Vector3<Real> axis = rAxis;
	Vector3<Real> u = axis / axis.Length();
	Real c = Math<Real>::Cos(angle);
	Real s = Math<Real>::Sin(angle);
	Real t = (static_cast<Real>(1.0)) - c;

	Real tuxuy = t*u.X()*u.Y();
	Real tuxuz = t*u.X()*u.Z();
	Real tuyuz = t*u.Y()*u.Z();

	mEntry[0][0] = t*u.X()*u.X() + c;
	mEntry[0][1] = tuxuy - s*u.Z();
	mEntry[0][2] = tuxuz + s*u.Y();
	mEntry[0][3] = static_cast<Real>(0.0);
	
	mEntry[1][0] = tuxuy + s*u.Z();
	mEntry[1][1] = t*u.Y()*u.Y() + c;
	mEntry[1][2] = tuyuz - s*u.X();
	mEntry[1][3] = static_cast<Real>(0.0);

	mEntry[2][0] = tuxuz - s*u.Y();
	mEntry[2][1] = tuyuz + s*u.X();
	mEntry[2][2] = t*u.Z()*u.Z() + c;
	mEntry[2][3] = static_cast<Real>(0.0);

// alternative approach without Length/Sqrt()
// 	Real cos = Math<Real>::Cos(angle);
// 	Real sin = Math<Real>::Sin(angle);
// 	Real oneMinusCos = (static_cast<Real>(1.0)) - cos;
// 	Real x2 = rAxis[0] * rAxis[0];
// 	Real y2 = rAxis[1] * rAxis[1];
// 	Real z2 = rAxis[2] * rAxis[2];
// 	Real xyM = rAxis[0] * rAxis[1] * oneMinusCos;
// 	Real xzM = rAxis[0] * rAxis[2] * oneMinusCos;
// 	Real yzM = rAxis[1] * rAxis[2] * oneMinusCos;
// 	Real xSin = rAxis[0] * sin;
// 	Real ySin = rAxis[1] * sin;
// 	Real zSin = rAxis[2] * sin;
// 
// 	mEntry[0][0] = x2 * oneMinusCos + cos;
// 	mEntry[0][1] = xyM - zSin;
// 	mEntry[0][2] = xzM + ySin;
// 	mEntry[0][3] = static_cast<Real>(0.0);
// 
// 	mEntry[1][0] = xyM + zSin;
// 	mEntry[1][1] = y2 * oneMinusCos + cos;
// 	mEntry[1][2] = yzM - xSin;
// 	mEntry[1][3] = static_cast<Real>(0.0);
// 
// 	mEntry[2][0] = xzM - ySin;
// 	mEntry[2][1] = yzM + xSin;
// 	mEntry[2][2] = z2 * oneMinusCos + cos;
// 	mEntry[2][3] = static_cast<Real>(0.0);
// 
	return *this;
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real> Matrix34<Real>::operator* (Matrix34& rMatrix)
{
	Real4* a = mEntry;
	Real4* b = rMatrix.mEntry;

	return Matrix34<Real>(
		a[0][0]*b[0][0] + a[0][1]*b[1][0] +	a[0][2]*b[2][0],
		a[0][0]*b[0][1] + a[0][1]*b[1][1] +	a[0][2]*b[2][1],
		a[0][0]*b[0][2] + a[0][1]*b[1][2] +	a[0][2]*b[2][2],
		a[0][0]*b[0][3] + a[0][1]*b[1][3] +	a[0][2]*b[2][3] + a[0][3],

		a[1][0]*b[0][0] + a[1][1]*b[1][0] +	a[1][2]*b[2][0],
		a[1][0]*b[0][1] + a[1][1]*b[1][1] +	a[1][2]*b[2][1],
		a[1][0]*b[0][2] + a[1][1]*b[1][2] +	a[1][2]*b[2][2],
		a[1][0]*b[0][3] + a[1][1]*b[1][3] +	a[1][2]*b[2][3] + a[1][3],

		a[2][0]*b[0][0] + a[2][1]*b[1][0] +	a[2][2]*b[2][0],
		a[2][0]*b[0][1] + a[2][1]*b[1][1] +	a[2][2]*b[2][1],
		a[2][0]*b[0][2] + a[2][1]*b[1][2] +	a[2][2]*b[2][2],
		a[2][0]*b[0][3] + a[2][1]*b[1][3] +	a[2][2]*b[2][3] + a[2][3]
		);
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real>::operator Real4* ()
{
	return mEntry;
}

//----------------------------------------------------------------------------
template <class Real>
inline Matrix34<Real>::operator const Real4* () const
{
	return mEntry;
}

//----------------------------------------------------------------------------
template <class Real>
void Matrix34<Real>::SetColumn(Int col, const Vector3<Real>& rVector)
{
	WIRE_ASSERT((0 =< col) && (col < 4));
	mEntry[0][col] = rVector.X();
	mEntry[1][col] = rVector.Y();
	mEntry[2][col] = rVector.Z();
}

//----------------------------------------------------------------------------
template <class Real>
Vector3<Real> Matrix34<Real>::GetColumn(Int col) const
{
	return Vector3<Real>(mEntry[0][col], mEntry[1][col], mEntry[2][col]);
}
