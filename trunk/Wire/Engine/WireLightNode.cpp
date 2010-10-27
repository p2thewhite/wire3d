#include "WireLightNode.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(LightNode, Light);

//----------------------------------------------------------------------------
LightNode::LightNode(Light* pLight)
	:
	mspLight(pLight)
{
	if (mspLight)
	{
		Local.SetTranslate(mspLight->Position);
	}
}

//----------------------------------------------------------------------------
LightNode::~LightNode()
{
}

//----------------------------------------------------------------------------
void LightNode::SetLight(Light* pLight)
{
	mspLight = pLight;

	if (mspLight)
	{
		Local.SetTranslate(mspLight->Position);
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
	}
}
