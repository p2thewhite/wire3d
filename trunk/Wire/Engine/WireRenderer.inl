// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
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
inline UInt Renderer::GetMaxTextureWidth() const
{
	return mMaxTextureWidth;
}

//----------------------------------------------------------------------------
inline UInt Renderer::GetMaxTextureHeight() const
{
	return mMaxTextureHeight;
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
inline Bool Renderer::UsesBatching() const
{
	return mStaticBatchingThreshold > 0 || mDynamicBatchingThreshold > 0;
}

//----------------------------------------------------------------------------
inline UInt Renderer::GetStaticBatchingThreshold() const
{
	return mStaticBatchingThreshold;
}

//----------------------------------------------------------------------------
inline Bool Renderer::UsesStaticBatching() const
{
	return mStaticBatchingThreshold > 0;
}

//----------------------------------------------------------------------------
inline UInt Renderer::GetDynamicBatchingThreshold() const
{
	return mDynamicBatchingThreshold;
}

//----------------------------------------------------------------------------
inline Bool Renderer::UsesDynamicBatching() const
{
	return mDynamicBatchingThreshold > 0;
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
