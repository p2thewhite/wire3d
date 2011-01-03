//----------------------------------------------------------------------------
inline Vector3F& NodeDLod::ModelCenter()
{
    return mModelLodCenter;
}

//----------------------------------------------------------------------------
inline const Vector3F& NodeDLod::GetModelCenter() const
{
    return mModelLodCenter;
}

//----------------------------------------------------------------------------
inline const Vector3F& NodeDLod::GetWorldCenter() const
{
    return mWorldLodCenter;
}

//----------------------------------------------------------------------------
inline Float NodeDLod::GetModelMinDistance(UInt i) const
{
    WIRE_ASSERT(i < mModelMinDist.GetQuantity());
    return mModelMinDist[i];
}

//----------------------------------------------------------------------------
inline Float NodeDLod::GetModelMaxDistance(UInt i) const
{
    WIRE_ASSERT(i < mModelMaxDist.GetQuantity());
    return mModelMaxDist[i];
}

//----------------------------------------------------------------------------
inline Float NodeDLod::GetWorldMinDistance(UInt i) const
{
    WIRE_ASSERT(i < mWorldMinDist.GetQuantity());
    return mWorldMinDist[i];
}

//----------------------------------------------------------------------------
inline Float NodeDLod::GetWorldMaxDistance(UInt i) const
{
    WIRE_ASSERT(i < mWorldMaxDist.GetQuantity());
    return mWorldMaxDist[i];
}
