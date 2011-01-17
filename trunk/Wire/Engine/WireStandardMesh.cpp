#include "WireStandardMesh.h"

#include "WireGeometry.h"

using namespace Wire;

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
	Float radius)
{
	VertexAttributes attr;
	attr.SetPositionChannels(3);  // channels: X, Y, Z

	Int iZSm1 = zSampleCount-1;
	Int iZSm2 = zSampleCount-2;
	Int iZSm3 = zSampleCount-3;
	Int iRSp1 = radialSampleCount+1;
	const Int vertexQuantity = iZSm2*iRSp1 + 2;
	const Int triangleQuantity = 2*iZSm2*radialSampleCount;
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attr, vertexQuantity);
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(3 * triangleQuantity);

	// generate geometry
	const Float invRS = 1.0F / static_cast<Float>(radialSampleCount);
	const Float zFactor = 2.0F / static_cast<Float>(iZSm1);
	Int iR;
	Int iZ;
	Int iZStart;
	Int i;

	// Generate points on the unit circle to be used in computing the mesh
	// points on a cylinder slice.
	Float* pSin = WIRE_NEW Float[iRSp1];
	Float* pCos = WIRE_NEW Float[iRSp1];
	for (iR = 0; iR < radialSampleCount; iR++)
	{
		Float angle = MathF::TWO_PI * invRS * iR;
		pCos[iR] = MathF::Cos(angle);
		pSin[iR] = MathF::Sin(angle);
	}

	pSin[radialSampleCount] = pSin[0];
	pCos[radialSampleCount] = pCos[0];

	// generate the cylinder itself
	for (iZ = 1, i = 0; iZ < iZSm1; iZ++)
	{
		const Float zFraction = -1.0F + zFactor * iZ;  // in (-1,1)
		const Float fZ = radius * zFraction;

		// compute center of slice
		Vector3F sliceCenter(0.0F, 0.0F, fZ);

		// compute radius of slice
		Float sliceRadius = MathF::Sqrt(MathF::FAbs(radius*radius - fZ*fZ));

		// compute slice vertices with duplication at end point
		Vector3F normal;
		Int save = i;
		for (iR = 0; iR < radialSampleCount; iR++)
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

			if (attr.GetTCoordChannelQuantity() > 0)
			{
				Vector2F tCoord = Vector2F(radialFraction, 0.5F *
					(zFraction+1.0F));
				for (UInt unit = 0; unit < attr.GetTCoordChannelQuantity();
					unit++)
				{
					if (attr.HasTCoord(unit))
					{
						pVBuffer->TCoord2(unit, i) = tCoord;
					}
				}
			}

			i++;
		}

		pVBuffer->Position3(i) = pVBuffer->Position3(save);
		if (attr.HasNormal())
		{
			pVBuffer->Normal3(i) = pVBuffer->Normal3(save);
		}

		if (attr.GetTCoordChannelQuantity() > 0)
		{
			Vector2F tCoord = Vector2F(1.0F, 0.5F * (zFraction+1.0F));
			for (UInt unit = 0; unit < attr.GetTCoordChannelQuantity();
				unit++)
			{
				if (attr.HasTCoord(unit))
				{
					pVBuffer->TCoord2(unit, i) = tCoord;
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

	if (attr.GetTCoordChannelQuantity() > 0)
	{
		Vector2F tCoord = Vector2F(0.5F, 0.5F);
		for (UInt unit = 0; unit < attr.GetTCoordChannelQuantity(); unit++)
		{
			if (attr.HasTCoord(unit))
			{
				pVBuffer->TCoord2(unit, i) = tCoord;
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

	if (attr.GetTCoordChannelQuantity() > 0)
	{
		Vector2F tCoord = Vector2F(0.5F, 1.0F);
		for (UInt unit = 0; unit < attr.GetTCoordChannelQuantity(); unit++)
		{
			if (attr.HasTCoord(unit))
			{
				pVBuffer->TCoord2(unit, i) = tCoord;
			}
		}
	}

	i++;
	WIRE_ASSERT(i == vertexQuantity);

	// generate connectivity
	UInt* pLocalIndex = pIBuffer->GetData();
	for (iZ = 0, iZStart = 0; iZ < iZSm3; iZ++)
	{
		Int i0 = iZStart;
		Int i1 = i0 + 1;
		iZStart += iRSp1;
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
	Int iVQm2 = vertexQuantity-2;
	for (i = 0; i < radialSampleCount; i++)
	{
		pLocalIndex[0] = i;
		pLocalIndex[1] = iVQm2;
		pLocalIndex[2] = i+1;
		pLocalIndex += 3;
	}

	// north pole triangles
	Int iVQm1 = vertexQuantity-1;
	Int offset = iZSm3 * iRSp1;
	for (i = 0; i < radialSampleCount; i++)
	{
		pLocalIndex[0] = i+offset;
		pLocalIndex[1] = i+1+offset;
		pLocalIndex[2] = iVQm1;
		pLocalIndex += 3;
	}

	WIRE_ASSERT(pLocalIndex == pIBuffer->GetData() + 3*triangleQuantity);

	WIRE_DELETE[] pCos;
	WIRE_DELETE[] pSin;

	Geometry* pMesh = WIRE_NEW Geometry(pVBuffer, pIBuffer);

	// The duplication of vertices at the seam cause the automatically
	// generated bounding volume to be slightly off center. Reset the bound
	// to use the true information.
	pMesh->ModelBound->SetCenter(Vector3F::ZERO);
	pMesh->ModelBound->SetRadius(radius);
	return pMesh;
}
