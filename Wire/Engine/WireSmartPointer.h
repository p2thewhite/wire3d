#pragma once
#ifndef WIRESMARTPOINTER_H
#define WIRESMARTPOINTER_H

namespace Wire
{

template <class T>
class Pointer
{
public:
    // construction and destruction
    Pointer (T* pObject = 0);
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
    bool operator== (T* pObject) const;
    bool operator!= (T* pObject) const;
    bool operator== (const Pointer& rReference) const;
    bool operator!= (const Pointer& rReference) const;

protected:
    // the shared object
    T* mpObject;
};

#include "WireSmartPointer.inl"

}

#endif /* WIRESMARTPOINTER_H */
