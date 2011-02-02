#pragma once
#ifndef WIRERTTI_H
#define WIRERTTI_H

#include "WireTypes.h"

namespace Wire
{

class Rtti
{

public:
	Rtti(const Char* pName, const Rtti* pBaseType);
	~Rtti();

	inline const Char* GetName () const;
	inline Bool IsExactly(const Rtti& rType) const;
	Bool IsDerived(const Rtti& rType) const;

private:
	const Char* mpName;
	const Rtti* mpBaseType;
};

#include "WireRtti.inl"

}

//----------------------------------------------------------------------------
#define WIRE_DECLARE_RTTI                                                    \
public:                                                                      \
	static const Rtti TYPE;                                                  \
	inline virtual const Rtti& GetType() const { return TYPE; }

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_RTTI(nsName, className, baseClassName)                \
	const Rtti className::TYPE(#nsName"."#className, &baseClassName::TYPE)

//----------------------------------------------------------------------------
#define WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(className, baseClassName)           \
	const Rtti className::TYPE(#className, &baseClassName::TYPE)

#endif
