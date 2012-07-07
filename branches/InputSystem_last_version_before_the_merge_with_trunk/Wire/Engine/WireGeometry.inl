// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline Mesh* Geometry::GetMesh()
{
	return mspMesh;
}

//----------------------------------------------------------------------------
inline const Mesh* Geometry::GetMesh() const
{
	return mspMesh;
}

//----------------------------------------------------------------------------
inline void Geometry::SetMesh(Mesh* pMesh)
{
	mspMesh = pMesh;
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
