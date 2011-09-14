// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRECULLERSORTER_H
#define WIRECULLERSORTER_H

#include "WireCuller.h"

namespace Wire
{

class CullerSorter : public Culler
{

public:
	CullerSorter(const Camera* pCamera = NULL, UInt maxQuantity = VisibleSet::
		VS_DEFAULT_MAX_QUANTITY, UInt growBy = VisibleSet::VS_DEFAULT_GROWBY);
	virtual ~CullerSorter();

	virtual void Insert(Spatial* pObject, Effect* pGlobalEffect);
};

}

#endif