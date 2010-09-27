#include "WireDLodNode.h"

#include "WireCuller.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(DLodNode, SwitchNode);

//----------------------------------------------------------------------------
DLodNode::DLodNode()
	:
	mModelLodCenter(Vector3F::ZERO)
{
}

//----------------------------------------------------------------------------
DLodNode::~DLodNode()
{
}

//----------------------------------------------------------------------------
void DLodNode::SetLod(UInt i, Spatial* pLod, Float minDist, Float maxDist)
{
	SetChild(i, pLod);
	SetModelDistance(i, minDist, maxDist);
}

//----------------------------------------------------------------------------
void DLodNode::SetModelDistance(UInt i, Float minDist, Float maxDist)
{
    mModelMinDist.SetElement(i, minDist);
    mModelMaxDist.SetElement(i, maxDist);
    mWorldMinDist.SetElement(i, minDist);
    mWorldMaxDist.SetElement(i, maxDist);
}

//----------------------------------------------------------------------------
void DLodNode::SelectLevelOfDetail(const Camera* pCamera)
{
    // ASSERT: The child array of an DLodNode is compacted. There are no
    // empty slots in the array and the number of children is GetQuantity().
    // Moreover, it is assumed that all model distance values were set for
    // these children.
	WIRE_ASSERT(mModelMinDist.GetQuantity() == GetQuantity());

    // compute world LOD center
    mWorldLodCenter = World.ApplyForward(mModelLodCenter);

    // compute world squared distance intervals
    for (UInt i = 0; i < mChildren.GetQuantity(); i++)
    {
        Float value = mModelMinDist[i];
        mWorldMinDist[i] = World.GetUniformScale() * value;

        value = mModelMaxDist[i];
        mWorldMaxDist[i] = World.GetUniformScale() * value;
    }

    // select the LOD child
    SetActiveChild(SN_INVALID_CHILD);
    if (mChildren.GetQuantity() > 0)
    {
        Vector3F diff = mWorldLodCenter - pCamera->GetLocation();
        Float dist = diff.Length();

        for (UInt i = 0; i < mChildren.GetQuantity(); i++) 
        {
            if (mWorldMinDist[i] <= dist && dist < mWorldMaxDist[i])
            {
                SetActiveChild(i);
                break;
            }
        }
    }
}

//----------------------------------------------------------------------------
void DLodNode::GetVisibleSet(Culler& rCuller, Bool noCull)
{
    SelectLevelOfDetail(rCuller.GetCamera());
    SwitchNode::GetVisibleSet(rCuller, noCull);
}
