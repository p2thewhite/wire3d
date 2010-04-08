//----------------------------------------------------------------------------
inline Bool Renderer::BeginScene(Camera* pCamera)
{
	mpCamera = pCamera;
	OnFrameChange();
	OnViewportChange();

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

//----------------------------------------------------------------------------
inline UShort Renderer::RGB888ToRGB565(UChar* pRGB888)
{
	UShort rgb565 = static_cast<UShort>(*pRGB888++);		// red
	rgb565 &= 0xF8;
	rgb565 = rgb565 << 8;

	UShort component = static_cast<UShort>(*pRGB888++);		// green
	component &= 0xFC;
	rgb565 |= component << 3;

	component = static_cast<UShort>(*pRGB888);				// blue
	component &= 0xF8;
	rgb565 |= component >> 3;

	return rgb565;
}

//----------------------------------------------------------------------------
inline UShort Renderer::RGBA8888ToRGBA4444(UChar* pRGBA8888)
{
	UShort rgba4444 = static_cast<UShort>(*pRGBA8888++);		// red
	rgba4444 &= 0xF0;
	rgba4444 = rgba4444 << 8;

	UShort component = static_cast<UShort>(*pRGBA8888++);		// green
	component &= 0xF0;
	rgba4444 |= component << 4;

	component = static_cast<UShort>(*pRGBA8888++);				// blue
	component &= 0xF0;
	rgba4444 |= component;

	component = static_cast<UShort>(*pRGBA8888);				// alpha
	rgba4444 |= component >> 4;

	return rgba4444;
}
