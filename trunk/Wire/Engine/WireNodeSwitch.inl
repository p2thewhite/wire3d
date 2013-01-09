// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void NodeSwitch::SetActiveChild(Int activeChild)
{
    WIRE_ASSERT(activeChild == SN_INVALID_CHILD || 
		static_cast<UInt>(activeChild) < GetQuantity());
    mActiveChild = activeChild;
}

//----------------------------------------------------------------------------
inline Int NodeSwitch::GetActiveChild() const
{
    return mActiveChild;
}

//----------------------------------------------------------------------------
inline void NodeSwitch::DisableAllChildren()
{
    mActiveChild = SN_INVALID_CHILD;
}
