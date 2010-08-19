#pragma once
#ifndef WIRETHASHTABLE_H
#define WIRETHASHTABLE_H

// The class TKEY is either native data or is class data that has the
// following member functions:
//   TKEY::TKEY ()
//   TKEY& TKEY::operator= (const TKEY&)
//   Bool TKEY::operator== (const TKEY&) const
//   Bool TKEY::operator!= (const TKEY&) const
//   TKEY::operator UInt () const
// The implicit conversion to UInt is used to select a hash table
// index for the T object. The return value need not be within the range of
// hash table indices. THashTable will use modular arithmetic to make this
// happen.
//
// The class TVALUE is either native data or is class data that has the
// following member functions:
//   TVALUE::TVALUE ()
//   TVALUE& TVALUE::operator= (const TVALUE&)

#include "WireMath.h"

namespace Wire
{

template <class TKEY, class TVALUE>
class THashTable
{
public:
    // construction and destruction
    THashTable(UInt tableSize = 256);
    ~THashTable();

    // element access
    UInt GetQuantity() const;

    // insert a key-value pair into the hash table (false if rKey exists)
    Bool Insert(const TKEY& rKey, const TVALUE& rValue);

    // search for a key and returns it value (null, if key does not exist)
    TVALUE* Find(const TKEY& rKey) const;

    // remove key-value pairs from the hash table
    Bool Remove(const TKEY& rKey);
    void RemoveAll();

    // linear traversal of table
    TVALUE* GetFirst(TKEY* pKey = NULL) const;
    TVALUE* GetNext(TKEY* pKey = NULL) const;

    // user-specified key-to-index construction
    UInt (*UserHashFunction)(const TKEY&);

private:
    struct HashItem
    {
        TKEY mKey;
        TVALUE mValue;
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

#include "WireTHashTable.inl"

}

#endif
