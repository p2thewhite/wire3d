#include "WireGeometry.h"
#include "WireCuller.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Geometry, Spatial);

//----------------------------------------------------------------------------
Geometry::Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer)
 	:
 	VBuffer(pVBuffer),
	IBuffer(pIBuffer),
	ModelBound(BoundingVolume::Create())
{
	System::Memset(States, 0, GlobalState::MAX_STATE_TYPE * sizeof(
		GlobalState*));
	UpdateModelBound();
}

//----------------------------------------------------------------------------
Geometry::~Geometry()
{
}

//----------------------------------------------------------------------------
void Geometry::UpdateModelBound()
{
	ModelBound->ComputeFromData(VBuffer);
}

//----------------------------------------------------------------------------
void Geometry::UpdateWorldBound()
{
	ModelBound->TransformBy(World, WorldBound);
}

//----------------------------------------------------------------------------
void Geometry::UpdateState(TArray<GlobalState*>* pGStack,
	TArray<Light*>* pLStack)
{
	// update global state
	for (UInt i = 0; i < GlobalState::MAX_STATE_TYPE; i++)
	{
		TArray<GlobalState*>& rState = pGStack[i];
  		States[i] = rState[rState.GetQuantity()-1];
	}

	// update light state
	Lights.RemoveAll();
	Lights.SetQuantity(pLStack->GetQuantity());
	for (UInt i = 0; i < pLStack->GetQuantity(); i++)
	{
		Lights[i] = (*pLStack)[i];
	}
}

//----------------------------------------------------------------------------
void Geometry::GetVisibleSet(Culler& rCuller, Bool)
{
	rCuller.Insert(this, NULL);
}

//----------------------------------------------------------------------------
void Geometry::GenerateNormals(Bool ignoreHardEdges)
{
	if (!VBuffer || !IBuffer)
	{
		return;
	}

	if (VBuffer->GetAttributes().GetNormalChannels() != 3)
	{
		return;
	}

	UInt* pIndices = IBuffer->GetData();

	// calculate the normals of the individual triangles
	TArray<Vector3F> faceNormals(IBuffer->GetIndexQuantity()/3);
	for (UInt i = 0; i < IBuffer->GetIndexQuantity(); i +=3)
	{
		Vector3F v1 = VBuffer->Position3(pIndices[i+1]) - VBuffer->Position3(
			pIndices[i]);
		Vector3F v2 = VBuffer->Position3(pIndices[i+2]) - VBuffer->Position3(
			pIndices[i+1]);

		Vector3F normal = v1.Cross(v2);
		normal.Normalize();
		faceNormals.Append(normal);
	}

	// collect the triangles each vertex is part of
	TArray<TArray<UInt> > buckets(VBuffer->GetVertexQuantity());
	buckets.SetQuantity(VBuffer->GetVertexQuantity());
	for (UInt i = 0; i < VBuffer->GetVertexQuantity(); i++)
	{
		TArray<UInt> indenticalVertices;
		if (!ignoreHardEdges)
		{
			indenticalVertices.Append(i);
		}
		else
		{
			for (UInt k = 0; k < VBuffer->GetVertexQuantity(); k++)
			{
				Vector3F pos = VBuffer->Position3(i);
				if (pos == VBuffer->Position3(k))
				{
					indenticalVertices.Append(k);
				}
			}
		}

		for (UInt j = 0; j < IBuffer->GetIndexQuantity(); j += 3)
		{
			for (UInt k = 0; k < indenticalVertices.GetQuantity(); k++)
			{
				UInt i1 = indenticalVertices[k];
				if (pIndices[j] == i1 || pIndices[j+1] == i1 ||
					pIndices[j+2] == i1)
				{
					buckets[i].Append(j/3);
				}
			}
		}
	}

	// calculate the normal of the vertex from the normals of its faces
	for (UInt i = 0; i < buckets.GetQuantity(); i++)
	{
		Vector3F normal(Vector3F::ZERO);
		for (UInt j = 0; j < buckets[i].GetQuantity(); j++)
		{
			normal += faceNormals[buckets[i][j]];
		}

		if (buckets[i].GetQuantity() > 0)
		{
			normal /= static_cast<Float>(buckets[i].GetQuantity());
			normal.Normalize();
		}
		else
		{
			// vertex not used in mesh, use a default normal
			normal = Vector3F::UNIT_X;
		}

		VBuffer->Normal3(i) = normal;
	}
}
