//----------------------------------------------------------------------------
inline Bool Renderer::BeginScene(Camera* pCamera)
{
	mpCamera = pCamera;

	// stub for derived classes
	return true;
}

//----------------------------------------------------------------------------
inline void Renderer::EndScene()
{
	// stub for derived classes
}

//----------------------------------------------------------------------------
inline Int Renderer::GetHeight() const
{
	// stub for derived classes
	return 480;
}

//----------------------------------------------------------------------------
inline Int Renderer::GetWidth() const
{
	// stub for derived classes
	return 640;
}

//----------------------------------------------------------------------------
inline const ColorRGBA& Renderer::GetClearColor() const
{
	return mClearColor;
}
