// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline Camera* Renderer::GetCamera() const
{
	return mspCamera;
}

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
inline UInt Renderer::GetMaxVertexStreams() const
{
	return mVertexBuffers.GetQuantity();
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
inline Bool Renderer::SupportsBatching() const
{
	return mSupportsBatching;
}

//----------------------------------------------------------------------------
inline Bool Renderer::UsesBatching() const
{
	Bool usesStaticBatching = mStaticBatchingMaxIndexCount > 0;
	Bool usesDynamicBatching = (mDynamicBatchingMaxVertexCount > 0) &&
		(mDynamicBatchingMaxIndexCount > 0);
	return mBatchedIndexBuffer && (usesStaticBatching || 
		(usesDynamicBatching && mBatchedVertexBuffers.GetQuantity() > 0));
}

//----------------------------------------------------------------------------
inline UInt Renderer::GetStaticBatchingThreshold() const
{
	return mStaticBatchingMaxIndexCount / 3;
}

//----------------------------------------------------------------------------
inline void Renderer::SetStaticBatchingThreshold(UInt maxTriangles)
{
	if (mSupportsBatching)
	{
		mStaticBatchingMaxIndexCount = maxTriangles * 3;
	}
}

//----------------------------------------------------------------------------
inline void Renderer::GetVertexBatchingThreshold(UInt& rMaxVertices,
	UInt& rMaxTriangles) const
{
	rMaxVertices = mDynamicBatchingMaxVertexCount;
	rMaxTriangles = mDynamicBatchingMaxIndexCount / 3;
}

//----------------------------------------------------------------------------
inline void Renderer::SetDynamicBatchingThreshold(UInt maxVertices,
	UInt maxTriangles)
{
	if (mSupportsBatching)
	{
		mDynamicBatchingMaxVertexCount = maxVertices;
		mDynamicBatchingMaxIndexCount = maxTriangles * 3;
	}
}

//----------------------------------------------------------------------------
inline RendererStatistics* Renderer::GetStatistics()
{
	return &mStatistics;
}

//----------------------------------------------------------------------------
inline PdrRendererData* Renderer::GetRendererData() const
{
	return mpData;
}
