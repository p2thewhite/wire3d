#include "WireRtti.h"
using namespace Wire;

//----------------------------------------------------------------------------
Rtti::Rtti(const Rtti* pBaseType)
{
	mpBaseType = pBaseType;
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
