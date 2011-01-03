#pragma once
#ifndef WIRECONTROLLER_H
#define WIRECONTROLLER_H

#include "WireObject.h"

namespace Wire
{

class SceneObject;

class Controller : public Object
{
	WIRE_DECLARE_RTTI;

public:
	// destruction (abstract base class)
	virtual ~Controller();

	// the controlled object
	inline SceneObject* GetObject() const;

	// geometric update
	virtual Bool Update(Double appTime);

protected:
	// construction (abstract base class)
	Controller();

	// the controlled scene object
	friend class SceneObject;
	inline void SetObject(SceneObject* pObject);

	// Regular pointer used for controlled object to avoid circular smart
	// pointers between controller and object.
	SceneObject* mpObject;

	Double mLastAppTime;
};

typedef Pointer<Controller> ControllerPtr;
#include "WireController.inl"

}

#endif
