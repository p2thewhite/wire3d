#pragma once
#ifndef WIREGXRESOURCES_H
#define WIREGXRESOURCES_H

#include "../../Engine/WireResourceIdentifier.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ VBufferID : public ResourceIdentifier
{
public:
	struct VertexElement
	{
		void* Data;
		UChar Attr;
		UChar CompCnt;
		UChar CompType;
	};

	Float* ID;
	UInt VertexSize;
	TArray<VertexElement>* Elements;
};

}

#endif
