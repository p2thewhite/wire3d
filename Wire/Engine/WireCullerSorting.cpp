// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireCullerSorting.h"

#include "WireEffect.h"
#include "WireGeometry.h"
#include "WireLight.h"
#include "WireMaterial.h"
#include "WireMesh.h"
#include "WireRenderer.h"
#include "WireStateAlpha.h"

using namespace Wire;

//----------------------------------------------------------------------------
CullerSorting::CullerSorting(const Camera* pCamera, UInt maxQuantity,
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
CullerSorting::~CullerSorting()
{
	// The visible set created and stored in mVisibleSets in the constructor
	// is deleted by the destructor of the parent class, not here.
	WIRE_DELETE mpTransparentGeometry;
	WIRE_DELETE mpOpaqueGeometry;
}

//----------------------------------------------------------------------------
void CullerSorting::ComputeVisibleSet(Spatial* pScene)
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
void CullerSorting::UnwrapEffectStackAndSort(VisibleSet* pSource, VisibleSet*
	pDestination)
{
	pDestination->Clear();
	mKeys.SetQuantity(0, false);

	// The destination set will have at least the size of the source set.
	if (pDestination->GetMaxQuantity() < pSource->GetMaxQuantity())
	{
		pDestination->SetMaxQuantity(pSource->GetQuantity());
	}

	if (mKeys.GetMaxQuantity() < pSource->GetMaxQuantity())
	{
		mKeys.SetMaxQuantity(pSource->GetQuantity());
	}

	UInt indexStack[Renderer::MAX_GLOBAL_EFFECTS][2];
	indexStack[0][0] = 0;
	indexStack[0][1] = 0;
	UInt top = 0;
	UInt left;

	const UInt visibleQuantity = pSource->GetQuantity();
	VisibleObject* const pVisible = pSource->GetVisible();
	VisibleObject* const pVisibleDst = pDestination->GetVisible();

	for (UInt i = 0; i < visibleQuantity; i++)
	{
		if (pVisible[i].Object)
		{
			if (pVisible[i].GlobalEffect)
			{
				if (indexStack[0][0] < indexStack[0][1])
				{
					WIRE_ASSERT(i == indexStack[0][1]); // TODO check
					// Sort leaves with no effect
					QuickSort(mKeys, pDestination->GetVisible(),
						indexStack[0][0], i-1);
				}

				// Begin the scope of a global effect.
				top++;
				WIRE_ASSERT(top < Renderer::MAX_GLOBAL_EFFECTS);
				indexStack[top][0] = i;
				indexStack[top][1] = i;
			}
			else
			{
				// Found a leaf Geometry object.
				if (top == 0)
				{
					WIRE_ASSERT(DynamicCast<Geometry>(pVisible[i].Object));					
					pDestination->Insert(pVisible[i].Object, NULL);
					mKeys.Append(GetKey(pVisible[i].Object));
				}

				indexStack[top][1]++;
			}
		}
		else
		{
			// End the scope of a global effect.
			WIRE_ASSERT(!pVisible[i].GlobalEffect);
			UInt min = indexStack[top][0];
			UInt max = indexStack[top][1];

			pDestination->Insert(pVisible[min].Object, pVisible[min].
				GlobalEffect);
			mKeys.Append(0);	// dummy key to pad the array
			left = pDestination->GetQuantity();

			for (UInt j = min+1; j <= max; j++)
 			{
				if (pVisible[j].IsDrawable())
				{
					pDestination->Insert(pVisible[j].Object, NULL);
					mKeys.Append(GetKey(pVisible[j].Object));
				}
			}

// TODO: clean-up
//			QuickSort(mKeys.GetArray(), pDestination->GetVisible(), left,
			QuickSort(mKeys, pDestination->GetVisible(), left,
				pDestination->GetQuantity()-1);

			pDestination->Insert(NULL, NULL);
			mKeys.Append(0);

			WIRE_ASSERT(top > 0 /* More 'ends' than 'starts'*/); // TODO
			--top;
			indexStack[top][1] = max + 1;
			indexStack[0][0] = pDestination->GetQuantity();
			indexStack[0][1] = pDestination->GetQuantity();
		}
	}

	WIRE_ASSERT(top == 0);
	if (indexStack[0][0] < pDestination->GetQuantity()-1)
	{
//	QuickSort(mKeys.GetArray(), pDestination->GetVisible(), indexStack[0][0],
		QuickSort(mKeys, pDestination->GetVisible(), indexStack[0][0],
			pDestination->GetQuantity()-1);
	}
}

//----------------------------------------------------------------------------
//void CullerSorting::QuickSort(UInt* const pKeys, VisibleObject* const
void CullerSorting::QuickSort(TArray<UInt>& pKeys, VisibleObject* const
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
void CullerSorting::Insert(Spatial* pObject, Effect* pGlobalEffect)
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

//----------------------------------------------------------------------------
UInt CullerSorting::GetKey(Spatial* pSpatial)
{
	WIRE_ASSERT(pSpatial);
	WIRE_ASSERT(DynamicCast<Geometry>(pSpatial));

	Geometry* pGeometry = StaticCast<Geometry>(pSpatial);
	UInt key = 0;

	// number of bits we use for the sorting key
	enum
	{
		MATERIAL = 10,
		STATESET = 10,
		DEPTH = 12
	};
	
	 // The sum of the ranges must fit in the key
	WIRE_ASSERT((MATERIAL + STATESET + DEPTH) <= sizeof(key) * 8);

	Float z = GetCamera()->GetLocation().Z() - pGeometry->WorldBound->
		GetCenter().Z();
	const Float far = GetCamera()->GetDMax();
	const Float far3 = far*3;

	z += far;
	z /= far3;

	z = z < 0 ? 0 : z;
	z = z >= 1.0f ? 1.0f - MathF::ZERO_TOLERANCE : z;

	key = static_cast<UInt>(z * (1<<DEPTH));
	if (pGeometry->States[State::ALPHA] && StaticCast<StateAlpha>(pGeometry->
		States[State::ALPHA])->BlendEnabled)
	{
		key = (((1<<DEPTH)-1) - key);
	}

	WIRE_ASSERT(key < (1<<DEPTH));

	// The following asserts let you know when you have created more materials
	// and state sets than the key can handle. 
	Material* pMaterial = pGeometry->GetMaterial();
	if (pMaterial)
	{
		WIRE_ASSERT(pMaterial->ID < (1<<MATERIAL));
		key |= pMaterial->ID << DEPTH;
	}

	WIRE_ASSERT(pGeometry->StateSetID < (1<<STATESET));
	key |= pGeometry->StateSetID << (MATERIAL + DEPTH);

	return key;
}
