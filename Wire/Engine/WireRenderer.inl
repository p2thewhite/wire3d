//----------------------------------------------------------------------------
inline Bool Renderer::OnBeginScene(Camera* pCamera)
{
	mpCamera = pCamera;
	OnFrameChange();
	OnViewportChange();

	// stub for derived classes
	return true;
}

//----------------------------------------------------------------------------
inline Int Renderer::GetHeight() const
{
	return mHeight;
}

//----------------------------------------------------------------------------
inline Int Renderer::GetWidth() const
{
	return mWidth;
}

//----------------------------------------------------------------------------
inline const ColorRGBA& Renderer::GetClearColor() const
{
	return mClearColor;
}

//----------------------------------------------------------------------------
inline AlphaState* Renderer::GetAlphaState()
{
	return StaticCast<AlphaState>(mspStates[GlobalState::ALPHA]);
}

//----------------------------------------------------------------------------
inline CullState* Renderer::GetCullState()
{
	return StaticCast<CullState>(mspStates[GlobalState::CULL]);
}

//----------------------------------------------------------------------------
inline FogState* Renderer::GetFogState()
{
	return StaticCast<FogState>(mspStates[GlobalState::FOG]);
}

//----------------------------------------------------------------------------
inline WireframeState* Renderer::GetWireframeState()
{
	return StaticCast<WireframeState>(mspStates[GlobalState::WIREFRAME]);
}

//----------------------------------------------------------------------------
inline ZBufferState* Renderer::GetZBufferState()
{
	return StaticCast<ZBufferState>(mspStates[GlobalState::ZBUFFER]);
}
