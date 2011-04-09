#include "WireObject.h"

using namespace Wire;

const Rtti Object::TYPE("Wire.Object", NULL);

//----------------------------------------------------------------------------
Object::Object()
	:
	mReferences(0)
{
}

//----------------------------------------------------------------------------
Object::~Object ()
{
	// This assert traps the case when someone tries 'delete spObject'
	// for a smart-pointer-based object. Instead, use 'spObject = NULL'.
	WIRE_ASSERT(mReferences == 0);
}

//----------------------------------------------------------------------------
void Object::DecrementReferences()
{
	if (--mReferences == 0)
	{
		WIRE_DELETE this;
	}
}