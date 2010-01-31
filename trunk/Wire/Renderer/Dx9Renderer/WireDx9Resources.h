#pragma once
#ifndef WIREDX9RESOURCES_H
#define WIREDX9RESOURCES_H

#include "../../Engine/WireResourceIdentifier.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ IBufferID : public ResourceIdentifier
{
public:
	IDirect3DIndexBuffer9* ID;
};

class /*WIRE_RENDERER_ITEM*/ VBufferID : public ResourceIdentifier
{
public:
	IDirect3DVertexBuffer9* ID;
	IDirect3DVertexDeclaration9* Declaration;
	UInt VertexSize;
};

class /*WIRE_RENDERER_ITEM*/ TextureID : public ResourceIdentifier
{
public:
	IDirect3DBaseTexture9* ID;
	Texture* TextureObject;
};

}

#endif
