// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt Material::GetTextureQuantity() const
{
	return mTextures.GetQuantity();
}

//----------------------------------------------------------------------------
inline Texture2D* Material::GetTexture(UInt i) const
{
	return mTextures[i].Texture;
}

//----------------------------------------------------------------------------
inline Material::BlendMode Material::GetBlendMode(UInt i) const
{
	return mTextures[i].BlendMode;
}

//----------------------------------------------------------------------------
inline void Material::SetBlendMode(BlendMode blendmode, UInt i)
{
	mTextures[i].BlendMode = blendmode;
}
