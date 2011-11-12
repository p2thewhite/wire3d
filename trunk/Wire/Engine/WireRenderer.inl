// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

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
	return mTexture2Ds.GetQuantity();
}

//----------------------------------------------------------------------------
inline UInt Renderer::GetMaxLights() const
{
	return mLights.GetQuantity();
}

//----------------------------------------------------------------------------
inline const ColorRGBA& Renderer::GetClearColor() const
{
	return mClearColor;
}

//----------------------------------------------------------------------------
inline const StateAlpha* Renderer::GetStateAlpha() const
{
	return StaticCast<StateAlpha>(mspStates[State::ALPHA]);
}

//----------------------------------------------------------------------------
inline const StateCull* Renderer::GetStateCull() const
{
	return StaticCast<StateCull>(mspStates[State::CULL]);
}

//----------------------------------------------------------------------------
inline const StateFog* Renderer::GetStateFog() const
{
	return StaticCast<StateFog>(mspStates[State::FOG]);
}

//----------------------------------------------------------------------------
inline const StateMaterial* Renderer::GetStateMaterial() const
{
	return StaticCast<StateMaterial>(mspStates[State::MATERIAL]);
}

//----------------------------------------------------------------------------
inline const StateWireframe* Renderer::GetStateWireframe() const
{
	return StaticCast<StateWireframe>(mspStates[State::WIREFRAME]);
}

//----------------------------------------------------------------------------
inline const StateZBuffer* Renderer::GetStateZBuffer() const
{
	return StaticCast<StateZBuffer>(mspStates[State::ZBUFFER]);
}

//----------------------------------------------------------------------------
inline const Renderer::Statistics* Renderer::GetStatistics()
{
	return s_pRenderer ? &s_pRenderer->mStatistics : NULL;
}

//----------------------------------------------------------------------------
inline PdrRendererData* Renderer::GetRendererData() const
{
	return mpData;
}
