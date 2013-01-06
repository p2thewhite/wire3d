// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline Geometry::operator RenderObject* ()
{
	return mspRenderObject;
}

//----------------------------------------------------------------------------
inline Geometry::operator const RenderObject* () const
{
	return mspRenderObject;
}

//----------------------------------------------------------------------------
inline RenderObject* Geometry::GetRenderObject()
{
	return mspRenderObject;
}

//----------------------------------------------------------------------------
inline const RenderObject* Geometry::GetRenderObject() const
{
	return mspRenderObject;
}

//----------------------------------------------------------------------------
inline Mesh* Geometry::GetMesh()
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetMesh();
}

//----------------------------------------------------------------------------
inline const Mesh* Geometry::GetMesh() const
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetMesh();
}

//----------------------------------------------------------------------------
inline void Geometry::SetMesh(Mesh* pMesh)
{
	WIRE_ASSERT(mspRenderObject);
	mspRenderObject->SetMesh(pMesh);
}

//----------------------------------------------------------------------------
inline Material* Geometry::GetMaterial()
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetMaterial();
}

//----------------------------------------------------------------------------
inline const Material* Geometry::GetMaterial() const
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetMaterial();
}

//----------------------------------------------------------------------------
inline void Geometry::SetMaterial(Material* pMaterial)
{
	WIRE_ASSERT(mspRenderObject);
	mspRenderObject->SetMaterial(pMaterial);
}

//----------------------------------------------------------------------------
inline TArray<Pointer<Light> >& Geometry::GetLights()
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetLights();
}

//----------------------------------------------------------------------------
inline const TArray<Pointer<Light> >& Geometry::GetLights() const
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetLights();
}

//----------------------------------------------------------------------------
inline StatePtr* Geometry::GetStates()
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetStates();
}

//----------------------------------------------------------------------------
inline const StatePtr* Geometry::GetStates() const
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetStates();
}

//----------------------------------------------------------------------------
inline UInt Geometry::GetStateSetID() const
{
	WIRE_ASSERT(mspRenderObject);
	return mspRenderObject->GetStateSetID();
}
