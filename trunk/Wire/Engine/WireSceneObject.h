#pragma once
#ifndef WIRESCENEOBJECT_H
#define WIRESCENEOBJECT_H

#include "WireController.h"
#include "WireObject.h"
#include "WireTArray.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ SceneObject : public Object
{
	WIRE_DECLARE_RTTI;

public:
	virtual ~SceneObject();

 	UInt GetControllerQuantity() const;
 	Controller* GetController(UInt i) const;
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
