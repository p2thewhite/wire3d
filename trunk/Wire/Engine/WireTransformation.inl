//----------------------------------------------------------------------------
inline Bool Transformation::IsIdentity() const
{
	return mIsIdentity;
}
//----------------------------------------------------------------------------
inline Bool Transformation::IsRSMatrix() const
{
	return mIsRSMatrix;
}
//----------------------------------------------------------------------------
inline Bool Transformation::IsUniformScale() const
{
	return mIsRSMatrix && mIsUniformScale;
}

//----------------------------------------------------------------------------
inline Transformation::operator Matrix34F ()
{
	Matrix34F transformation;
	GetTransformation(transformation);
	return transformation;
}

//----------------------------------------------------------------------------
inline Transformation::operator const Matrix34F () const
{
	Matrix34F transformation;
	GetTransformation(transformation);
	return transformation;
}

//----------------------------------------------------------------------------
inline const Matrix34F& Transformation::GetMatrix() const
{
	return mMatrix;
}

//----------------------------------------------------------------------------
inline Vector3F Transformation::GetTranslate() const
{
	return mMatrix.GetColumn(3);
}

//----------------------------------------------------------------------------
inline const Vector3F& Transformation::GetScale() const
{
	WIRE_ASSERT(mIsRSMatrix);
	return mScale;
}

//----------------------------------------------------------------------------
inline Float Transformation::GetUniformScale() const
{
	WIRE_ASSERT(mIsRSMatrix && mIsUniformScale);
	return mScale.X();
}
