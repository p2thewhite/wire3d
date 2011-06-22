// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESCENEOBJECT_H
#define WIRESCENEOBJECT_H

#include "WireController.h"
#include "WireObject.h"
#include "WireTArray.h"

namespace Wire
{

class SceneObject : public Object
{
	WIRE_DECLARE_RTTI;

public:
	virtual ~SceneObject();

	inline UInt GetControllerQuantity() const;
 	inline Controller* GetController(UInt i) const;
 	void AttachController(Controller* pController);
 	void DetachController(Controller* pController);
 	void DetachAllControllers();
 	Bool UpdateControllers(Double appTime);

protected:
	SceneObject();

private:
	TArray<ControllerPtr> mControllers;
};

typedef Pointer<SceneObject> SceneObjectPtr;
#include "WireSceneObject.inl"

}

#endif
