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
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		if (vertexColorChannels == 3)
		{
			pCubeVerts->Color3(i) = colors[i];
		}
		else if (vertexColorChannels == 4)
		{
			const ColorRGB& rCol = colors[i];
			ColorRGBA col = ColorRGBA(rCol.R(), rCol.G(), rCol.B(), 1.0F);
			pCubeVerts->Color4(i) = col;
		}

		if (useNormals)
		{
			// use platonic normal as default
			Vector3F normal = vertices[i];
			normal.Normalize();
			pCubeVerts->Normal3(i) =  normal;
		}
	}

	UInt indices[] = { 0, 2, 1,	0, 3, 2, 0, 1, 5, 0, 5, 4, 0, 4, 7,	0, 7, 3,
		6, 4, 5, 6, 7, 4, 6, 5, 1, 6, 1, 2,	6, 2, 3, 6, 3, 7, };

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for (UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
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
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];

		for (UInt unit = 0; unit < uvQuantity; unit++)
		{
			pCubeVerts->TCoord2(i, unit) = uvs[i];
		}

		if (vertexColorChannels == 3)
		{
			pCubeVerts->Color3(i) = colors[i];
		}
		else if (vertexColorChannels == 4)
		{
			const ColorRGB& rCol = colors[i];
			ColorRGBA col = ColorRGBA(rCol.R(), rCol.G(), rCol.B(), 1.0F);
			pCubeVerts->Color4(i) = col;
		}

		if (useNormals)
		{
			// use platonic normal as default
			Vector3F normal = vertices[i];
			normal.Normalize();
			pCubeVerts->Normal3(i) =  normal;
		}
	}

	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(6*6);
	for	(UInt i = 0; i < 6; i++)
	{
		(*pIndices)[0+i*6] = indices[0+i*4];
		(*pIndices)[1+i*6] = indices[1+i*4];
		(*pIndices)[2+i*6] = indices[3+i*4];

		(*pIndices)[3+i*6] = indices[3+i*4];
		(*pIndices)[4+i*6] = indices[1+i*4];
		(*pIndices)[5+i*6] = indices[2+i*4];
	}

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
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
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];

		for (UInt unit = 0; unit < uvQuantity; unit++)
		{
			pCubeVerts->TCoord2(i, unit) = uvs[i];
		}

		if (vertexColorChannels == 3)
		{
			pCubeVerts->Color3(i) = colors[i];
		}
		else if (vertexColorChannels == 4)
		{
			const ColorRGB& rCol = colors[i];
			ColorRGBA col = ColorRGBA(rCol.R(), rCol.G(), rCol.B(), 1.0F);
			pCubeVerts->Color4(i) = col;
		}

		if (useNormals)
		{
			// use platonic normal as default
			Vector3F normal = vertices[i];
			normal.Normalize();
			pCubeVerts->Normal3(i) =  normal;
		}
	}

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
	return pCube;
}
