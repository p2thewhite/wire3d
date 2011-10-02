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
		mKeys.SetMaxQuantity(pSource->GetQuantity());
	}

	UInt indexStack[Renderer::MAX_GLOBAL_EFFECTS][2];
	indexStack[0][0] = 0;
	indexStack[0][1] = 0;
	UInt top = 0;

	const UInt visibleQuantity = pSource->GetQuantity();
	VisibleObject* pVisible = pSource->GetVisible();
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
}

//----------------------------------------------------------------------------
void CullerSorter::QuickSort(UInt arr[], UInt left, UInt right)
{
	UInt i = left;
	UInt j = right;
	UInt pivot = arr[(left + right) / 2];

	while (i <= j)
	{
		while (arr[i] < pivot)
		{
			i++;
		}

		while (arr[j] > pivot)
		{
			j--;
		}

		if (i <= j)
		{
			UInt tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	if (left < j)
	{
		QuickSort(arr, left, j);
	}

	if (i < right)
	{
		QuickSort(arr, i, right);
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
