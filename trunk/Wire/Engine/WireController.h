#pragma once
#ifndef WIRECONTROLLER_H
#define WIRECONTROLLER_H

#include "WireObject.h"

namespace Wire
{

class SceneObject;

class /*WIRE_ENGINE_ITEM*/ Controller : public Object
{

public:
	// destruction (abstract base class)
	virtual ~Controller();

	// the controlled object
	SceneObject* GetObject() const;

	// geometric update
	virtual Bool Update(Double appTime);

protected:
	// construction (abstract base class)
	Controller();

	// the controlled scene object
	friend class SceneObject;
	virtual void SetObject(SceneObject* pObject);

	// Regular pointer used for controlled object to avoid circular smart
	// pointers between controller and object.
	SceneObject* mpObject;

	Double mLastAppTime;
};

typedef Pointer<Controller> ControllerPtr;
#include "WireController.inl"

}

#endif
