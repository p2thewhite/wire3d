#pragma once
#ifndef WIRETMAP_H
#define WIRETMAP_H

#include "WireTArray.h"

namespace Wire
{

template <class TKEY, class TVALUE>
class TMap
{
public:
	// construction and destruction
	TMap(UInt quantity = 0, UInt growBy = 1);
	virtual ~TMap();

	struct MapElement
	{
		TKEY Key;
		TVALUE Value;
	};

	void Insert(const TKEY& rKey, const TVALUE& rValue);
	void Remove(const TKEY& rKey);
	TVALUE* Find(const TKEY& rKey);

private:
	TArray<MapElement>* mpArray;
};

#include "WireTMap.inl"

}

#endif
