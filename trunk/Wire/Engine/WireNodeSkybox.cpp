// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNodeSkybox.h"

#include "WireCuller.h"
#include "WireGeometry.h"
#include "WireMesh.h"
#include "WireStateCull.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, NodeSkybox, Node);

//----------------------------------------------------------------------------
NodeSkybox::NodeSkybox(Texture2D* pPosZ, Texture2D* pNegZ, Texture2D* pPosX,
	Texture2D* pNegX, Texture2D* pPosY, Texture2D* pNegY, Float scale)
{
	Init(pPosZ, pNegZ, pPosX, pNegX, pPosY, pNegY, scale);
}

//----------------------------------------------------------------------------
NodeSkybox::~NodeSkybox()
{
}

//----------------------------------------------------------------------------
void NodeSkybox::UpdateWorldData(Double appTime)
{
	mAppTime = appTime;
}

//----------------------------------------------------------------------------
void NodeSkybox::GetVisibleSet(Culler& rCuller, Bool noCull)
{
	const Camera* const pCamera = rCuller.GetCamera();
	World.SetTranslate(pCamera->GetLocation());

	Node::UpdateWorldData(mAppTime);
	Node::GetVisibleSet(rCuller, noCull);
}

//----------------------------------------------------------------------------
void NodeSkybox::Init(Texture2D* pPosZ, Texture2D* pNegZ, Texture2D* pPosX,
	Texture2D* pNegX, Texture2D* pPosY, Texture2D* pNegY, Float scale)
{
	mAppTime = 0;
	Culling = CULL_NEVER;
	WorldIsCurrent = true;
	StateCull* pCull = WIRE_NEW StateCull;
	pCull->CullFace = StateCull::CM_BACK;
	AttachState(pCull);

	// +Z (front)
	AddQuad(Vector3F(1.0F, 1.0F, 1.0F), Vector3F(-1.0F, 1.0F, 1.0F),
			Vector3F(-1.0F, -1.0F, 1.0F), Vector3F(1.0F, -1.0F, 1.0F),
			pPosZ, scale);

	// -Z (back)
	AddQuad(Vector3F(-1.0F, 1.0F, -1.0F), Vector3F(1.0F, 1.0F,-1.0F),
			Vector3F(1.0F, -1.0F, -1.0F), Vector3F(-1.0F, -1.0F, -1.0F),
			pNegZ, scale);

	// +X (left)
	AddQuad(Vector3F(1.0F, 1.0F, -1.0F), Vector3F(1.0F, 1.0F, 1.0F),
			Vector3F(1.0F, -1.0F, 1.0F), Vector3F(1.0F, -1.0F, -1.0F),
			pPosX, scale);

	// -X (right)
	AddQuad(Vector3F(-1.0F, 1.0F, 1.0F), Vector3F(-1.0F, 1.0F, -1.0F),
			Vector3F(-1.0F, -1.0F, -1.0F), Vector3F(-1.0F, -1.0F, 1.0F),
			pNegX, scale);

	// +Y (up)
	AddQuad(Vector3F(1.0F, 1.0F, -1.0F), Vector3F(-1.0F, 1.0F, -1.0F),
			Vector3F(-1.0F, 1.0F, 1.0F), Vector3F(1.0F, 1.0F, 1.0F),
			pPosY, scale);

	// -Y (down)
	AddQuad(Vector3F(1.0F, -1.0F, 1.0F), Vector3F(-1.0F, -1.0F, 1.0F),
			Vector3F(-1.0F, -1.0F, -1.0F), Vector3F(1.0F, -1.0F, -1.0F),
			pNegY, scale);
}

//----------------------------------------------------------------------------
void NodeSkybox::AddQuad(const Vector3F& v0, const Vector3F& v1, const
	Vector3F& v2, const Vector3F& v3, Texture2D* pTexture, const Float scale)
{
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetTCoordChannels(2);	// channels: U, V

	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes, 4);
	pVBuffer->Position3(0) = v0 * scale;
	pVBuffer->TCoord2(0) = Vector2F(0, 0);
	pVBuffer->Position3(1) = v1 * scale;
	pVBuffer->TCoord2(1) = Vector2F(1, 0);
	pVBuffer->Position3(2) = v2 * scale;
	pVBuffer->TCoord2(2) = Vector2F(1, 1);
	pVBuffer->Position3(3) = v3 * scale;
	pVBuffer->TCoord2(3) = Vector2F(0, 1);

	const UShort indices[6] = { 0, 1, 2, 0, 2, 3 };
	UInt indexQuantity = sizeof(indices) / sizeof(UShort);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIBuffer)[i] = indices[i];
	}

	Geometry* pQuad = WIRE_NEW Geometry(pVBuffer, pIBuffer);
	Material* pMaterial = WIRE_NEW Material;
	pMaterial->AddTexture(pTexture, Material::BM_REPLACE);
	pQuad->SetMaterial(pMaterial);
	AttachChild(pQuad);
}
