// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESTANDARDMESH_H
#define WIRESTANDARDMESH_H

#include "WireMain.h"
#include "WireTexture2D.h"
#include "WireVector3.h"
#include "WireTypes.h"

namespace Wire
{

class Geometry;
class RenderObject;
class Text;

class StandardMesh
{
	WIRE_DECLARE_TERMINATE;

public:
	// Helper class for creating standard meshes centered at (0,0,0).
	// Parameters:
	//     vertexColorChannels: 0 (none), 3 (rgb) or 4 (rgba)
	//     uvQuanity: number of uv sets to generate
	//     useNormals: true (required for lighting), false (no normals)

	// cube using 8 vertices
	static RenderObject* CreateCube8(const UInt vertexColorChannels = 0,
		const Bool useNormals = false, const Float extent = 1.0F);
	static Geometry* CreateCube8AsNode(const UInt vertexColorChannels = 0,
		const Bool useNormals = false, const Float extent = 1.0F);

	// cube using 14 vertices (for cross-like uv mapping)
 	static RenderObject* CreateCube14(const UInt vertexColorChannels = 0,
 		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);
	static Geometry* CreateCube14AsNode(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// cube using 24 vertices (each side has its own uv coordinates)
	static RenderObject* CreateCube24(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);
	static Geometry* CreateCube24AsNode(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// plane (mesh subdivided in x, y direction)
	static RenderObject* CreatePlane(const UInt xTileCount,
		const UInt yTileCount, const Float xTotalSize, const Float yTotalSize,
		const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false);
	static Geometry* CreatePlaneAsNode(const UInt xTileCount,
		const UInt yTileCount, const Float xTotalSize, const Float yTotalSize,
		const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false);

	// quad consisting of 2 triangles.
	static RenderObject* CreateQuad(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);
	static Geometry* CreateQuadAsNode(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// cylinder with axis through z-axis
	static RenderObject* CreateCylinder(Int axisSampleCount,
		Int radialSampleCount, const Float radius, const Float height,
		const UInt uvQuantity, const UInt vertexColorChannels,
		const Bool useNormals = false);
	static Geometry* CreateCylinderAsNode(Int axisSampleCount,
		Int radialSampleCount, const Float radius, const Float height,
		const UInt uvQuantity, const UInt vertexColorChannels,
		const Bool useNormals = false);

	// sphere with poles going through z-axis
	static RenderObject* CreateSphere(Int zSampleCount, Int radialSampleCount,
		Float radius, const UInt uvQuantity = 0,
		const UInt vertexColorChannels = 0, const Bool useNormals = false);
	static Geometry* CreateSphereAsNode(Int zSampleCount,
		Int radialSampleCount, Float radius, const UInt uvQuantity = 0,
		const UInt vertexColorChannels = 0, const Bool useNormals = false);

	// text using the built-in font
	static Text* CreateText(UInt maxLength = 4000);

private:
	static Geometry* CreateNode(RenderObject* pRenderObject);

	static Texture2DPtr s_spFontTexture;
	static const UChar s_Font[];
};

WIRE_REGISTER_TERMINATE(StandardMesh);

}

#endif
