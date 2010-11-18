//----------------------------------------------------------------------------
template <class TKEY>
THashSet<TKEY>::THashSet(UInt tableSize)
{
    WIRE_ASSERT(tableSize > 0);

    mTableSize = tableSize;
    mQuantity = 0;
    mIndex = 0;
    mpItem = 0;
    mpTable = WIRE_NEW HashItem*[mTableSize];
	System::Memset(mpTable, 0, mTableSize*sizeof(HashItem*));
    UserHashFunction = NULL;
}

//----------------------------------------------------------------------------
template <class TKEY>
THashSet<TKEY>::~THashSet()
{
    RemoveAll();
    WIRE_DELETE[] mpTable;
}

//----------------------------------------------------------------------------
template <class TKEY>
inline UInt THashSet<TKEY>::GetQuantity() const
{
    return mQuantity;
}

//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::Insert(const TKEY& rKey)
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
            return &pItem->mKey;
        }

        pItem = pItem->mpNext;
    }

    // add item to beginning of list
    pItem = WIRE_NEW HashItem;
    pItem->mKey = rKey;
    pItem->mpNext = mpTable[index];
    mpTable[index] = pItem;
    mQuantity++;

    return &pItem->mKey;
}

//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::Get(const TKEY& rtKey) const
{
    // find hash table entry for given key
    UInt index = HashFunction(rtKey);
    HashItem* pItem = mpTable[index];

    // search for item in list associated with key
    while (pItem)
    {
        if (rtKey == pItem->mKey)
        {
            // item is in hash table
            return &pItem->mKey;
        }

        pItem = pItem->mpNext;
    }

    return NULL;
}

//----------------------------------------------------------------------------
template <class TKEY>
Bool THashSet<TKEY>::Remove(const TKEY& rKey)
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
        HashItem* pkSave = pItem;
        mpTable[index] = pItem->mpNext;
        WIRE_DELETE pkSave;
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
template <class TKEY>
void THashSet<TKEY>::RemoveAll()
{
    if (mQuantity > 0)
    {
        for (UInt index = 0; index < mTableSize; index++)
        {
            while (mpTable[index])
            {
                HashItem* pSave = mpTable[index];
                mpTable[index] = mpTable[index]->mpNext;
                WIRE_DELETE pSave;
                if (--mQuantity == 0)
                {
                    return;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::GetFirst() const
{
    if (mQuantity > 0)
    {
        for (mIndex = 0; mIndex < mTableSize; mIndex++)
        {
            if (mpTable[mIndex])
            {
                mpItem = mpTable[mIndex];
                return &mpItem->mKey;
            }
        }
    }

    return NULL;
}

//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::GetNext() const
{
    if (mQuantity > 0)
    {
        mpItem = mpItem->mpNext;
        if (mpItem)
        {
            return &mpItem->mKey;
        }
        
        for (mIndex++; mIndex < mTableSize; mIndex++)
        {
            if (mpTable[mIndex])
            {
                mpItem = mpTable[mIndex];
                return &mpItem->mKey;
            }
        }
    }

    return NULL;
}

//----------------------------------------------------------------------------
template <class TKEY>
UInt THashSet<TKEY>::HashFunction(const TKEY& rKey) const
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
