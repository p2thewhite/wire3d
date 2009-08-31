//----------------------------------------------------------------------------
inline Vector3F Camera::GetLocation() const
{
	return mLocation;
}

//----------------------------------------------------------------------------
inline Vector3F Camera::GetDVector() const
{
	return mDVector;
}

//----------------------------------------------------------------------------
inline Vector3F Camera::GetUVector() const
{
	return mUVector;
}

//----------------------------------------------------------------------------
inline Vector3F Camera::GetRVector() const
{
	return mRVector;
}

//----------------------------------------------------------------------------
inline Matrix34F& Camera::GetView()
{
	return mView;
}

//----------------------------------------------------------------------------
inline Float Camera::GetDMin() const
{
	return mFrustum[VF_DMIN];
}

//----------------------------------------------------------------------------
inline Float Camera::GetDMax() const
{
	return mFrustum[VF_DMAX];
}

//----------------------------------------------------------------------------
inline Float Camera::GetUMin() const
{
	return mFrustum[VF_UMIN];
}

//----------------------------------------------------------------------------
inline Float Camera::GetUMax() const
{
	return mFrustum[VF_UMAX];
}

//----------------------------------------------------------------------------
inline Float Camera::GetRMin() const
{
	return mFrustum[VF_RMIN];
}

//----------------------------------------------------------------------------
inline Float Camera::GetRMax() const
{
	return mFrustum[VF_RMAX];
}

//----------------------------------------------------------------------------
inline const Float* Camera::GetFrustum() const
{
	return mFrustum;
}
