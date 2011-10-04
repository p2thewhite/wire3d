// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireCullerSorter.h"

#include "WireEffect.h"
#include "WireGeometry.h"
#include "WireLight.h"
#include "WireMaterial.h"
#include "WireRenderer.h"
#include "WireStateAlpha.h"

using namespace Wire;

//----------------------------------------------------------------------------
CullerSorter::CullerSorter(const Camera* pCamera, UInt maxQuantity,
	UInt growBy)
	:
	Culler(pCamera, maxQuantity, growBy)
{
	mVisibleSets.Append(WIRE_NEW VisibleSet(maxQuantity, growBy));
	mpOpaqueGeometry = WIRE_NEW VisibleSet(maxQuantity, growBy);
	mpTransparentGeometry = WIRE_NEW VisibleSet(maxQuantity, growBy);
	mKeys.SetMaxQuantity(maxQuantity);
	mKeys.SetGrowBy(growBy);
}

//----------------------------------------------------------------------------
CullerSorter::~CullerSorter()
{
	// The visible set created and stored in mVisibleSets in the constructor
	// is deleted by the destructor of the parent class, not here.
	WIRE_DELETE mpTransparentGeometry;
	WIRE_DELETE mpOpaqueGeometry;
}

//----------------------------------------------------------------------------
void CullerSorter::ComputeVisibleSet(Spatial* pScene)
{
	Culler::ComputeVisibleSet(pScene);

	WIRE_ASSERT(mVisibleSets.GetQuantity() >= 2);
	UnwrapEffectStackAndSort(mVisibleSets[0], mpOpaqueGeometry);
	UnwrapEffectStackAndSort(mVisibleSets[1], mpTransparentGeometry);

	VisibleSet* pTemp = mVisibleSets[0];
	mVisibleSets[0] = mpOpaqueGeometry;
	mpOpaqueGeometry = pTemp;

	pTemp = mVisibleSets[1];
	mVisibleSets[1] = mpTransparentGeometry;
	mpTransparentGeometry = pTemp;
}

//----------------------------------------------------------------------------
void CullerSorter::UnwrapEffectStackAndSort(VisibleSet* pSource, VisibleSet*
	pDestination)
{
	pDestination->Clear();
	mKeys.SetQuantity(0, false);

	// The destination set will have at least the size of the source set.
	if (pDestination->GetMaxQuantity() < pSource->GetMaxQuantity())
	{
		pDestination->SetMaxQuantity(pSource->GetQuantity());
	}

	UInt indexStack[Renderer::MAX_GLOBAL_EFFECTS][2];
	indexStack[0][0] = 0;
	indexStack[0][1] = 0;
	UInt top = 0;

	const UInt visibleQuantity = pSource->GetQuantity();
	VisibleObject* const pVisible = pSource->GetVisible();
	for (UInt i = 0; i < visibleQuantity; i++)
	{
		if (pVisible[i].Object)
		{
			if (pVisible[i].GlobalEffect)
			{
				// Begin the scope of a global effect.
				top++;
				WIRE_ASSERT(top < Renderer::MAX_GLOBAL_EFFECTS);
				indexStack[top][0] = i;
				indexStack[top][1] = i;
			}
			else
			{
				indexStack[top][1]++;
			}
		}
		else
		{
			// End the scope of a global effect.
			WIRE_ASSERT(!pVisible[i].GlobalEffect);
			UInt min = indexStack[top][0];
			UInt max = indexStack[top][1];

			pDestination->Insert(pVisible[i].Object, pVisible[i].
				GlobalEffect);

 			for (UInt i = min+1; i <= max; i++)
 			{
				WIRE_ASSERT(pVisible[i].GlobalEffect == NULL);
				WIRE_ASSERT(DynamicCast<Geometry>(pVisible[i].Object));
				UInt id = 0;
				Material* pMaterial = StaticCast<Geometry>(pVisible[i].
					Object)->GetMaterial();
				if (pMaterial)
				{
					id = pMaterial->ID;
				}

				mKeys.Append(id);
				pDestination->Insert(pVisible[i].Object, NULL);
			}

//			QuickSort(mKeys.GetArray(), pDestination->GetVisible(), min+1,
			QuickSort(mKeys, pDestination->GetVisible(), min+1,
				max);

			pDestination->Insert(NULL, NULL);

			if (--top > 0)
			{
				indexStack[top][1] = max + 1;
			}
		}
	}

	WIRE_ASSERT(top == 0);
	for (UInt i = indexStack[0][0]; i < indexStack[0][1]; i++)
	{
		WIRE_ASSERT(pVisible[i].GlobalEffect == NULL);
		WIRE_ASSERT(DynamicCast<Geometry>(pVisible[i].Object));
		UInt id = 0;
		Material* pMaterial = StaticCast<Geometry>(pVisible[i].
			Object)->GetMaterial();
		if (pMaterial)
		{
			id = pMaterial->ID;
		}

		mKeys.Append(id);
		pDestination->Insert(pVisible[i].Object, pVisible[i].GlobalEffect);
	}

	if (indexStack[0][0] < indexStack[0][1])
	{
		//	QuickSort(mKeys.GetArray(), pDestination->GetVisible(), indexStack[0][0],
		QuickSort(mKeys, pDestination->GetVisible(), indexStack[0][0],
			indexStack[0][1]-1);
	}
}

//----------------------------------------------------------------------------
//void CullerSorter::QuickSort(UInt* const pKeys, VisibleObject* const
void CullerSorter::QuickSort(TArray<UInt>& pKeys, VisibleObject* const
	pVisibles, Int left, Int right)
{
	Int i = left;
	Int j = right;
	UInt pivot = pKeys[(left + right) / 2];

	while (i <= j)
	{
		while (pKeys[i] < pivot)
		{
			i++;
		}

		while (pKeys[j] > pivot)
		{
			j--;
		}

		if (i <= j)
		{
			UInt tmp = pKeys[i];
			pKeys[i] = pKeys[j];
			pKeys[j] = tmp;

			WIRE_ASSERT(pVisibles[i].GlobalEffect == NULL);
			Spatial* pTmp = pVisibles[i].Object;
			pVisibles[i].Object = pVisibles[j].Object;
			pVisibles[j].Object = pTmp;

			i++;
			j--;
		}
	};

	if (left < j)
	{
		QuickSort(pKeys, pVisibles, left, j);
	}

	if (i < right)
	{
		QuickSort(pKeys, pVisibles, i, right);
	}
}

//----------------------------------------------------------------------------
void CullerSorter::Insert(Spatial* pObject, Effect* pGlobalEffect)
{
	WIRE_ASSERT(mVisibleSets.GetQuantity() >= 2);

	Geometry* pGeometry = DynamicCast<Geometry>(pObject);
	if (!pGeometry)
	{
		GetVisibleSet(0)->Insert(pObject, pGlobalEffect);
		GetVisibleSet(1)->Insert(pObject, pGlobalEffect);
		return;
	}

	WIRE_ASSERT(pGlobalEffect == NULL);
	StateAlpha* pAlpha = DynamicCast<StateAlpha>(pGeometry->States[State::
		ALPHA]);
	if (pAlpha)
	{
		if (pAlpha->BlendEnabled)
		{
			GetVisibleSet(1)->Insert(pObject, pGlobalEffect);
		}
		else
		{
			GetVisibleSet(0)->Insert(pObject, pGlobalEffect);
		}
	}
}
