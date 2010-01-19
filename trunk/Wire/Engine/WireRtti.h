#pragma once
#ifndef WIRERTTI_H
#define WIRERTTI_H

#include "../Foundation/WireSystem.h"
#include "WireRttiMCR.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Rtti
{

public:
	Rtti(const Rtti* pBaseType);
	~Rtti();

	Bool IsExactly(const Rtti& rType) const;
	Bool IsDerived(const Rtti& rType) const;

private:
	const Rtti* mpBaseType;
};

#include "WireRtti.inl"

}

#endif
