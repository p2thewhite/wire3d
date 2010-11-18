//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
THashTable<TKEY,TVALUE>::THashTable(UInt tableSize)
{
    WIRE_ASSERT(tableSize > 0);

    mTableSize = tableSize;
    mQuantity = 0;
    mIndex = 0;
    mpItem = NULL;
    mpTable = WIRE_NEW HashItem*[mTableSize];
	WIRE_ASSERT(mpTable);

	System::Memset(mpTable, 0, mTableSize * sizeof(HashItem*));
    UserHashFunction = NULL;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
THashTable<TKEY,TVALUE>::~THashTable()
{
    RemoveAll();
    WIRE_DELETE[] mpTable;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
inline UInt THashTable<TKEY,TVALUE>::GetQuantity() const
{
    return mQuantity;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
Bool THashTable<TKEY,TVALUE>::Insert(const TKEY& rKey, const TVALUE& rValue)
{
    // find hash table entry for given key
    UInt index = HashFunction(rKey);
    HashItem* pItem = mpTable[index];

    // search for item in list associated with key
    while (pItem)
    {
        if (rKey == pItem->mKey)
        {
            // item already in hash table
            return false;
        }

        pItem = pItem->mpNext;
    }

    // add item to beginning of list
    pItem = WIRE_NEW HashItem;
	WIRE_ASSERT(pItem);
	pItem->mKey = rKey;
    pItem->mValue = rValue;
    pItem->mpNext = mpTable[index];
    mpTable[index] = pItem;
    mQuantity++;

    return true;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
TVALUE* THashTable<TKEY,TVALUE>::Find(const TKEY& rKey) const
{
    // find hash table entry for given key
    UInt index = HashFunction(rKey);
    HashItem* pItem = mpTable[index];

    // search for item in list associated with key
    while (pItem)
    {
        if (rKey == pItem->mKey)
        {
            // item is in hash table
            return &pItem->mValue;
        }

        pItem = pItem->mpNext;
    }

    return NULL;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
Bool THashTable<TKEY,TVALUE>::Remove(const TKEY& rKey)
{
    // find hash table entry for given key
    UInt index = HashFunction(rKey);
    HashItem* pItem = mpTable[index];

    if (!pItem)
    {
        return false;
    }

    if (rKey == pItem->mKey)
    {
        // item is at front of list, strip it off
        HashItem* pSave = pItem;
        mpTable[index] = pItem->mpNext;
        WIRE_DELETE pSave;
        mQuantity--;
        return true;
    }

    // search for item in list
    HashItem* pPrev = pItem;
    HashItem* pCurr = pItem->mpNext;
    while (pCurr && rKey != pCurr->mKey)
    {
        pPrev = pCurr;
        pCurr = pCurr->mpNext;
    }

    if (pCurr)
    {
        // found the item
        pPrev->mpNext = pCurr->mpNext;
        WIRE_DELETE pCurr;
        mQuantity--;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
void THashTable<TKEY,TVALUE>::RemoveAll()
{
    if (mQuantity > 0)
    {
        for (UInt i = 0; i < mTableSize; i++)
        {
            while (mpTable[i])
            {
                HashItem* pkSave = mpTable[i];
                mpTable[i] = mpTable[i]->mpNext;
                WIRE_DELETE pkSave;
                if (--mQuantity == 0)
                {
                    return;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
TVALUE* THashTable<TKEY,TVALUE>::GetFirst(TKEY* pKey) const
{
    if (mQuantity > 0)
    {
        for (mIndex = 0; mIndex < mTableSize; mIndex++)
        {
            if (mpTable[mIndex])
            {
                mpItem = mpTable[mIndex];
				if (pKey)
				{
					*pKey = mpItem->mKey;
				}

				return &mpItem->mValue;
            }
        }
    }

    return NULL;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
TVALUE* THashTable<TKEY,TVALUE>::GetNext(TKEY* pKey) const
{
    if (mQuantity > 0)
    {
        mpItem = mpItem->mpNext;
        if (mpItem)
        {
			if (pKey)
			{
				*pKey = mpItem->mKey;
			}

			return &mpItem->mValue;
        }
        
        for (mIndex++; mIndex < mTableSize; mIndex++)
        {
            if (mpTable[mIndex])
            {
                mpItem = mpTable[mIndex];
				if (pKey)
				{
					*pKey = mpItem->mKey;
				}

				return &mpItem->mValue;
            }
        }
    }

    return NULL;
}

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
UInt THashTable<TKEY,TVALUE>::HashFunction(const TKEY& rKey) const
{
	if (UserHashFunction)
    {
        return (*UserHashFunction)(rKey);
    }

    // default hash function
    static Double sHashMultiplier = 0.5 * (MathD::Sqrt(5.0) - 1.0);
	UInt key = reinterpret_cast<UInt>(rKey);
    key %= mTableSize;
	Double fraction = MathD::FMod(sHashMultiplier * key, 1.0);
	return static_cast<UInt>(MathD::Floor(mTableSize*fraction));
}
