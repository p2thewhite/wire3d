#pragma once
#ifndef WIREVISIBLESET_H
#define WIREVISIBLESET_H

#include "WireVisibleObject.h"
#include "WireTArray.h"

namespace Wire
{

class Spatial;

class /*WIRE_ENGINE_ITEM*/ VisibleSet
{

public:
	enum
	{
		VS_DEFAULT_MAX_QUANTITY = 32,
		VS_DEFAULT_GROWBY = 32,
	};

	VisibleSet(UInt maxQuantity = VS_DEFAULT_MAX_QUANTITY, UInt growBy =
		VS_DEFAULT_GROWBY);
	virtual ~VisibleSet ();

	inline UInt GetQuantity() const;
	inline VisibleObject* GetVisible();
	inline VisibleObject& GetVisible(UInt i);

	// Creates a VisibleObject from the input and appends it to the end of the
	// VisibleObject array.
	void Insert(Spatial* pkObject, Effect* pGlobalEffect);
	inline void Clear();

private:
	TArray<VisibleObject> mVisible;
};

#include "WireVisibleSet.inl"

}

#endif
