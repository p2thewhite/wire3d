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
inline const StateAlpha* Renderer::GetAlphaState() const
{
	return StaticCast<StateAlpha>(mspStates[StateGlobal::ALPHA]);
}

//----------------------------------------------------------------------------
inline const StateCull* Renderer::GetCullState() const
{
	return StaticCast<StateCull>(mspStates[StateGlobal::CULL]);
}

//----------------------------------------------------------------------------
inline const StateFog* Renderer::GetFogState() const
{
	return StaticCast<StateFog>(mspStates[StateGlobal::FOG]);
}

//----------------------------------------------------------------------------
inline const StateMaterial* Renderer::GetMaterialState() const
{
	return StaticCast<StateMaterial>(mspStates[StateGlobal::MATERIAL]);
}

//----------------------------------------------------------------------------
inline const StateWireframe* Renderer::GetWireframeState() const
{
	return StaticCast<StateWireframe>(mspStates[StateGlobal::WIREFRAME]);
}

//----------------------------------------------------------------------------
inline const StateZBuffer* Renderer::GetZBufferState() const
{
	return StaticCast<StateZBuffer>(mspStates[StateGlobal::ZBUFFER]);
}
