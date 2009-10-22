#pragma once
#ifndef WIREDX9RESOURCES_H
#define WIREDX9RESOURCES_H

#include "../../Engine/WireResourceIdentifier.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ IBufferID : public ResourceIdentifier
{
public:
	LPDIRECT3DINDEXBUFFER9 ID;
};


}

#endif
