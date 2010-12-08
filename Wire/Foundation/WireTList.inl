//----------------------------------------------------------------------------
template <class T>
TList<T>::TList()
{
    mpNext = NULL;
}

//----------------------------------------------------------------------------
template <class T>
TList<T>::TList(const T& rItem, TList* pNext)
    :
    mItem(rItem)
{
    mpNext = pNext;
}

//----------------------------------------------------------------------------
template <class T>
TList<T>::~TList()
{
}

//----------------------------------------------------------------------------
template <class T>
void TList<T>::SetItem(const T& rItem)
{
    mItem = rItem;
}

//----------------------------------------------------------------------------
template <class T>
T& TList<T>::Item()
{
    return mItem;
}

//----------------------------------------------------------------------------
template <class T>
const T& TList<T>::GetItem() const
{
    return mItem;
}

//----------------------------------------------------------------------------
template <class T>
void TList<T>::SetNext(TList* pNext)
{
    mpNext = pNext;
}

//----------------------------------------------------------------------------
template <class T>
TList<T>*& TList<T>::Next()
{
    return mpNext;
}

//----------------------------------------------------------------------------
template <class T>
const TList<T>* TList<T>::GetNext() const
{
    return mpNext;
}

//----------------------------------------------------------------------------
template <class T>
UInt TList<T>::GetQuantity() const
{
	UInt quantity = 0;
	for (const TList<T>* pIter = this; pIter; pIter = pIter->mpNext)
	{
		quantity++;
	}

	return quantity;
}
