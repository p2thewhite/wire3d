#pragma once
#ifndef WIREOBJECT_H
#define WIREOBJECT_H

#include "WireRtti.h"
#include "WireSmartPointer.h"

namespace Wire
{
 
class /*WIRE_ENGINE_ITEM*/ Object
{
public:
	virtual ~Object();

	// run-time type information system
	static const Rtti TYPE;
	virtual const Rtti& GetType() const;
	Bool IsExactly(const Rtti& rType) const;
	Bool IsDerived(const Rtti& rType) const;
	Bool IsExactlyTypeOf(const Object* pObject) const;
	Bool IsDerivedTypeOf(const Object* pObject) const;

	// smart pointer system
	void IncrementReferences();
	void DecrementReferences();
	Int GetReferences() const;

protected:
	Object();

private:
	Int mReferences;
};

// static and dynamic type casting
template <class T> T* StaticCast(Object* pObj);
template <class T> const T* StaticCast(const Object* pObj);
template <class T> T* DynamicCast(Object* pObj);
template <class T> const T* DynamicCast(const Object* pObj);

typedef Pointer<Object> ObjectPtr;
#include "WireObject.inl"

}

#endif
