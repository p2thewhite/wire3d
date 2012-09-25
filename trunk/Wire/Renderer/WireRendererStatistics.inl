// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetDrawCalls() const
{
	return mDrawCalls;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetBatchedStatic() const
{
	return mBatchedStatic;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetBatchedDynamic() const
{
	return mBatchedDynamic;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetTriangles() const
{
	return mTriangles;
}

// number of buffers and their total size bound to the Renderer
//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetVBOCount() const
{
	return mVBOCount;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetVBOTotalSize() const
{
	return mVBOTotalSize;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetIBOCount() const
{
	return mIBOCount;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetIBOTotalSize() const
{
	return mIBOTotalSize;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetTextureCount() const
{
	return mTextureCount;
}

//----------------------------------------------------------------------------
inline UInt RendererStatistics::GetTextureTotalSize() const
{
	return mTextureTotalSize;
}
