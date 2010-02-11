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

	TArray<VertexElement>* Elements;
};

class /*WIRE_RENDERER_ITEM*/ TextureID : public ResourceIdentifier
{
public:
	Texture* TextureObject;
};

}

#endif
