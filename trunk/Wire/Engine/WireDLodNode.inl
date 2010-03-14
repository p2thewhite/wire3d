//----------------------------------------------------------------------------
inline Vector3F& DLodNode::ModelCenter()
{
    return mModelLodCenter;
}

//----------------------------------------------------------------------------
inline const Vector3F& DLodNode::GetModelCenter() const
{
    return mModelLodCenter;
}

//----------------------------------------------------------------------------
inline const Vector3F& DLodNode::GetWorldCenter() const
{
    return mWorldLodCenter;
}

//----------------------------------------------------------------------------
inline Float DLodNode::GetModelMinDistance(UInt i) const
{
    WIRE_ASSERT(i < mModelMinDist.GetQuantity());
    return mModelMinDist[i];
}

//----------------------------------------------------------------------------
inline Float DLodNode::GetModelMaxDistance(UInt i) const
{
    WIRE_ASSERT(i < mModelMaxDist.GetQuantity());
    return mModelMaxDist[i];
}

//----------------------------------------------------------------------------
inline Float DLodNode::GetWorldMinDistance(UInt i) const
{
    WIRE_ASSERT(i < mWorldMinDist.GetQuantity());
    return mWorldMinDist[i];
}

//----------------------------------------------------------------------------
inline Float DLodNode::GetWorldMaxDistance(UInt i) const
{
    WIRE_ASSERT(i < mWorldMaxDist.GetQuantity());
    return mWorldMaxDist[i];
}
