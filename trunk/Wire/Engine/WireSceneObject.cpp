// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireSceneObject.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, SceneObject, Object);

//----------------------------------------------------------------------------
SceneObject::SceneObject()
{
}

//----------------------------------------------------------------------------
SceneObject::~SceneObject()
{
	DetachAllControllers();
}

//----------------------------------------------------------------------------
void SceneObject::AttachController(Controller* pController)
{
	// controller must exist
	if (!pController)
	{
		WIRE_ASSERT(pController);
		return;
	}

	// check if controller is already in the list
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		if (pController == mControllers[i])
		{
			// controller already exists, nothing to do
			return;
		}
	}

	// Bind the controller to the object.
	pController->SetSceneObject(this);

	// Controller not in current list, add it.
	mControllers.Append(pController);
}

//----------------------------------------------------------------------------
void SceneObject::DetachController(Controller* pController)
{
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		if (pController == mControllers[i])
		{
			// Unbind the controller from the object.
			pController->SetSceneObject(NULL);

			// Remove the controller from the list.
			mControllers.RemoveAt(i);

			return;
		}
	}
}

//----------------------------------------------------------------------------
void SceneObject::DetachAllControllers()
{
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		// Unbind the controller from the object.
		Controller* pController = mControllers[i];
		pController->SetSceneObject(NULL);
	}

	mControllers.RemoveAll();
}

//----------------------------------------------------------------------------
Bool SceneObject::UpdateControllers(Double appTime)
{
	Bool wasUpdated = false;
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		Controller* pController = mControllers[i];
		if (pController->Update(appTime))
		{
			wasUpdated = true;
		}
	}

	return wasUpdated;
}

//----------------------------------------------------------------------------
Bool SceneObject::OnGetVisibleUpdateControllers(const Camera* pCamera)
{
	Bool wasUpdated = false;
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		Controller* pController = mControllers[i];
		if (pController->OnGetVisibleUpdate(pCamera))
		{
			wasUpdated = true;
		}
	}

	return wasUpdated;
}

//----------------------------------------------------------------------------
Controller* SceneObject::GetController(const Rtti& rType, Bool findDerivedTypes)
{
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		if (findDerivedTypes)
		{
			if (mControllers[i]->IsDerived(rType))
			{
				return mControllers[i];
			}
		}
		else if (mControllers[i]->IsExactly(rType))
		{
			return mControllers[i];
		}
	}

	return NULL;
}
