// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireVisibleSet.h"

#include "WireEffect.h"
#include "WireRenderObject.h"

using namespace Wire;

//----------------------------------------------------------------------------
VisibleSet::VisibleSet(UInt maxQuantity, UInt growBy)
	:
	mVisible(maxQuantity, growBy),
	mTransformations(maxQuantity, growBy),
	mKeys(maxQuantity, growBy)
{
}

//----------------------------------------------------------------------------
VisibleSet::~VisibleSet()
{
}

//----------------------------------------------------------------------------
void VisibleSet::Clear()
{
	mVisible.SetQuantity(0, false);
	mTransformations.SetQuantity(0, false);
	mKeys.SetQuantity(0, false);
}

//----------------------------------------------------------------------------
void VisibleSet::Sort(Int left, Int right)
{
	QuickSort(mKeys, mVisible.GetArray(), mTransformations.GetArray(), left,
		right);
}

//----------------------------------------------------------------------------
//void VisibleSet::QuickSort(UInt* const pKeys, Object** const pVisible,
void VisibleSet::QuickSort(TPODArray<UInt>& rKeys, Object** const pVisible,
	Transformation** pTrafo, Int left, Int right)
{
	Int i = left;
	Int j = right;
	UInt pivot = rKeys[(left + right) / 2];

	while (i <= j)
	{
		while (rKeys[i] < pivot)
		{
			i++;
		}

		while (rKeys[j] > pivot)
		{
			j--;
		}

		if (i <= j)
		{
			UInt tmp = rKeys[i];
			rKeys[i] = rKeys[j];
			rKeys[j] = tmp;

			WIRE_ASSERT((DynamicCast<RenderObject>(pVisible[i])));
			Object* pTmp = pVisible[i];
			pVisible[i] = pVisible[j];
			pVisible[j] = pTmp;

			Transformation* pTmpTrafo = pTrafo[i];
			pTrafo[i] = pTrafo[j];
			pTrafo[j] = pTmpTrafo;

			i++;
			j--;
		}
	};

	if (left < j)
	{
		QuickSort(rKeys, pVisible, pTrafo, left, j);
	}

	if (i < right)
	{
		QuickSort(rKeys, pVisible, pTrafo, i, right);
	}
}

//----------------------------------------------------------------------------
void VisibleSet::Sort()
{
	UInt indexStack[Effect::MAX_SIMULTANEOUS_EFFECTS][2];
	indexStack[0][0] = 0;
	indexStack[0][1] = 0;
	UInt top = 0;

	Object** pVisible = mVisible.GetArray();
	Transformation** pTransformations = mTransformations.GetArray();

	for (UInt i = 0; i < GetQuantity(); i++)
	{
		if (pVisible[i])
		{
			if (pTransformations[i] == NULL)
			{
				WIRE_ASSERT(DynamicCast<Effect>(pVisible[i]));
				if (indexStack[0][0] < indexStack[0][1])
				{
					WIRE_ASSERT(i == indexStack[0][1]); // TODO check
					// Sort leaves with no effect
					Sort(indexStack[0][0], i-1);
				}

				// Begin the scope of an effect.
				top++;
				WIRE_ASSERT(top < Effect::MAX_SIMULTANEOUS_EFFECTS);
				indexStack[top][0] = i;
				indexStack[top][1] = i;
			}
			else
			{
				// Found a leaf object.
				WIRE_ASSERT(DynamicCast<RenderObject>(pVisible[i]));
				indexStack[top][1]++;
			}
		}
		else
		{
			// End the scope of an effect.
 			UInt min = indexStack[top][0];
 			UInt max = indexStack[top][1];
 			WIRE_ASSERT(pTransformations[min] == NULL);
 			WIRE_ASSERT(DynamicCast<Effect>(pVisible[min]));

			if (--top > 0)
			{
				indexStack[top][1] = max + 1;
			}
			else
			{
				indexStack[0][0] = max + 2;
				indexStack[0][1] = max + 2;
			}
		}
	}

	WIRE_ASSERT(top == 0);
	WIRE_ASSERT(indexStack[0][1] = GetQuantity());
	if (indexStack[0][0] != indexStack[0][1])
	{
		Sort(indexStack[0][0], indexStack[0][1]-1);
	}
}
