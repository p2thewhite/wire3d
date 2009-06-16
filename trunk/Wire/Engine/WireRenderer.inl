//-------------------------------------------------------------------------
inline Bool Renderer::BeginScene()
{
	// stub for derived classes
	return true;
}

//-------------------------------------------------------------------------
inline void Renderer::EndScene()
{
	// stub for derived classes
}

//-------------------------------------------------------------------------
inline Int Renderer::GetHeight() const
{
	// stub for derived classes
	return 0;
}

//-------------------------------------------------------------------------
inline Int Renderer::GetWidth() const
{
	// stub for derived classes
	return 0;
}

//-------------------------------------------------------------------------
inline const ColorRGBA& Renderer::GetClearColor() const
{
	return mClearColor;
}
