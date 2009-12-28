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
inline void Renderer::SetCullState(CullState* pState)
{
	mspStates[GlobalState::CULL] = pState;
}

//----------------------------------------------------------------------------
inline CullState* Renderer::GetCullState()
{
	// TODO: use StaticCast rtti
	return static_cast<CullState*>(mspStates[GlobalState::CULL].Get());
}
