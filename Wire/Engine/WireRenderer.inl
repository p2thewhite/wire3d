//----------------------------------------------------------------------------
inline Bool Renderer::BeginScene(Camera* pCamera)
{
	mpCamera = pCamera;
	OnFrameChange();

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
inline void Renderer::SetAlphaState(AlphaState* pState)
{
	mspStates[GlobalState::ALPHA] = pState;
}

//----------------------------------------------------------------------------
inline void Renderer::SetCullState(CullState* pState)
{
	mspStates[GlobalState::CULL] = pState;
}

//----------------------------------------------------------------------------
inline void Renderer::SetFogState(FogState* pState)
{
	mspStates[GlobalState::FOG] = pState;
}

//----------------------------------------------------------------------------
inline void Renderer::SetWireframeState(WireframeState* pState)
{
	mspStates[GlobalState::WIREFRAME] = pState;
}

//----------------------------------------------------------------------------
inline void Renderer::SetZBufferState(ZBufferState* pState)
{
	mspStates[GlobalState::ZBUFFER] = pState;
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
