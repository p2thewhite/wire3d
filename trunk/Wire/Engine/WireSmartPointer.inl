//-------------------------------------------------------------------------
template <class T>
Pointer<T>::Pointer(T* pObject)
{
    mpObject = pObject;
    if (mpObject)
    {
        mpObject->IncrementReferences();
    }
}

//-------------------------------------------------------------------------
template <class T>
Pointer<T>::Pointer(const Pointer& rPointer)
{
    mpObject = rPointer.mpObject;
    if (mpObject)
    {
        mpObject->IncrementReferences();
    }
}

//-------------------------------------------------------------------------
template <class T>
Pointer<T>::~Pointer()
{
    if (mpObject)
    {
        mpObject->DecrementReferences();
    }
}

//-------------------------------------------------------------------------
template <class T>
Pointer<T>::operator T* () const
{
    return mpObject;
}

//-------------------------------------------------------------------------
template <class T>
T& Pointer<T>::operator* () const
{
    return *mpObject;
}

//-------------------------------------------------------------------------
template <class T>
T* Pointer<T>::operator-> () const
{
    return mpObject;
}

//-------------------------------------------------------------------------
template <class T>
Pointer<T>& Pointer<T>::operator= (T* pObject)
{
    if (mpObject != pObject)
    {
        if (pObject)
        {
            pObject->IncrementReferences();
        }

        if (mpObject)
        {
            mpObject->DecrementReferences();
        }

        mpObject = pObject;
    }

	return *this;
}

//-------------------------------------------------------------------------
template <class T>
Pointer<T>& Pointer<T>::operator= (const Pointer& rPointer)
{
    if (mpObject != rPointer.mpObject)
    {
        if (rPointer.mpObject)
        {
            rPointer.mpObject->IncrementReferences();
        }

        if (mpObject)
        {
            mpObject->DecrementReferences();
        }

        mpObject = rPointer.mpObject;
    }

    return *this;
}

//-------------------------------------------------------------------------
template <class T>
bool Pointer<T>::operator== (T* pObject) const
{
    return mpObject == pObject;
}

//-------------------------------------------------------------------------
template <class T>
bool Pointer<T>::operator!= (T* pObject) const
{
    return mpObject != pObject;
}

//-------------------------------------------------------------------------
template <class T>
bool Pointer<T>::operator== (const Pointer& rPointer) const
{
    return mpObject == rPointer.mpObject;
}

//-------------------------------------------------------------------------
template <class T>
bool Pointer<T>::operator!= (const Pointer& rPointer) const
{
    return mpObject != rPointer.mpObject;
}

//-------------------------------------------------------------------------
