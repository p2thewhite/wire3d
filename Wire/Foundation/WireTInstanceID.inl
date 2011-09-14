// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

template<class T> TArray<TInstanceID<T>*> TInstanceID<T>::s_Instances(0, 10);
template<class T> TInstanceID<T>** TInstanceID<T>::s_pVacant = NULL;

//----------------------------------------------------------------------------
template <class T>
TInstanceID<T>::TInstanceID()
{
	if (s_pVacant)
	{
		mID = s_pVacant - s_Instances.GetArray();

		if (*s_pVacant)
		{
			s_pVacant = &(s_Instances[reinterpret_cast<TInstanceID<T>**>
				(*s_pVacant) - s_Instances.GetArray()]);
		}
		else
		{
			s_pVacant = NULL;
		}
	}
	else
	{
		mID = s_Instances.GetQuantity();
	}

	s_Instances.SetElement(mID, NULL);
}

//----------------------------------------------------------------------------
template <class T>
TInstanceID<T>::~TInstanceID()
{
	if (s_pVacant)
	{
		s_Instances[mID] = reinterpret_cast<TInstanceID<T>*>(s_pVacant);
	}

	s_pVacant = &(s_Instances[mID]);
}

//----------------------------------------------------------------------------
template <class T>
TInstanceID<T>::operator UInt () const
{
	return mID;
}
