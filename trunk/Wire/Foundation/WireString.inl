//----------------------------------------------------------------------------
inline UInt String::GetLength() const
{
    return mLength;
}

//----------------------------------------------------------------------------
inline String::operator const Char* () const
{
    return mpText;
}
