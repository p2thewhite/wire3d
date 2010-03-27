#pragma once
#ifndef WIREGXRESOURCES_H
#define WIREGXRESOURCES_H

#include "WireResourceIdentifier.h"
#include "WireTArray.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ VBufferID : public ResourceIdentifier
{
public:
	struct VertexElement
	{
		void* Data;
		UInt Size;
		UChar Stride;
		UChar Attr;
		UChar CompCnt;
		UChar CompType;
	};

	struct DisplayList
	{
		void* DL;
		UInt DLSize;
		IBufferID* RegisteredIBuffer;
	};

	TArray<VertexElement> Elements;
	TArray<DisplayList> DisplayLists;
};

class /*WIRE_RENDERER_ITEM*/ IBufferID : public ResourceIdentifier
{
public:
	TArray<VBufferID*> RegisteredVBuffers;

};

class /*WIRE_RENDERER_ITEM*/ TextureID : public ResourceIdentifier
{
public:
	Texture* TextureObject;
};

}

#endif
