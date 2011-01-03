#include "WireNodeSwitch.h"

#include "WireCuller.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, NodeSwitch, Node);

//----------------------------------------------------------------------------
NodeSwitch::NodeSwitch()
{
    mActiveChild = SN_INVALID_CHILD;
}

//----------------------------------------------------------------------------
NodeSwitch::~NodeSwitch()
{
}

//----------------------------------------------------------------------------
void NodeSwitch::GetVisibleSet(Culler& rCuller, Bool noCull)
{
    if (mActiveChild == SN_INVALID_CHILD)
    {
        return;
    }

	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
    {
        // This is a global effect. Place a 'begin' marker in the visible
        // set to indicate the effect is active.
        rCuller.Insert(this, mEffects[i]);
    }

    Spatial* pChild = mChildren[mActiveChild];
    if (pChild)
    {
        pChild->OnGetVisibleSet(rCuller, noCull);
    }

	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
    {
        // Place an 'end' marker in the visible set to indicate that the
        // global effect is inactive.
        rCuller.Insert(NULL, NULL);
    }
}
