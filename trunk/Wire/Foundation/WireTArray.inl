//----------------------------------------------------------------------------
template <class T>
TArray<T>::TArray(Int quantity, Int growBy)
{
	WIRE_ASSERT(quantity >= 0 && growBy >= 0);

	if (quantity < 0)
	{
		quantity = 0;
	}

	if (growBy < 0)
	{
		growBy = 0;
	}

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
	if (mpArray)
	{
		WIRE_DELETE[] mpArray;
	}
}

//----------------------------------------------------------------------------
template <class T>
TArray<T>& TArray<T>::operator= (const TArray& rObject)
{
	mQuantity = rObject.mQuantity;
	mMaxQuantity = rObject.mMaxQuantity;
	mGrowBy = rObject.mGrowBy;

	if (mpArray)
	{
		WIRE_DELETE[] mpArray;
	}

	if (mMaxQuantity > 0)
	{
		mpArray = WIRE_NEW T[mMaxQuantity];
		for (Int i = 0; i < mMaxQuantity; i++)
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
Int TArray<T>::GetQuantity() const
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
T& TArray<T>::operator[] (Int i)
{
	WIRE_ASSERT(0 <= i && i < mQuantity && mpArray);
	if (i >= mQuantity)
	{
		i = mQuantity-1;
	}

	if (i < 0)
	{
		i = 0;
	}

	return mpArray[i];
}

//----------------------------------------------------------------------------
template <class T>
const T& TArray<T>::operator[] (Int i) const
{
	WIRE_ASSERT(0 <= i && i < mQuantity && mpArray);
	if (i >= mQuantity)
	{
		i = mQuantity-1;
	}

	if (i < 0)
	{
		i = 0;
	}

	return mpArray[i];
}

//-------------------------------------------------------------------------------
template <class T>
void TArray<T>::Remove(Int i)
{
	WIRE_ASSERT(0 <= i && i < mQuantity);
	if (i < 0 || i >= mQuantity)
	{
		return;
	}

	for (Int j = i+1; j < mQuantity; i = j++)
	{
		mpArray[i] = mpArray[j];
	}

	mpArray[mQuantity-1] = T();
	mQuantity--;
}

//-------------------------------------------------------------------------------
template <class T>
void TArray<T>::RemoveAll()
{
	for (int i = 0; i < mQuantity; i++)
	{
		mpArray[i] = T();
	}

	mQuantity = 0;
}

//-------------------------------------------------------------------------------
template <class T>
void TArray<T>::SetMaxQuantity(Int newMaxQuantity, Bool copy)
{
	WIRE_ASSERT(newMaxQuantity >= 0);
	if (newMaxQuantity < 0)
	{
		newMaxQuantity = 0;
	}

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
			Int copyQuantity;
			if (newMaxQuantity > mMaxQuantity)
			{
				copyQuantity = mMaxQuantity;
			}
			else
			{
				copyQuantity = newMaxQuantity;
			}

			for (Int i = 0; i < copyQuantity; i++)
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

//-------------------------------------------------------------------------------
template <class T>
Int TArray<T>::GetMaxQuantity() const
{
	return mMaxQuantity;
}

//-------------------------------------------------------------------------------
template <class T>
void TArray<T>::SetGrowBy(Int growBy)
{
	WIRE_ASSERT(growBy >= 0);
	if (growBy >= 0)
	{
		mGrowBy = growBy;
	}
}

//-------------------------------------------------------------------------------
template <class T>
Int TArray<T>::GetGrowBy() const
{
	return mGrowBy;
}
