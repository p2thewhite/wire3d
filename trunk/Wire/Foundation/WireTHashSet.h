#pragma once
#ifndef WIRETHASHSET_H
#define WIRETHASHSET_H

// The class TKEY is either native data or is class data that has the
// following member functions:
//   TKEY::TKEY ()
//   TKEY& TKEY::operator= (const TKEY&)
//   Bool TKEY::operator== (const TKEY&) const
//   Bool TKEY::operator!= (const TKEY&) const
//   TKEY::operator UInt () const
// The implicit conversion to UInt is used to select a hash table
// index for the T object. The return value need not be within the range of
// hash table indices. THashSet will use modular arithmetic to make this
// happen.

#include "WireTypes.h"

namespace Wire
{

template <class TKEY>
class THashSet
{
public:
    // construction and destruction
    THashSet(UInt tableSize = 128);
    ~THashSet ();

    // element access
    UInt GetQuantity() const;

    // A pointer to the actual storage is returned so that the caller has
    // direct access to it. This allows a subset of TKEY members to be used
    // in key comparison.
    TKEY* Insert(const TKEY& rKey);

    // If the input key exists, a pointer to the actual storage is returned.
    // This allows a subset of TKEY members to be used in key comparison,
    // but gives the caller a chance to modify other TKEY members.
    TKEY* Get(const TKEY& rKey) const;

	Bool Remove(const TKEY& rKey);
    void RemoveAll();

    // linear traversal of map
    TKEY* GetFirst() const;
    TKEY* GetNext() const;

    // user-specified key-to-index construction
    UInt (*UserHashFunction)(const TKEY&);

private:
    class HashItem
    {
    public:
        TKEY mKey;
        HashItem* mpNext;
    };

    // Default key-to-index construction (override by user-specified when
    // requested).
    UInt HashFunction(const TKEY& rKey) const;

    // hash table
    UInt mTableSize;
    UInt mQuantity;
    HashItem** mpTable;

    // iterator for traversal
    mutable UInt mIndex;
    mutable HashItem* mpItem;
};

#include "WireTHashSet.inl"

}

#endif
