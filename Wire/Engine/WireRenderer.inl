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
inline UInt Renderer::GetMaxTextureStages() const
{
	return mMaxTextureStages;
}

//----------------------------------------------------------------------------
inline PdrRendererData* Renderer::GetRendererData() const
{
	return mpData;
}

//----------------------------------------------------------------------------
inline const ColorRGBA& Renderer::GetClearColor() const
{
	return mClearColor;
}

//----------------------------------------------------------------------------
inline const AlphaState* Renderer::GetAlphaState() const
{
	return StaticCast<AlphaState>(mspStates[GlobalState::ALPHA]);
}

//----------------------------------------------------------------------------
inline const CullState* Renderer::GetCullState() const
{
	return StaticCast<CullState>(mspStates[GlobalState::CULL]);
}

//----------------------------------------------------------------------------
inline const FogState* Renderer::GetFogState() const
{
	return StaticCast<FogState>(mspStates[GlobalState::FOG]);
}

//----------------------------------------------------------------------------
inline const MaterialState* Renderer::GetMaterialState() const
{
	return StaticCast<MaterialState>(mspStates[GlobalState::MATERIAL]);
}

//----------------------------------------------------------------------------
inline const WireframeState* Renderer::GetWireframeState() const
{
	return StaticCast<WireframeState>(mspStates[GlobalState::WIREFRAME]);
}

//----------------------------------------------------------------------------
inline const ZBufferState* Renderer::GetZBufferState() const
{
	return StaticCast<ZBufferState>(mspStates[GlobalState::ZBUFFER]);
}
