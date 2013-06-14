// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt SceneObject::GetControllerQuantity() const
{
	return mControllers.GetQuantity();
}

//----------------------------------------------------------------------------
inline Controller* SceneObject::GetController(UInt i) const
{
	WIRE_ASSERT(i < mControllers.GetQuantity());
	return mControllers[i];
}

//----------------------------------------------------------------------------
inline void SceneObject::SetName(const String& rName)
{
	mName = rName;
}

//----------------------------------------------------------------------------
inline const String& SceneObject::GetName() const
{
	return mName;
}

//----------------------------------------------------------------------------
template <class ControllerType>
ControllerType* SceneObject::FindController(Bool findDerivedTypes)
{
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		if (findDerivedTypes)
		{
			if (mControllers[i]->IsDerived(ControllerType::TYPE))
			{
				return StaticCast<ControllerType>(mControllers[i]);
			}
		}
		else if (mControllers[i]->IsExactly(ControllerType::TYPE))
		{
			return StaticCast<ControllerType>(mControllers[i]);
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------
template <class ControllerType>
void SceneObject::FindControllers(TArray<ControllerType*>& rControllers,
	Bool findDerivedTypes)
{
	for (UInt i = 0; i < mControllers.GetQuantity(); i++)
	{
		Bool isType = false;
		if (findDerivedTypes)
		{
			if (mControllers[i]->IsDerived(ControllerType::TYPE))
			{
				isType = true;
			}
		}
		else if (mControllers[i]->IsExactly(ControllerType::TYPE))
		{
			isType = true;
		}

		if (isType)
		{
			rControllers.Append(StaticCast<ControllerType>(mControllers[i]));
		}
	}
}
