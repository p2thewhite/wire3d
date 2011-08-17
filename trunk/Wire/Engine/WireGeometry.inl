// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline VertexBuffer* Geometry::GetVBuffer()
{
	return mspVBuffer;
}

//----------------------------------------------------------------------------
inline const VertexBuffer* Geometry::GetVBuffer() const
{
	return mspVBuffer;
}

//----------------------------------------------------------------------------
inline IndexBuffer* Geometry::GetIBuffer()
{
	return mspIBuffer;
}

//----------------------------------------------------------------------------
inline const IndexBuffer* Geometry::GetIBuffer() const
{
	return mspIBuffer;
}

//----------------------------------------------------------------------------
inline Material* Geometry::GetMaterial()
{
	return mspMaterial;
}

//----------------------------------------------------------------------------
inline const Material* Geometry::GetMaterial() const
{
	return mspMaterial;
}

//----------------------------------------------------------------------------
inline void Geometry::SetMaterial(Material* pMaterial)
{
	mspMaterial = pMaterial;
}

//----------------------------------------------------------------------------
inline BoundingVolume* Geometry::GetModelBound()
{
	return mspModelBound;
}

//----------------------------------------------------------------------------
inline const BoundingVolume* Geometry::GetModelBound() const
{
	return mspModelBound;
}
