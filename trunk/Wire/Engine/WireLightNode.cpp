#include "WireLightNode.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(LightNode, Node);

//----------------------------------------------------------------------------
LightNode::LightNode(Light* pLight)
	:
	mspLight(pLight)
{
	LightToLocalTransform();
}

//----------------------------------------------------------------------------
LightNode::~LightNode()
{
}

//----------------------------------------------------------------------------
void LightNode::SetLight(Light* pLight)
{
	mspLight = pLight;

	LightToLocalTransform();
	if (mspLight)
	{
		UpdateGS();
	}
}

//----------------------------------------------------------------------------
void LightNode::UpdateWorldData(double appTime)
{
	Node::UpdateWorldData(appTime);

	if (mspLight)
	{
		mspLight->Position = World.GetTranslate();
		mspLight->Direction = World.GetMatrix().GetColumn(0);
	}
}

//----------------------------------------------------------------------------
void LightNode::LightToLocalTransform()
{
	if (!mspLight)
	{
		return;
	}

	Local.SetTranslate(mspLight->Position);
	Vector3F u;
	Vector3F v;
	Vector3F::GenerateOrthonormalBasis(u, v, mspLight->Direction);
	Local.SetRotate(Matrix34F(mspLight->Direction, u, v, true));
}
