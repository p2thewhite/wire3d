//----------------------------------------------------------------------------
inline void Culler::SetCamera(const Camera* pCamera)
{
	mpCamera = pCamera;
}
//----------------------------------------------------------------------------
inline const Camera* Culler::GetCamera() const
{
	return mpCamera;
}
