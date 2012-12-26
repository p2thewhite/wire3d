// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderObject.h"

#include "WireBoundingVolume.h"
#include "WireLight.h"
#include "WireMaterial.h"
#include "WireMesh.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, RenderObject, Object);

//----------------------------------------------------------------------------
RenderObject::RenderObject(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer,
	Material* pMaterial)
	:
	mspMaterial(pMaterial),
	mStateSetID(System::MAX_UINT)
{
	mspMesh = WIRE_NEW Mesh(pVBuffer, pIBuffer);
}

//----------------------------------------------------------------------------
RenderObject::RenderObject(Mesh* pMesh, Material* pMaterial)
	:
	mspMesh(pMesh),
	mspMaterial(pMaterial),
	mStateSetID(System::MAX_UINT)
{
	WIRE_ASSERT(pMesh);
}

//----------------------------------------------------------------------------
RenderObject::~RenderObject()
{
}
