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
	mpOpaqueObjects = WIRE_NEW VisibleSet(maxQuantity, growBy);
	mpTransparentObjects = WIRE_NEW VisibleSet(maxQuantity, growBy);
}

//----------------------------------------------------------------------------
CullerSorting::~CullerSorting()
{
	// The visible set created and stored in mVisibleSets in the constructor
	// is deleted by the destructor of the parent class, not here.
	WIRE_DELETE mpTransparentObjects;
	WIRE_DELETE mpOpaqueObjects;
}

//----------------------------------------------------------------------------
void CullerSorting::ComputeVisibleSet(Spatial* pScene)
{
	Culler::ComputeVisibleSet(pScene);
	Sort();
}

//----------------------------------------------------------------------------
void CullerSorting::Sort()
{
	WIRE_ASSERT(mVisibleSets.GetQuantity() >= 2);
	UnwrapEffectStackAndSort(mVisibleSets[0], mpOpaqueObjects);
	UnwrapEffectStackAndSort(mVisibleSets[1], mpTransparentObjects);

	VisibleSet* pTemp = mVisibleSets[0];
	mVisibleSets[0] = mpOpaqueObjects;
	mpOpaqueObjects = pTemp;

	pTemp = mVisibleSets[1];
	mVisibleSets[1] = mpTransparentObjects;
	mpTransparentObjects = pTemp;
}

//----------------------------------------------------------------------------
void CullerSorting::UnwrapEffectStackAndSort(VisibleSet* pSource, VisibleSet*
	pDestination)
{
	pDestination->Clear();

	// The destination set will have at least the size of the source set.
	if (pDestination->GetMaxQuantity() < pSource->GetMaxQuantity())
	{
		pDestination->SetMaxQuantity(pSource->GetMaxQuantity());
	}

	UInt indexStack[Effect::MAX_SIMULTANEOUS_EFFECTS][2];
	indexStack[0][0] = 0;
	indexStack[0][1] = 0;
	UInt top = 0;
	UInt left;

	const UInt visibleQuantity = pSource->GetQuantity();
	Object** pVisible;
	Transformation** pTransformations;
	pSource->GetSet(pVisible, pTransformations);

	for (UInt i = 0; i < visibleQuantity; i++)
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
					pDestination->Sort(indexStack[0][0], i-1);
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
				if (top == 0)
				{
					pDestination->Insert(pVisible[i], pTransformations[i],
						pSource->GetKey(i));
				}

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
			pDestination->Insert(pVisible[min], NULL);
			left = pDestination->GetQuantity();

			for (UInt j = min+1; j <= max; j++)
 			{
				RenderObject* pRenderObject = DynamicCast<RenderObject>(
					pVisible[j]);
				if (pRenderObject)
				{
					pDestination->Insert(pRenderObject, pTransformations[j],
						pSource->GetKey(j));
				}
			}

			pDestination->Sort(left, pDestination->GetQuantity()-1);
			pDestination->Insert(NULL, NULL);

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
		pDestination->Sort(indexStack[0][0], dstQty-1);
	}
}

//----------------------------------------------------------------------------
void CullerSorting::Insert(Object* pObject, Transformation* pTransformation,
	const Vector3F& rPosition)
{
	WIRE_ASSERT(mVisibleSets.GetQuantity() >= 2);

	RenderObject* pRenderObject = DynamicCast<RenderObject>(pObject);
	if (!pRenderObject)
	{
		GetVisibleSet(0)->Insert(pObject, NULL);
		GetVisibleSet(1)->Insert(pObject, NULL);
		return;
	}

	UInt key = GetKey(pRenderObject, rPosition);
	StateAlpha* pAlpha = StaticCast<StateAlpha>(pRenderObject->GetStates()[
		State::ALPHA]);
	if (pAlpha)
	{
		if (pAlpha->BlendEnabled)
		{
			GetVisibleSet(1)->Insert(pObject, pTransformation, key);
		}
		else
		{
			GetVisibleSet(0)->Insert(pObject, pTransformation, key);
		}
	}
	else
	{
		GetVisibleSet(0)->Insert(pObject, pTransformation, key);
	}
}

//----------------------------------------------------------------------------
UInt CullerSorting::GetKey(RenderObject* pRenderObject, const Vector3F&
	rPosition)
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

	Float z = GetCamera()->GetDVector().Dot(rPosition);
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

	// If StateSetID is MAX_UINT, it wasn't initialized (call UpdateRS() once,
	// or initialize manually)
	WIRE_ASSERT(pRenderObject->GetStateSetID() < (1<<STATESET));
	key |= pRenderObject->GetStateSetID() << (MATERIAL + DEPTH);

	return key;
}
