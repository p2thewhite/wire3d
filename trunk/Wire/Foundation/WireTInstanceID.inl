// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

template<class T> TArray<UInt> TInstanceID<T>::s_Instances(0, 10);
template<class T> UInt TInstanceID<T>::s_FreeID = 0;

//----------------------------------------------------------------------------
template <class T>
TInstanceID<T>::TInstanceID()
{
	if (s_FreeID > 0)
	{
		mID = s_FreeID-1;
		s_FreeID = s_Instances[mID];
	}
	else
	{
		mID = s_Instances.GetQuantity();
	}

	s_Instances.SetElement(mID, 0);
}

//----------------------------------------------------------------------------
template <class T>
TInstanceID<T>::~TInstanceID()
{
	if (s_FreeID > 0)
	{
		if (s_FreeID > mID+1)
		{
			s_Instances[mID] = s_FreeID;
		}
		else
		{
			// sort mID into descending order so new instances always get
			// the lowest free ID.
			UInt freeID = s_FreeID-1;
			UInt lastValidFreeID = freeID;
			while (freeID < mID)
			{
				lastValidFreeID = freeID;
				freeID = s_Instances[freeID]-1;
			}

			s_Instances[mID] = s_Instances[lastValidFreeID];
			s_Instances[lastValidFreeID] = mID+1;
			return;
		}	
	}

	s_FreeID = mID+1;
}

//----------------------------------------------------------------------------
template <class T>
TInstanceID<T>::operator UInt () const
{
	return mID;
}
