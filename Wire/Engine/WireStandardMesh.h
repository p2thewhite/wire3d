// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESTANDARDMESH_H
#define WIRESTANDARDMESH_H

#include "WireMain.h"
#include "WireTexture2D.h"
#include "WireTypes.h"

namespace Wire
{

class ColorRGBA;
class Geometry;

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
	static Geometry* CreateCube8(const UInt vertexColorChannels = 0,
		const Bool useNormals = false, const Float extent = 1.0F);

	// cube using 14 vertices (for cross-like uv mapping)
 	static Geometry* CreateCube14(const UInt vertexColorChannels = 0,
 		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// cube using 24 vertices (each side has its own uv coordinates)
	static Geometry* CreateCube24(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// plane (mesh subdivided in x, y direction)
	static Geometry* CreatePlane(const UInt xTileCount, const UInt yTileCount,
		const Float xTotalSize, const Float yTotalSize,
		const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false);

	// quad consisting of 2 triangles.
	static Geometry* CreateQuad(const UInt vertexColorChannels = 0,
		const UInt uvQuantity = 0, const Bool useNormals = false,
		const Float extent = 1.0F);

	// sphere with poles going through z-axis
	static Geometry* CreateSphere(Int zSampleCount, Int radialSampleCount,
		Float radius, const UInt uvQuantity = 0,
		const UInt vertexColorChannels = 0, const Bool useNormals = false);

	// text mesh using the following control characters:
	// "\n" newline
	// "\1\R\G\B\A" use color RGBA (within value range [1,255])
	// "\2" use supplied rColor
	static Geometry* CreateText(const Char* pText, const Float screenWidth,
		const Float screenHeight, const ColorRGBA& rColor);

private:
	static Texture2DPtr s_spFontTexture;
	static const UChar s_Font[];
};

WIRE_REGISTER_TERMINATE(StandardMesh);

}

#endif
