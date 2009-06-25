//-------------------------------------------------------------------------
inline Matrix34f& Camera::GetView()
{
	return mView;
}

//-------------------------------------------------------------------------
inline Float Camera::GetDMin() const
{
	return mFrustum[VF_DMIN];
}

//-------------------------------------------------------------------------
inline Float Camera::GetDMax() const
{
	return mFrustum[VF_DMAX];
}

//-------------------------------------------------------------------------
inline Float Camera::GetUMin() const
{
	return mFrustum[VF_UMIN];
}

//-------------------------------------------------------------------------
inline Float Camera::GetUMax() const
{
	return mFrustum[VF_UMAX];
}

//-------------------------------------------------------------------------
inline Float Camera::GetRMin() const
{
	return mFrustum[VF_RMIN];
}

//-------------------------------------------------------------------------
inline Float Camera::GetRMax() const
{
	return mFrustum[VF_RMAX];
}
