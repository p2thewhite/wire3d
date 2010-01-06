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
	// TODO: use StaticCast rtti
	return static_cast<AlphaState*>(mspStates[GlobalState::ALPHA].Get());
}

//----------------------------------------------------------------------------
inline CullState* Renderer::GetCullState()
{
	return static_cast<CullState*>(mspStates[GlobalState::CULL].Get());
}

//----------------------------------------------------------------------------
inline FogState* Renderer::GetFogState()
{
	return static_cast<FogState*>(mspStates[GlobalState::FOG].Get());
}

//----------------------------------------------------------------------------
inline WireframeState* Renderer::GetWireframeState()
{
	return static_cast<WireframeState*>(
		mspStates[GlobalState::WIREFRAME].Get());
}

//----------------------------------------------------------------------------
inline ZBufferState* Renderer::GetZBufferState()
{
	return static_cast<ZBufferState*>(mspStates[GlobalState::ZBUFFER].Get());
}
