// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt RenderTarget::GetTargetQuantity() const
{
	return mTargetQuantity;
}

//----------------------------------------------------------------------------
inline Image2D::FormatMode RenderTarget::GetFormat() const
{
	return mFormat;
}

//----------------------------------------------------------------------------
inline UInt RenderTarget::GetWidth() const
{
	return mWidth;
}

//----------------------------------------------------------------------------
inline UInt RenderTarget::GetHeight() const
{
	return mHeight;
}

//----------------------------------------------------------------------------
inline Bool RenderTarget::HasMipmaps() const
{
	return mHasMipmaps;
}

//----------------------------------------------------------------------------
inline Bool RenderTarget::HasDepthStencil() const
{
	return mHasDepthStencil;
}
