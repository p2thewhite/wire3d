#pragma once
#ifndef WIREGXRESOURCES_H
#define WIREGXRESOURCES_H

#include "../../Engine/WireResourceIdentifier.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ VBufferID : public ResourceIdentifier
{
public:
	Float* ID;
	UInt VertexSize;
};

}

#endif
