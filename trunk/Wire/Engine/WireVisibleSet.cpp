#include "WireVisibleSet.h"

using namespace Wire;

//----------------------------------------------------------------------------
VisibleSet::VisibleSet(UInt maxQuantity, UInt growBy)
{
	mVisible.SetGrowBy(growBy);
	mVisible.SetMaxQuantity(maxQuantity, false);
}

//----------------------------------------------------------------------------
VisibleSet::~VisibleSet()
{
}

//----------------------------------------------------------------------------
void VisibleSet::Insert(Spatial* pObject/*, Effect* pGlobalEffect*/)
{
	VisibleObject visible;
	visible.Object = pObject;
	mVisible.Append(visible);
}
