#include "WireObject.h"

using namespace Wire;

//----------------------------------------------------------------------------
Object::Object()
	:
	mReferences(NULL)
{
}

//----------------------------------------------------------------------------
Object::~Object ()
{
	// This assert traps the case when someone tries 'delete spObject'
	// for a smart-pointer-based object.  Instead, use 'spObject = 0'.
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
