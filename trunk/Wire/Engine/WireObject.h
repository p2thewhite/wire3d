#pragma once
#ifndef WIREOBJECT_H
#define WIREOBJECT_H

#include "WireSmartPointer.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Object
{
public:
	virtual ~Object();

	void IncrementReferences();
	void DecrementReferences();
	Int GetReferences() const;

protected:
	Object();

private:
	Int mReferences;
};

typedef Pointer<Object> ObjectPtr;
#include "WireObject.inl"

}

#endif
