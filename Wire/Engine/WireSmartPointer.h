#pragma once
#ifndef WIRESMARTPOINTER_H
#define WIRESMARTPOINTER_H

#include "../Foundation/WireSystem.h"

namespace Wire
{

template <class T>
class Pointer
{
public:
    // construction and destruction
    Pointer (T* pObject = NULL);
    Pointer (const Pointer& rPointer);
    ~Pointer ();

    // implicit conversions
    operator T* () const;
    T& operator* () const;
    T* operator-> () const;

    // assignment
    Pointer& operator= (T* pObject);
    Pointer& operator= (const Pointer& rReference);

    // comparisons
    Bool operator== (T* pObject) const;
    Bool operator!= (T* pObject) const;
    Bool operator== (const Pointer& rReference) const;
    Bool operator!= (const Pointer& rReference) const;

protected:
    // the shared object
    T* mpObject;
};

#include "WireSmartPointer.inl"

}

#endif
