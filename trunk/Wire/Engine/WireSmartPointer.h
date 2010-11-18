#pragma once
#ifndef WIRESMARTPOINTER_H
#define WIRESMARTPOINTER_H

#include "WireSystem.h"

namespace Wire
{

template <class T>
class Pointer
{
public:
    // construction and destruction
	Pointer(T* pObject = NULL);
	Pointer(const Pointer& rPointer);
	~Pointer();

    // implicit conversions
	inline operator T* () const;
	inline T& operator* () const;
	inline T* operator-> () const;

	inline T* Get() const;

    // assignment
	Pointer& operator= (T* pObject);
	Pointer& operator= (const Pointer& rReference);

    // comparisons
	inline Bool operator== (T* pObject) const;
	inline Bool operator!= (T* pObject) const;
	inline Bool operator== (const Pointer& rReference) const;
	inline Bool operator!= (const Pointer& rReference) const;

protected:
    // the shared object
    T* mpObject;
};

#include "WireSmartPointer.inl"

}

#endif
