// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2014. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void Controller::SetSceneObject(SceneObject* pObject)
{
	mpSceneObject = pObject;
}

//----------------------------------------------------------------------------
inline SceneObject* Controller::GetSceneObject() const
{
	return mpSceneObject;
}

//----------------------------------------------------------------------------
inline void Controller::OnAttach()
{
}

//----------------------------------------------------------------------------
inline void Controller::OnDetach()
{
}
