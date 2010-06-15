//----------------------------------------------------------------------------
inline UInt Renderer::GetHeight() const
{
	return mHeight;
}

//----------------------------------------------------------------------------
inline UInt Renderer::GetWidth() const
{
	return mWidth;
}

//----------------------------------------------------------------------------
inline Float Renderer::GetMaxAnisotropy() const
{
	return mMaxAnisotropy;
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

//----------------------------------------------------------------------------
inline PdrRendererData* Renderer::GetRendererData()
{
	return mpData;
}
