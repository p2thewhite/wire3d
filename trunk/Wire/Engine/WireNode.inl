// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt Node::GetQuantity() const
{
	return mChildren.GetQuantity();
}

//----------------------------------------------------------------------------
inline UInt Node::GetEffectQuantity() const
{
	return mEffects.GetQuantity();
}

//----------------------------------------------------------------------------
inline Effect* Node::GetEffect(UInt i) const
{
	WIRE_ASSERT(i < mEffects.GetQuantity());
	return mEffects[i];
}

//----------------------------------------------------------------------------
inline void Node::DetachEffect(Effect* pEffect)
{
	mEffects.Remove(pEffect);
}

//----------------------------------------------------------------------------
inline void Node::DetachAllEffects()
{
	mEffects.RemoveAll();
}
