#pragma once
#ifndef WIRERTTI_H
#define WIRERTTI_H

#include "WireRttiMCR.h"
#include "WireSystem.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Rtti
{

public:
	Rtti(const Rtti* pBaseType);
	~Rtti();

	inline Bool IsExactly(const Rtti& rType) const;
	Bool IsDerived(const Rtti& rType) const;

private:
	const Rtti* mpBaseType;
};

#include "WireRtti.inl"

}

#endif
