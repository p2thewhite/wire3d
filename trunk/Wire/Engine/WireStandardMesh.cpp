// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireStandardMesh.h"

#include "WireImage2D.h"
#include "WireGeometry.h"
#include "WireTexture2D.h"

using namespace Wire;

Texture2DPtr StandardMesh::s_spFontTexture;

WIRE_IMPLEMENT_TERMINATE(StandardMesh);

//----------------------------------------------------------------------------
void StandardMesh::Terminate()
{
	s_spFontTexture = NULL;
}

//----------------------------------------------------------------------------
Geometry* StandardMesh::CreateCube8(const UInt vertexColorChannels,
	const Bool useNormals, const Float extent)
{
	Vector3F vertices[] = {
		Vector3F(-extent, -extent, -extent),
		Vector3F(+extent, -extent, -extent),
		Vector3F(+extent, +extent, -extent),
		Vector3F(-extent, +extent, -extent),
		Vector3F(-extent, -extent, +extent),
		Vector3F(+extent, -extent, +extent),
		Vector3F(+extent, +extent, +extent),
		Vector3F(-extent, +extent, +extent)
	};

	const ColorRGB colors[] = {
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F), ColorRGB(0.0F, 0.0F, 0.0F)
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);	// XYZ

	if (vertexColorChannels > 0)
	{
		WIRE_ASSERT(vertexColorChannels == 3 || vertexColorChannels == 4);
		attributes.SetColorChannels(vertexColorChannels);	// RGB(A)
	}

	if (useNormals)
	{
		attributes.SetNormalChannels(3);	// channels: X, Y, Z
	}

	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pVBuffer->GetVertexQuantity(); i++)
	{
		pVBuffer->Position3(i) = vertices[i];
		if (vertexColorChannels == 3)
		{
			pVBuffer->Color3(i) = colors[i];
		}
		else if (vertexColorChannels == 4)
		{
			const ColorRGB& rCol = colors[i];
			ColorRGBA col = ColorRGBA(rCol.R(), rCol.G(), rCol.B(), 1.0F);
			pVBuffer->Color4(i) = col;
		}

		if (useNormals)
		{
			// use platonic normal as default
			Vector3F normal = vertices[i];
			normal.Normalize();
			pVBuffer->Normal3(i) =  normal;
		}
	}

	UInt indices[] = { 0, 2, 1,	0, 3, 2, 0, 1, 5, 0, 5, 4, 0, 4, 7,	0, 7, 3,
		6, 4, 5, 6, 7, 4, 6, 5, 1, 6, 1, 2,	6, 2, 3, 6, 3, 7, };

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(indexQuantity);
	for (UInt i = 0; i < indexQuantity; i++)
	{
		(*pIBuffer)[i] = indices[i];
	}

	Geometry* pCube = WIRE_NEW Geometry(pVBuffer, pIBuffer);
	return pCube;
}

//----------------------------------------------------------------------------
Geometry* StandardMesh::CreateCube14(const UInt vertexColorChannels,
	const UInt uvQuantity, const Bool useNormals, const Float extent)
{
	const Vector3F vertices[] = {
		Vector3F(-extent, -extent, -extent),
		Vector3F(-extent,  extent, -extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent, -extent, -extent),
		Vector3F( extent, -extent, extent),
		Vector3F( extent,  extent, extent),
		Vector3F(-extent,  extent, extent),
		Vector3F(-extent, -extent, extent),
		Vector3F( extent,  extent, extent),
		Vector3F(-extent,  extent, extent),
		Vector3F( extent,  extent, extent),
		Vector3F( extent, -extent, extent),
		Vector3F(-extent, -extent, extent),
		Vector3F( extent, -extent, extent)
	};

	const ColorRGB colors[] = {
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F), ColorRGB(0.0F, 0.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F)
	};

	const Vector2F uvs[] = {
		Vector2F(0.50F, 0.50F), Vector2F(0.50F, 0.25F),
		Vector2F(0.25F, 0.25F), Vector2F(0.25F, 0.50F),
		Vector2F(0.00F, 0.50F), Vector2F(0.00F, 0.25F),
		Vector2F(0.75F, 0.25F), Vector2F(0.75F, 0.50F),
		Vector2F(0.25F, 0.00F), Vector2F(0.50F, 0.00F),
		Vector2F(1.00F, 0.25F), Vector2F(1.00F, 0.50F),
		Vector2F(0.50F, 0.75F), Vector2F(0.25F, 0.75F)
	};

	const UInt indices[] = {
		0, 1, 2, 3,	11, 10, 6, 7, 7, 6, 1, 0, 3, 2, 5, 4, 1, 9, 8, 2, 12, 0,
		3, 13
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z

	for (UInt unit = 0; unit < uvQuantity; unit++)
	{
		attributes.SetTCoordChannels(2, unit);	// channels: U, V
	}

	if (vertexColorChannels > 0)
	{
		WIRE_ASSERT(vertexColorChannels == 3 || vertexColorChannels == 4);
		attributes.SetColorChannels(vertexColorChannels);	// RGB(A)
	}

	if (useNormals)
	{
		attributes.SetNormalChannels(3);	// channels: X, Y, Z
	}

	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(colors) / sizeof(ColorRGB)));	
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pVBuffer->GetVertexQuantity(); i++)
	{
		pVBuffer->Position3(i) = vertices[i];

		for (UInt unit = 0; unit < uvQuantity; unit++)
		{
			pVBuffer->TCoord2(i, unit) = uvs[i];
		}

		if (vertexColorChannels == 3)
		{
			pVBuffer->Color3(i) = colors[i];
		}
		else if (vertexColorChannels == 4)
		{
			const ColorRGB& rCol = colors[i];
			ColorRGBA col = ColorRGBA(rCol.R(), rCol.G(), rCol.B(), 1.0F);
			pVBuffer->Color4(i) = col;
		}

		if (useNormals)
		{
			// use platonic normal as default
			Vector3F normal = vertices[i];
			normal.Normalize();
			pVBuffer->Normal3(i) =  normal;
		}
	}

	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(6*6);
	for	(UInt i = 0; i < 6; i++)
	{
		(*pIBuffer)[0+i*6] = indices[0+i*4];
		(*pIBuffer)[1+i*6] = indices[1+i*4];
		(*pIBuffer)[2+i*6] = indices[3+i*4];

		(*pIBuffer)[3+i*6] = indices[3+i*4];
		(*pIBuffer)[4+i*6] = indices[1+i*4];
		(*pIBuffer)[5+i*6] = indices[2+i*4];
	}

	Geometry* pCube = WIRE_NEW Geometry(pVBuffer, pIBuffer);
	return pCube;
}

//----------------------------------------------------------------------------
Geometry* StandardMesh::CreateCube24(const UInt vertexColorChannels,
	const UInt uvQuantity, const Bool useNormals, const Float extent)
{
	const Vector3F vertices[] =
	{
		Vector3F(-extent,  extent,  extent),
		Vector3F( extent,  extent,  extent),
		Vector3F( extent, -extent,  extent),
		Vector3F(-extent, -extent,  extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent,  extent,  extent),
		Vector3F(-extent,  extent,  extent),
		Vector3F(-extent,  extent, -extent),
		Vector3F( extent, -extent,  extent),
		Vector3F( extent,  extent,  extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent, -extent, -extent),
		Vector3F(-extent,  extent, -extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent, -extent, -extent),
		Vector3F(-extent, -extent, -extent),
		Vector3F( extent, -extent, -extent),
		Vector3F( extent, -extent,  extent),
		Vector3F(-extent, -extent,  extent),
		Vector3F(-extent, -extent, -extent),
		Vector3F(-extent, -extent,  extent),
		Vector3F(-extent,  extent,  extent),
		Vector3F(-extent,  extent, -extent),
		Vector3F(-extent, -extent, -extent)
	};

	const ColorRGB colors[] = {
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F), ColorRGB(0.0F, 0.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F), ColorRGB(0.0F, 0.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F),
		ColorRGB(1.0F, 0.0F, 1.0F), ColorRGB(0.0F, 1.0F, 1.0F),
		ColorRGB(1.0F, 1.0F, 1.0F), ColorRGB(0.0F, 0.0F, 0.0F),
	};

	const Vector2F uvs[] =
	{
		Vector2F(0.0F, 0.0F), Vector2F(1.0F, 0.0F), Vector2F(1.0F, 1.0F),
		Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F),	Vector2F(1.0F, 0.0F),
		Vector2F(1.0F, 1.0F), Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F),
		Vector2F(1.0F, 0.0F), Vector2F(1.0F, 1.0F),	Vector2F(0.0F, 1.0F),
		Vector2F(0.0F, 0.0F), Vector2F(1.0F, 0.0F),	Vector2F(1.0F, 1.0F),
		Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F), Vector2F(1.0F, 0.0F),
		Vector2F(1.0F, 1.0F), Vector2F(0.0F, 1.0F), Vector2F(0.0F, 0.0F),
		Vector2F(1.0F, 0.0F), Vector2F(1.0F, 1.0F), Vector2F(0.0F, 1.0F)
	};

	const UInt indices[] =
	{
		0, 2, 1, 0, 3, 2, 4, 6, 5, 4, 7, 6,	8, 10, 9, 8, 11, 10, 12, 13, 14,
		12, 14, 15, 16, 17, 18,	16, 18, 19,	20, 21, 22,	20, 22, 23
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z

	for (UInt unit = 0; unit < uvQuantity; unit++)
	{
		attributes.SetTCoordChannels(2, unit);	// channels: U, V
	}

	if (useNormals)
	{
		attributes.SetNormalChannels(3);	// channels: X, Y, Z
	}

	if (vertexColorChannels > 0)
	{
		WIRE_ASSERT(vertexColorChannels == 3 || vertexColorChannels == 4);
		attributes.SetColorChannels(vertexColorChannels);	// RGB(A)
	}

	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	WIRE_ASSERT(vertexQuantity == (sizeof(colors) / sizeof(ColorRGB)));	
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pVBuffer->GetVertexQuantity(); i++)
	{
		pVBuffer->Position3(i) = vertices[i];

		for (UInt unit = 0; unit < uvQuantity; unit++)
		{
			pVBuffer->TCoord2(i, unit) = uvs[i];
		}

		if (vertexColorChannels == 3)
		{
			pVBuffer->Color3(i) = colors[i];
		}
		else if (vertexColorChannels == 4)
		{
			const ColorRGB& rCol = colors[i];
			ColorRGBA col = ColorRGBA(rCol.R(), rCol.G(), rCol.B(), 1.0F);
			pVBuffer->Color4(i) = col;
		}

		if (useNormals)
		{
			// use platonic normal as default
			Vector3F normal = vertices[i];
			normal.Normalize();
			pVBuffer->Normal3(i) =  normal;
		}
	}

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIBuffer)[i] = indices[i];
	}

	Geometry* pCube = WIRE_NEW Geometry(pVBuffer, pIBuffer);
	return pCube;
}

//----------------------------------------------------------------------------
Geometry* StandardMesh::CreatePlane(const UInt xTileCount,
	const UInt yTileCount, const Float xSizeTotal, const Float ySizeTotal,
	const UInt vertexColorChannels, const UInt uvQuantity,
	const Bool useNormals)
{
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	
	for (UInt unit = 0; unit < uvQuantity; unit++)
	{
		attributes.SetTCoordChannels(2, unit);	// channels: U, V
	}

	if (vertexColorChannels > 0)
	{
		WIRE_ASSERT(vertexColorChannels == 3 || vertexColorChannels == 4);
		attributes.SetColorChannels(vertexColorChannels);	// RGB(A)
	}

	if (useNormals)
	{
		attributes.SetNormalChannels(3);	// channels: X, Y, Z
	}

	const UInt vertexCount = (xTileCount+1) * (yTileCount+1);
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes, vertexCount);

	const Float xStride = xSizeTotal / xTileCount;
	const Float yStride = ySizeTotal / yTileCount;
	Float y = -ySizeTotal * 0.5F;
	for (UInt j = 0; j < yTileCount+1; j++)
	{
		Float x = -xSizeTotal * 0.5F;;
		for (UInt i = 0; i < xTileCount+1; i++)
		{
			const UInt idx = i + (xTileCount+1)*j;
			pVBuffer->Position3(idx) = Vector3F(x, y, 0);

			for (UInt unit = 0; unit < uvQuantity; unit++)
			{
				pVBuffer->TCoord2(i + (xTileCount+1)*j, unit) =
					Vector2F(x, y);
			}

			if (vertexColorChannels == 3)
			{
				pVBuffer->Color3(idx) = ColorRGB::WHITE;
			}
			else if (vertexColorChannels == 4)
			{
				pVBuffer->Color4(idx) = ColorRGBA::WHITE;
			}
			
			if (useNormals)
			{
				// use platonic normal as default
				Vector3F normal = pVBuffer->Position3(idx);
				normal.Normalize();
				pVBuffer->Normal3(idx) =  normal;
			}
			
			x += xStride;
		}

		y += yStride;
	}

	const UInt indexCount = xTileCount * yTileCount * 6;
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(indexCount);

	for (UInt j = 0; j < yTileCount; j++)
	{
		UInt offset = (xTileCount+1)*j;
		for (UInt i = 0; i < xTileCount; i++)
		{
			UInt index = xTileCount*j+i;
			UInt index0 = i+offset;
			UInt index1 = index0+xTileCount+1;
			UInt index2 = index0+1;
			UInt index3 = index0+xTileCount+2;

			(*pIBuffer)[index*6] = index0;
			(*pIBuffer)[index*6+1] = index2;
			(*pIBuffer)[index*6+2] = index1;

			(*pIBuffer)[index*6+3] = index1;
			(*pIBuffer)[index*6+4] = index2;
			(*pIBuffer)[index*6+5] = index3;

		}
	}

	Geometry* pGeo = WIRE_NEW Geometry(pVBuffer, pIBuffer);
	return pGeo;
}

//----------------------------------------------------------------------------
Geometry* StandardMesh::CreateQuad(const UInt vertexColorChannels,
	const UInt uvQuantity, const Bool useNormals, const Float extent)
{
	const Vector3F vertices[] =
	{
		Vector3F(-extent, extent, 0), Vector3F(extent, extent, 0),
		Vector3F(extent, -extent, 0), Vector3F(-extent, -extent, 0)
	};

	const Vector2F uvs[] =
	{
		Vector2F(0, 0), Vector2F(1, 0), Vector2F(1, 1), Vector2F(0, 1)
	};

	const ColorRGB colors[] = {
		ColorRGB(1.0F, 0.0F, 0.0F), ColorRGB(0.0F, 1.0F, 0.0F),
		ColorRGB(0.0F, 0.0F, 1.0F), ColorRGB(1.0F, 1.0F, 0.0F)
	};

	const UInt indices[] = { 0, 2, 1, 0, 3, 2 };

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z

	for (UInt unit = 0; unit < uvQuantity; unit++)
	{
		attributes.SetTCoordChannels(2, unit);	// channels: U, V
	}

	if (vertexColorChannels > 0)
	{
		WIRE_ASSERT(vertexColorChannels == 3 || vertexColorChannels == 4);
		attributes.SetColorChannels(vertexColorChannels);	// RGB(A)
	}

	if (useNormals)
	{
		attributes.SetNormalChannels(3);	// channels: X, Y, Z
	}

	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pVBuffer->GetVertexQuantity(); i++)
	{
 		pVBuffer->Position3(i) = vertices[i];

		for (UInt unit = 0; unit < uvQuantity; unit++)
		{
			pVBuffer->TCoord2(i) = uvs[i];
		}

		if (vertexColorChannels == 3)
		{
			pVBuffer->Color3(i) = colors[i];
		}
		else if (vertexColorChannels == 4)
		{
			const ColorRGB& rCol = colors[i];
			ColorRGBA col = ColorRGBA(rCol.R(), rCol.G(), rCol.B(), 1.0F);
			pVBuffer->Color4(i) = col;
		}
		
		if (useNormals)
		{
			// use platonic normal as default
			Vector3F normal = vertices[i];
			normal.Normalize();
			pVBuffer->Normal3(i) =  normal;
		}
	}

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIBuffer)[i] = indices[i];
	}

	Geometry* pQuad = WIRE_NEW Geometry(pVBuffer, pIBuffer);
	return pQuad;
}

//----------------------------------------------------------------------------
Geometry* StandardMesh::CreateSphere(Int zSampleCount, Int radialSampleCount,
	Float radius, const UInt uvQuantity, const UInt vertexColorChannels,
	const Bool useNormals)
{
	VertexAttributes attr;
	attr.SetPositionChannels(3);  // channels: X, Y, Z

	for (UInt unit = 0; unit < uvQuantity; unit++)
	{
		attr.SetTCoordChannels(2, unit);	// channels: U, V
	}

	if (vertexColorChannels > 0)
	{
		WIRE_ASSERT(vertexColorChannels == 3 || vertexColorChannels == 4);
		attr.SetColorChannels(vertexColorChannels);	// RGB(A)
	}

	if (useNormals)
	{
		attr.SetNormalChannels(3);	// channels: X, Y, Z
	}

	const Int vertexQuantity = (zSampleCount-2)*(radialSampleCount+1) + 2;
	const Int triangleQuantity = 2*(zSampleCount-2)*radialSampleCount;
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attr, vertexQuantity);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(3 * triangleQuantity);

	// generate geometry
	const Float invRS = 1.0F / static_cast<Float>(radialSampleCount);
	const Float zFactor = 2.0F / static_cast<Float>(zSampleCount-1);

	// Generate points on the unit circle to be used in computing the mesh
	// points on a cylinder slice.
	Float* pSin = WIRE_NEW Float[radialSampleCount+1];
	Float* pCos = WIRE_NEW Float[radialSampleCount+1];
	for (Int iR = 0; iR < radialSampleCount; iR++)
	{
		Float angle = MathF::TWO_PI * invRS * iR;
		pCos[iR] = MathF::Cos(angle);
		pSin[iR] = MathF::Sin(angle);
	}

	pSin[radialSampleCount] = pSin[0];
	pCos[radialSampleCount] = pCos[0];

	// generate the cylinder itself
	const UInt uvChannelCount = attr.GetTCoordChannelQuantity();
	Int i = 0;
	for (Int iZ = 1; iZ < zSampleCount-1; iZ++)
	{
		const Float zFraction = -1.0F + zFactor * iZ;  // in (-1,1)
		const Float z = radius * zFraction;

		// compute center of slice
		Vector3F sliceCenter(0.0F, 0.0F, z);

		// compute radius of slice
		Float sliceRadius = MathF::Sqrt(MathF::FAbs(radius*radius - z*z));

		// compute slice vertices with duplication at end point
		Vector3F normal;
		Int iSave = i;
		for (Int iR = 0; iR < radialSampleCount; iR++)
		{
			Float radialFraction = iR * invRS;  // in [0,1)
			Vector3F radial(pCos[iR], pSin[iR], 0.0F);
			pVBuffer->Position3(i) = sliceCenter + sliceRadius * radial;
			if (attr.HasNormal())
			{
				normal = pVBuffer->Position3(i);
				normal.Normalize();
				pVBuffer->Normal3(i) = normal;
			}

			if (vertexColorChannels == 3)
			{
				Vector3F v = pVBuffer->Position3(i);
				v.Normalize();
				pVBuffer->Color3(i) = ColorRGB(v.X(), v.Y(), v.Z());
			}
			else if (vertexColorChannels == 4)
			{
				Vector3F v = pVBuffer->Position3(i);
				v.Normalize();
				pVBuffer->Color4(i) = ColorRGBA(v.X(), v.Y(), v.Z(), 1);
			}

			if (uvChannelCount > 0)
			{
				Vector2F tCoord = Vector2F(radialFraction, 0.5F *
					(zFraction+1.0F));
				for (UInt unit = 0; unit < uvChannelCount; unit++)
				{
					if (attr.HasTCoord(unit))
					{
						pVBuffer->TCoord2(i, unit) = tCoord;
					}
				}
			}

			i++;
		}

		pVBuffer->Position3(i) = pVBuffer->Position3(iSave);
		if (attr.HasNormal())
		{
			pVBuffer->Normal3(i) = pVBuffer->Normal3(iSave);
		}

		if (vertexColorChannels == 3)
		{
			Vector3F v = pVBuffer->Position3(i);
			v.Normalize();
			pVBuffer->Color3(i) = ColorRGB(v.X(), v.Y(), v.Z());
		}
		else if (vertexColorChannels == 4)
		{
			Vector3F v = pVBuffer->Position3(i);
			v.Normalize();
			pVBuffer->Color4(i) = ColorRGBA(v.X(), v.Y(), v.Z(), 1);
		}

		if (uvChannelCount > 0)
		{
			Vector2F tCoord = Vector2F(1.0F, 0.5F * (zFraction+1.0F));
			for (UInt unit = 0; unit < uvChannelCount; unit++)
			{
				if (attr.HasTCoord(unit))
				{
					pVBuffer->TCoord2(i, unit) = tCoord;
				}
			}
		}

		i++;
	}

	// south pole
	pVBuffer->Position3(i) = -radius * Vector3F::UNIT_Z;
	if (attr.HasNormal())
	{
		pVBuffer->Normal3(i) = -Vector3F::UNIT_Z;
	}

	if (vertexColorChannels == 3)
	{
		Vector3F v = pVBuffer->Position3(i);
		v.Normalize();
		pVBuffer->Color3(i) = ColorRGB(v.X(), v.Y(), v.Z());
	}
	else if (vertexColorChannels == 4)
	{
		Vector3F v = pVBuffer->Position3(i);
		v.Normalize();
		pVBuffer->Color4(i) = ColorRGBA(v.X(), v.Y(), v.Z(), 1);
	}

	if (uvChannelCount > 0)
	{
		Vector2F tCoord = Vector2F(0.5F, 0.5F);
		for (UInt unit = 0; unit < uvChannelCount; unit++)
		{
			if (attr.HasTCoord(unit))
			{
				pVBuffer->TCoord2(i, unit) = tCoord;
			}
		}
	}

	i++;

	// north pole
	pVBuffer->Position3(i) = radius * Vector3F::UNIT_Z;
	if (attr.HasNormal())
	{
		pVBuffer->Normal3(i) = Vector3F::UNIT_Z;
	}

	if (vertexColorChannels == 3)
	{
		Vector3F v = pVBuffer->Position3(i);
		v.Normalize();
		pVBuffer->Color3(i) = ColorRGB(v.X(), v.Y(), v.Z());
	}
	else if (vertexColorChannels == 4)
	{
		Vector3F v = pVBuffer->Position3(i);
		v.Normalize();
		pVBuffer->Color4(i) = ColorRGBA(v.X(), v.Y(), v.Z(), 1);
	}

	if (uvChannelCount > 0)
	{
		Vector2F tCoord = Vector2F(0.5F, 1.0F);
		for (UInt unit = 0; unit < uvChannelCount; unit++)
		{
			if (attr.HasTCoord(unit))
			{
				pVBuffer->TCoord2(i, unit) = tCoord;
			}
		}
	}

	i++;
	WIRE_ASSERT(i == vertexQuantity);

	// generate connectivity
	UInt* pLocalIndex = pIBuffer->GetData();
	Int iZStart = 0;
	for (Int iZ = 0; iZ < zSampleCount-3; iZ++)
	{
		Int i0 = iZStart;
		Int i1 = i0 + 1;
		iZStart += radialSampleCount+1;
		Int i2 = iZStart;
		Int i3 = i2 + 1;
		for (i = 0; i < radialSampleCount; i++)
		{
			pLocalIndex[0] = i0++;
			pLocalIndex[1] = i1;
			pLocalIndex[2] = i2;
			pLocalIndex[3] = i1++;
			pLocalIndex[4] = i3++;
			pLocalIndex[5] = i2++;
			pLocalIndex += 6;
		}
	}

	// south pole triangles
	Int vQm2 = vertexQuantity-2;
	for (i = 0; i < radialSampleCount; i++)
	{
		pLocalIndex[0] = i;
		pLocalIndex[1] = vQm2;
		pLocalIndex[2] = i+1;
		pLocalIndex += 3;
	}

	// north pole triangles
	Int vQm1 = vertexQuantity-1;
	Int offset = (zSampleCount-3) * (radialSampleCount+1);
	for (i = 0; i < radialSampleCount; i++)
	{
		pLocalIndex[0] = i+offset;
		pLocalIndex[1] = i+1+offset;
		pLocalIndex[2] = vQm1;
		pLocalIndex += 3;
	}

	WIRE_ASSERT(pLocalIndex == pIBuffer->GetData() + 3*triangleQuantity);

	WIRE_DELETE[] pCos;
	WIRE_DELETE[] pSin;

	Geometry* pMesh = WIRE_NEW Geometry(pVBuffer, pIBuffer);

	// The duplication of vertices at the seam cause the automatically
	// generated bounding volume to be slightly off center. Reset the bound
	// to use the true information.
	pMesh->GetModelBound()->SetCenter(Vector3F::ZERO);
	pMesh->GetModelBound()->SetRadius(radius);
	return pMesh;
}

//----------------------------------------------------------------------------
Geometry* StandardMesh::CreateText(const Char* pText)
{
	if (!pText)
	{
		return NULL;
	}

	UInt textLength = System::Strlen(pText);
	UInt meshChars = 0;
	for (UInt i = 0; i < textLength; i++)
	{
		if (pText[i] > 32)
		{
			meshChars++;
		}
	}

	if (meshChars == 0)
	{
		return NULL;
	}

	if (!s_spFontTexture)
	{
		const UInt texWidth = 128;
		const UInt texHeight = 64;
		UChar* const pRaw = WIRE_NEW UChar[texWidth * texHeight * 3];
		UChar* pDst = pRaw;
		UChar* pSrc = const_cast<UChar*>(s_Font);

		for (UInt j = 0; j < 16*48; j++)
		{
			for (UInt i = 0; i < 8; i++)
			{
				if (*pSrc & (0x80 >> i))
				{
					*pDst++ = 0xFF;
					*pDst++ = 0xFF;
					*pDst++ = 0xFF;
				}
				else
				{
					*pDst++ = 0;
					*pDst++ = 0;
					*pDst++ = 0;
				}
			}

			pSrc++;
		}

		Image2D* pImage = WIRE_NEW Image2D(Image2D::FM_RGB888, texWidth,
			texHeight, pRaw, false);
		s_spFontTexture = WIRE_NEW Texture2D(pImage);
		s_spFontTexture->SetFilterType(Texture2D::FT_NEAREST);
	}

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetTCoordChannels(2);
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes, meshChars*4);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(meshChars*6);
	const UInt indices[] = { 0, 1, 2, 0, 2, 3 };

	Float x = 0;
	Float y = 0;
	Float xStride = 1.0F;
	Float yStride = 1.0F;
	const Float cw = 8.0F / s_spFontTexture->GetImage()->GetBound(0);
	const Float ch = 8.0F / s_spFontTexture->GetImage()->GetBound(1);
	UInt k = 0;
	for (UInt i = 0; i < textLength; i++)
	{
		if (pText[i] > 32)
		{
			UInt offset = static_cast<UInt>(pText[i] - 32);
			UInt offsetY = offset / 16;
			UInt offsetX = offset % 16;
			Float u = offsetX * cw;
			Float v = offsetY * ch;

			pVBuffer->Position3(k*4) = Vector3F(x, y, 0);
			pVBuffer->Position3(k*4+1) = Vector3F(x+xStride, y, 0);
			pVBuffer->Position3(k*4+2) = Vector3F(x+xStride, y+yStride, 0);
			pVBuffer->Position3(k*4+3) = Vector3F(x, y+yStride, 0);
		
			pVBuffer->TCoord2(k*4) = Vector2F(u, v);
 			pVBuffer->TCoord2(k*4+1) = Vector2F(u+cw, v);
 			pVBuffer->TCoord2(k*4+2) = Vector2F(u+cw, v+ch);
 			pVBuffer->TCoord2(k*4+3) = Vector2F(u, v+ch);

			for (UInt j = 0; j < (sizeof(indices) / sizeof(UInt)); j++)
			{
				(*pIBuffer)[k*6+j] = indices[j] + k*4;
			}

			k++;
		}

		x += xStride;

		if (pText[i] == '\n')
		{
			x = 0;
			y += yStride;
		}
	}

	Geometry* pGeo = WIRE_NEW Geometry(pVBuffer, pIBuffer);

	Material* pMaterial = WIRE_NEW Material;
	pMaterial->AddTexture(s_spFontTexture, Material::BM_REPLACE);
	pGeo->SetMaterial(pMaterial);

	return pGeo;
}

const UChar StandardMesh::s_Font[] = 
{
	0,16,40,40,16,96,32,16,16,16,16,0,0,0,0,0,0,16,40,40,60,100,80,16,32,8,84,
	16,0,0,0,4,0,16,40,124,80,8,80,16,64,4,56,16,0,0,0,8,0,16,0,40,56,16,32,0,
	64,4,16,124,0,124,0,16,0,16,0,124,20,32,84,0,64,4,56,16,16,0,0,32,0,0,0,
	40,120,76,72,0,32,8,84,16,16,0,0,64,0,16,0,40,16,12,52,0,16,16,16,0,32,0,
	16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,16,56,124,8,124,28,124,56,56,0,0,
	8,0,32,56,68,48,68,4,24,64,32,4,68,68,0,0,16,0,16,68,76,16,4,8,40,120,64,
	8,68,68,16,16,32,124,8,8,84,16,24,24,72,4,120,16,56,60,0,0,64,0,4,16,100,
	16,32,4,124,4,68,32,68,4,16,16,32,124,8,16,68,16,64,68,8,68,68,32,68,8,0,
	16,16,0,16,0,56,56,124,56,8,56,56,32,56,112,0,32,8,0,32,16,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,56,16,120,56,120,124,124,60,68,56,4,68,64,68,68,56,68,
	40,68,68,68,64,64,64,68,16,4,72,64,108,68,68,84,68,68,64,68,64,64,64,68,
	16,4,80,64,84,100,68,92,68,120,64,68,120,120,64,124,16,4,96,64,84,84,68,
	88,124,68,64,68,64,64,76,68,16,4,80,64,68,76,68,64,68,68,68,68,64,64,68,
	68,16,68,72,64,68,68,68,60,68,120,56,120,124,64,60,68,56,56,68,124,68,68,
	56,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,120,56,120,56,124,68,68,68,68,68,124,
	124,0,124,0,0,68,68,68,68,16,68,68,68,68,68,4,96,64,12,0,0,68,68,68,64,16,
	68,68,68,40,40,8,96,32,12,16,0,120,68,120,56,16,68,68,84,16,16,16,96,16,
	12,40,0,64,84,80,4,16,68,68,84,40,16,32,96,8,12,68,0,64,72,72,68,16,68,40,
	108,68,16,64,96,4,12,0,0,64,52,68,56,16,56,16,68,68,16,124,124,0,124,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,254,32,0,64,0,4,0,24,0,64,16,8,64,48,0,0,0,
	16,0,64,0,4,0,36,0,64,0,0,64,16,0,0,0,8,56,120,60,60,56,32,56,120,48,24,
	68,16,108,120,56,0,4,68,64,68,68,120,68,68,16,8,72,16,84,68,68,0,60,68,64,
	68,124,32,68,68,16,8,112,16,84,68,68,0,68,68,64,68,64,32,60,68,16,8,72,16,
	84,68,68,0,60,120,60,60,60,32,4,68,56,72,68,56,68,68,56,0,0,0,0,0,0,0,56,
	0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,16,112,52,0,0,0,0,0,32,0,0,0,0,
	0,0,48,16,24,88,84,120,60,92,60,120,68,68,68,68,68,124,48,16,24,0,40,68,
	68,96,64,32,68,68,68,40,68,8,96,16,12,0,84,68,68,64,56,32,68,68,84,16,68,
	16,48,16,24,0,40,120,60,64,4,36,76,40,84,40,60,32,48,16,24,0,84,64,4,64,
	120,24,52,16,108,68,4,124,28,16,112,0,0,64,4,0,0,0,0,0,0,0,56,0,0,16,0,0,0
};
