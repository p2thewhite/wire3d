// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESCENEOBJECT_H
#define WIRESCENEOBJECT_H

#include "WireController.h"
#include "WireString.h"
#include "WireTArray.h"

namespace Wire
{

class Camera;
class Controller;

class SceneObject : public Object
{
	WIRE_DECLARE_RTTI;

public:
	virtual ~SceneObject();

	void SetName(const String& rName);
	const String& GetName() const;

	inline UInt GetControllerQuantity() const;
 	inline Controller* GetController(UInt i) const;
 	void AttachController(Controller* pController);
 	void DetachController(Controller* pController);
 	void DetachAllControllers();

	Bool UpdateControllers(Double appTime);
	Bool OnGetVisibleUpdateControllers(const Camera* pCamera);

	template <class T> T* GetController(Bool findDerivedTypes = true);

	template <class T> void GetControllers(TArray<T*>& rControllers,
		Bool findDerivedTypes = true);

protected:
	SceneObject();

private:
	TArray<Pointer<Controller> > mControllers;
	String mName;
};

typedef Pointer<SceneObject> SceneObjectPtr;
#include "WireSceneObject.inl"

}

#endif
