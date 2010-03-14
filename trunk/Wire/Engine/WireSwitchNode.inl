//----------------------------------------------------------------------------
inline void SwitchNode::SetActiveChild(Int activeChild)
{
    WIRE_ASSERT(activeChild == SN_INVALID_CHILD || 
		static_cast<UInt>(activeChild) < GetQuantity());
    mActiveChild = activeChild;
}

//----------------------------------------------------------------------------
inline Int SwitchNode::GetActiveChild() const
{
    return mActiveChild;
}

//----------------------------------------------------------------------------
inline void SwitchNode::DisableAllChildren()
{
    mActiveChild = SN_INVALID_CHILD;
}
