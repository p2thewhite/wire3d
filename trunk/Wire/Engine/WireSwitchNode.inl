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
