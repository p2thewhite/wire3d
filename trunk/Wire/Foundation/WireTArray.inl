//----------------------------------------------------------------------------
template <class T>
TArray<T>::TArray(UInt quantity, UInt growBy)
{
	mQuantity = 0;
	mMaxQuantity = quantity;
	mGrowBy = growBy;
	mpArray = (mMaxQuantity > 0 ? WIRE_NEW T[mMaxQuantity] : NULL);
}

//----------------------------------------------------------------------------
template <class T>
TArray<T>::TArray(const TArray& rObject)
{
	mpArray = NULL;
	*this = rObject;
}

//----------------------------------------------------------------------------
template <class T>
TArray<T>::~TArray()
{
	WIRE_DELETE[] mpArray;
}

//----------------------------------------------------------------------------
template <class T>
TArray<T>& TArray<T>::operator= (const TArray& rObject)
{
	mQuantity = rObject.mQuantity;
	mMaxQuantity = rObject.mMaxQuantity;
	mGrowBy = rObject.mGrowBy;

	WIRE_DELETE[] mpArray;

	if (mMaxQuantity > 0)
	{
		mpArray = WIRE_NEW T[mMaxQuantity];
		for (UInt i = 0; i < mMaxQuantity; i++)
		{
			mpArray[i] = rObject.mpArray[i];
		}
	}
	else
	{
		mpArray = NULL;
	}

	return *this;
}

//----------------------------------------------------------------------------
template <class T>
UInt TArray<T>::GetQuantity() const
{
	return mQuantity;
}

//----------------------------------------------------------------------------
template <class T>
T* TArray<T>::GetArray()
{
	return mpArray;
}

//----------------------------------------------------------------------------
template <class T>
const T* TArray<T>::GetArray() const
{
	return mpArray;
}

//----------------------------------------------------------------------------
template <class T>
T& TArray<T>::operator[] (UInt i)
{
	WIRE_ASSERT(i < mQuantity && mpArray);
	if (i >= mQuantity)
	{
		i = mQuantity-1;
	}

	return mpArray[i];
}

//----------------------------------------------------------------------------
template <class T>
const T& TArray<T>::operator[] (UInt i) const
{
	WIRE_ASSERT(i < mQuantity && mpArray);
	if (i >= mQuantity)
	{
		i = mQuantity-1;
	}

	return mpArray[i];
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::Remove(UInt i)
{
	WIRE_ASSERT(i < mQuantity);
	if (i >= mQuantity)
	{
		return;
	}

	for (UInt j = i+1; j < mQuantity; i = j++)
	{
		mpArray[i] = mpArray[j];
	}

	mpArray[mQuantity-1] = T();
	mQuantity--;
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::RemoveLast()
{
	Remove(mQuantity-1);
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::RemoveAll()
{
	for (UInt i = 0; i < mQuantity; i++)
	{
		mpArray[i] = T();
	}

	mQuantity = 0;
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::SetMaxQuantity(UInt newMaxQuantity, Bool copy)
{
	if (newMaxQuantity == 0)
	{
		WIRE_DELETE[] mpArray;
		mpArray = NULL;
		mQuantity = 0;
		mMaxQuantity = 0;
		return;
	}

	if (newMaxQuantity != mMaxQuantity)
	{
		T* pNewArray = WIRE_NEW T[newMaxQuantity];

		if (copy)
		{
			UInt copyQuantity;
			if (newMaxQuantity > mMaxQuantity)
			{
				copyQuantity = mMaxQuantity;
			}
			else
			{
				copyQuantity = newMaxQuantity;
			}

			for (UInt i = 0; i < copyQuantity; i++)
			{
				pNewArray[i] = mpArray[i];
			}

			if (mQuantity > newMaxQuantity)
			{
				mQuantity = newMaxQuantity;
			}
		}
		else
		{
			mQuantity = 0;
		}

		WIRE_DELETE[] mpArray;
		mpArray = pNewArray;
		mMaxQuantity = newMaxQuantity;
	}
}

//----------------------------------------------------------------------------
template <class T>
UInt TArray<T>::GetMaxQuantity() const
{
	return mMaxQuantity;
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::SetGrowBy(UInt growBy)
{
	mGrowBy = growBy;
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::SetQuantity(UInt newQuantity, Bool copy)
{
	if (newQuantity > mMaxQuantity)
	{
		SetMaxQuantity(newQuantity, copy);
	}
	
	mQuantity = newQuantity;
}

//----------------------------------------------------------------------------
template <class T>
UInt TArray<T>::GetGrowBy() const
{
	return mGrowBy;
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::Append(const T& rElement)
{
	if (++mQuantity > mMaxQuantity)
	{
		if (mGrowBy > 0)
		{
			// Increase the size of the array. In the event rElement is a
			// reference to something in the current array, the reallocation
			// in SetMaxQuantity will invalidate the reference. A copy of
			// rElement is made here to avoid the invalidation.
			T saveElement(rElement);
			SetMaxQuantity(mMaxQuantity + mGrowBy, true);
			mpArray[mQuantity-1] = saveElement;
			return;
		}

		// cannot grow the array, overwrite the last element
		--mQuantity;
	}

	mpArray[mQuantity-1] = rElement;
}

//----------------------------------------------------------------------------
template <class T>
void TArray<T>::SetElement(UInt i, const T& rElement)
{
	if (i >= mQuantity)
	{
		if (i >= mMaxQuantity)
		{
			if (mGrowBy > 0)
			{
				// increase the size of the array
				if (i+1 >= mMaxQuantity)
				{
					UInt n = 1 + static_cast<Int>(0.5F + (i+1 - mMaxQuantity) /
						static_cast<Float>(mGrowBy));

					// Increase the size of the array. In the event rtElement
					// is a reference to something in the current array, the
					// reallocation in SetMaxQuantity will invalidate the
					// reference. A copy of rtElement is made here to avoid
					// the invalidation.
					T saveElement(rElement);
					SetMaxQuantity(mMaxQuantity + n * mGrowBy, true);
					mQuantity = i+1;
					mpArray[i] = saveElement;
					return;
				}
			}
			else
			{
				// cannot grow the array, overwrite the last element
				i = mQuantity-1;
			}
		}
		mQuantity = i+1;
	}

	mpArray[i] = rElement;
}
