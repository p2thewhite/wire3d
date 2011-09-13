// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRETINSTANCEID_H
#define WIRETINSTANCEID_H

#include "WireTArray.h"

namespace Wire
{

template <class T>
class TInstanceID
{
public:
	TInstanceID();
	~TInstanceID();

private:
	UInt mID;

	static TArray<TInstanceID<T>*> s_Instances;
	static TInstanceID<T>** s_pVacant;
};

#include "WireTInstanceID.inl"

}

#endif
