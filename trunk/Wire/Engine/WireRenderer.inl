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
inline const ColorRGBA& Renderer::GetClearColor() const
{
	return mClearColor;
}
