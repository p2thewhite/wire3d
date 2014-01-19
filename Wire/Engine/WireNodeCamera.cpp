// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2014. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNodeCamera.h"

#include "WireCamera.h"
#include "WireCuller.h"
#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, NodeCamera, Node);

//----------------------------------------------------------------------------
NodeCamera::NodeCamera(Camera* pCamera)
	:
	mspCamera(pCamera),
	mDepth(0),
	mEnabled(true)
{
	CameraToLocalTransform();
}

//----------------------------------------------------------------------------
NodeCamera::~NodeCamera()
{
}

//----------------------------------------------------------------------------
void NodeCamera::Set(Camera* pCamera)
{
	mspCamera = pCamera;

	CameraToLocalTransform();
}

//----------------------------------------------------------------------------
void NodeCamera::UpdateWorldData(Double appTime, Bool updateControllers)
{
	Node::UpdateWorldData(appTime, updateControllers);

	if (mspCamera && mEnabled)
	{
		mspCamera->SetFrame(World.GetTranslate(),
			World.GetRotate().GetColumn(0),
			World.GetRotate().GetColumn(1),
			World.GetRotate().GetColumn(2));
	}
}

//----------------------------------------------------------------------------
void NodeCamera::CameraToLocalTransform()
{
	if (mspCamera)
	{
		Matrix34F matrix(mspCamera->GetDVector(), mspCamera->GetUVector(),
			mspCamera->GetRVector(), mspCamera->GetLocation());
		Local.SetMatrix(matrix, false);
	}
}

//----------------------------------------------------------------------------
void NodeCamera::Draw(TArray<NodeCamera*>& rCameras, Spatial* pRoot,
	Culler& rCuller, Renderer* pRenderer)
{
	WIRE_ASSERT(pRenderer && pRoot);
	if (!pRenderer || !pRoot)
	{
		return;
	}

	for (UInt i = 0; i < rCameras.GetQuantity(); i++)
	{
		rCuller.SetCamera(rCameras[i]->Get());
		rCuller.ComputeVisibleSet(pRoot);
		pRenderer->SetCamera(rCameras[i]->Get());
		pRenderer->Draw(rCuller.GetVisibleSets());
	}
}
