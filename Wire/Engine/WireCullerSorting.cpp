// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireCullerSorting.h"

#include "WireEffect.h"
#include "WireRenderer.h"
#include "WireRenderObject.h"

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
		pDestination->SetMaxQuantity(pSource->GetMaxQuantity());
	}

	if (mKeys.GetMaxQuantity() < pSource->GetMaxQuantity())
	{
		mKeys.SetMaxQuantity(pSource->GetMaxQuantity());
	}

	UInt indexStack[Renderer::MAX_GLOBAL_EFFECTS][2];
	indexStack[0][0] = 0;
	indexStack[0][1] = 0;
	UInt top = 0;
	UInt left;

	const UInt visibleQuantity = pSource->GetQuantity();
	Object** const pVisible = pSource->GetVisible();

	for (UInt i = 0; i < visibleQuantity; i++)
	{
		if (pVisible[i])
		{
			if (DynamicCast<Effect>(pVisible[i]))
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
				// Found a leaf object.
				WIRE_ASSERT(DynamicCast<RenderObject>(pVisible[i]));
				if (top == 0)
				{
					pDestination->Insert(pVisible[i]);
					mKeys.Append(GetKey(StaticCast<RenderObject>(pVisible[i])));
				}

				indexStack[top][1]++;
			}
		}
		else
		{
			// End the scope of a global effect.
			UInt min = indexStack[top][0];
			UInt max = indexStack[top][1];

			WIRE_ASSERT(DynamicCast<Effect>(pVisible[min]));
			pDestination->Insert(pVisible[min]);
			mKeys.Append(0);	// dummy key to pad the array
			left = pDestination->GetQuantity();

			for (UInt j = min+1; j <= max; j++)
 			{
				RenderObject* pRenderObject = DynamicCast<RenderObject>(
					pVisible[j]);
				if (pRenderObject)
				{
					pDestination->Insert(pVisible[j]);
					mKeys.Append(GetKey(pRenderObject));
				}
			}

// TODO: clean-up
//			QuickSort(mKeys.GetArray(), pDestination->GetVisible(), left,
			QuickSort(mKeys, pDestination->GetVisible(), left,
				pDestination->GetQuantity()-1);

			pDestination->Insert(NULL);
			mKeys.Append(0);

			WIRE_ASSERT(top > 0 /* More 'ends' than 'starts'*/); // TODO
			--top;
			indexStack[top][1] = max + 1;
			indexStack[0][0] = pDestination->GetQuantity();
			indexStack[0][1] = pDestination->GetQuantity();
		}
	}

	WIRE_ASSERT(top == 0);
	UInt dstQty = pDestination->GetQuantity();
	if ((dstQty > 0) && (indexStack[0][0] < dstQty-1))
	{
//	QuickSort(mKeys.GetArray(), pDestination->GetVisible(), indexStack[0][0],
		QuickSort(mKeys, pDestination->GetVisible(), indexStack[0][0],
			dstQty-1);
	}
}

//----------------------------------------------------------------------------
//void CullerSorting::QuickSort(UInt* const pKeys, Object** const pVisible,
void CullerSorting::QuickSort(TArray<UInt>& pKeys, Object** const pVisible,
	Int left, Int right)
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

			WIRE_ASSERT((DynamicCast<RenderObject>(pVisible[i])));
			Object* pTmp = pVisible[i];
			pVisible[i] = pVisible[j];
			pVisible[j] = pTmp;

			i++;
			j--;
		}
	};

	if (left < j)
	{
		QuickSort(pKeys, pVisible, left, j);
	}

	if (i < right)
	{
		QuickSort(pKeys, pVisible, i, right);
	}
}

//----------------------------------------------------------------------------
void CullerSorting::Insert(Object* pObject)
{
	WIRE_ASSERT(mVisibleSets.GetQuantity() >= 2);

	RenderObject* pRenderObject = DynamicCast<RenderObject>(pObject);
	if (!pRenderObject)
	{
		GetVisibleSet(0)->Insert(pObject);
		GetVisibleSet(1)->Insert(pObject);
		return;
	}

	StateAlpha* pAlpha = StaticCast<StateAlpha>(pRenderObject->GetStates()[
		State::ALPHA]);
	if (pAlpha)
	{
		if (pAlpha->BlendEnabled)
		{
			GetVisibleSet(1)->Insert(pObject);
		}
		else
		{
			GetVisibleSet(0)->Insert(pObject);
		}
	}
	else
	{
		WIRE_ASSERT(false/* there's no AlphaState, call UpdateRS() to init*/);
		GetVisibleSet(0)->Insert(pObject);
	}
}

//----------------------------------------------------------------------------
UInt CullerSorting::GetKey(RenderObject* pRenderObject)
{
	WIRE_ASSERT(pRenderObject);
	UInt key = 0;

	// number of bits we use for the sorting key (MSB to LSB)
	enum
	{
		STATESET = 8,
		MATERIAL = 8,
		DEPTH = 16
	};
	
	 // The sum of the ranges must fit in the key
	WIRE_ASSERT((STATESET + MATERIAL + DEPTH) <= sizeof(key) * 8);

	const Vector3F& rCamPos = GetCamera()->GetLocation();
	Vector3F pos;
	if (pRenderObject->WorldBound)
	{
		pos = pRenderObject->WorldBound->GetCenter() - rCamPos;
	}
	else
	{
		// if no world bound was supplied, we use the world translation 
		// instead (depending on the actual vertex positions, this can be
		// very inaccurate)
		pos = pRenderObject->World.GetTranslate() - rCamPos;
	}

	Float z = GetCamera()->GetDVector().Dot(pos);
	const Float far = GetCamera()->GetDMax();
	const Float far3 = far*3;

	z += far;
	z /= far3;

	z = z < 0 ? 0 : z;
	z = z >= 1.0F ? 1.0F - MathF::ZERO_TOLERANCE : z;

	key = static_cast<UInt>(z * (1<<DEPTH));
	StateAlpha* pStateAlpha = StaticCast<StateAlpha>(pRenderObject->
		GetStates()[State::ALPHA]);
	if (pStateAlpha && pStateAlpha->BlendEnabled)
	{
		key = (((1<<DEPTH)-1) - key);
	}

	WIRE_ASSERT(key < (1<<DEPTH));

	// The following asserts let you know when you have created more materials
	// and state sets than the key can handle. 
	Material* pMaterial = pRenderObject->GetMaterial();
	if (pMaterial)
	{
		WIRE_ASSERT(pMaterial->ID < (1<<MATERIAL));
		key |= pMaterial->ID << DEPTH;
	}

	// If StateSetID is MAX_UINT, it wasn't initialized (call UpdateRS() once)
	WIRE_ASSERT(pRenderObject->GetStateSetID() < (1<<STATESET));
	key |= pRenderObject->GetStateSetID() << (MATERIAL + DEPTH);

	return key;
}
