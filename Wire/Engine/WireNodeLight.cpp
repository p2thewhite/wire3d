#include "WireNodeLight.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, NodeLight, Node);

//----------------------------------------------------------------------------
NodeLight::NodeLight(Light* pLight)
	:
	mspLight(pLight)
{
	LightToLocalTransform();
}

//----------------------------------------------------------------------------
NodeLight::~NodeLight()
{
}

//----------------------------------------------------------------------------
void NodeLight::SetLight(Light* pLight)
{
	mspLight = pLight;

	LightToLocalTransform();
	if (mspLight)
	{
		UpdateGS();
	}
}

//----------------------------------------------------------------------------
void NodeLight::UpdateWorldData(Double appTime)
{
	Node::UpdateWorldData(appTime);

	if (mspLight)
	{
		mspLight->Position = World.GetTranslate();
		mspLight->Direction = World.GetMatrix().GetColumn(0);
	}
}

//----------------------------------------------------------------------------
void NodeLight::LightToLocalTransform()
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
