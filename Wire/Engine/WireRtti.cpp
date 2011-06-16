// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRtti.h"
using namespace Wire;

//----------------------------------------------------------------------------
Rtti::Rtti(const Char* pName, const Rtti* pBaseType)
	:
	mpName(pName),
	mpBaseType(pBaseType)
{
}

//----------------------------------------------------------------------------
Rtti::~Rtti ()
{
}

//----------------------------------------------------------------------------
Bool Rtti::IsDerived(const Rtti& rType) const
{
	const Rtti* pSearch = this;
	while (pSearch)
	{
		if (pSearch == &rType)
		{
			return true;
		}

		pSearch = pSearch->mpBaseType;
	}

	return false;
}
