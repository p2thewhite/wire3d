#pragma once
#ifndef WIRESTANDARDMESH_H
#define WIRESTANDARDMESH_H

#include "WireTypes.h"

namespace Wire
{

class Geometry;

class StandardMesh
{
public:
	// Standard meshes centered at (0,0,0)	

	// Cube using 8 vertices
	static Geometry* CreateCube8(const UInt vertexColorChannels = 0,
		const Bool useNormals = false, const Float extent = 1.0F);

	// Cube using 14 vertices (for cross-like uv mapping)
 	static Geometry* CreateCube14(const UInt vertexColorChannels = 0,
 		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// Cube using 24 vertices (each side has its own uv coordinates)
	static Geometry* CreateCube24(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// Plane (mesh subdivided in x, y direction)
	static Geometry* CreatePlane(const UInt xTileCount, const UInt yTileCount,
		const Float xTotalSize, const Float yTotalSize,
		const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false);

	// Quad consisting of 2 triangles.
	static Geometry* CreateQuad(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	static Geometry* CreateSphere(Int zSampleCount, Int radialSampleCount,
		Float radius);
};

}

#endif
