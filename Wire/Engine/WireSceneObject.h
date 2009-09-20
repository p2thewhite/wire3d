#pragma once
#ifndef WIRESCENEOBJECT_H
#define WIRESCENEOBJECT_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ SceneObject : public Object
{

};

typedef Pointer<SceneObject> SceneObjectPtr;
#include "WireSceneObject.inl"

}

#endif
