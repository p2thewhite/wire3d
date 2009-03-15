//-------------------------------------------------------------------------
inline Bool Transformation::IsIdentity() const
{
	return mIsIdentity;
}
//-------------------------------------------------------------------------
inline Bool Transformation::IsRSMatrix() const
{
	return mIsRSMatrix;
}
//-------------------------------------------------------------------------
inline Bool Transformation::IsUniformScale() const
{
	return mIsRSMatrix && mIsUniformScale;
}

//-------------------------------------------------------------------------
inline Transformation::operator Matrix34f ()
{
	Matrix34f transformation;
	GetTransformation(transformation);
	return transformation;
}

//-------------------------------------------------------------------------
inline Transformation::operator const Matrix34f () const
{
	Matrix34f transformation;
	GetTransformation(transformation);
	return transformation;
}

//-------------------------------------------------------------------------
inline const Matrix34f& Transformation::GetMatrix() const
{
	return mMatrix;
}

//-------------------------------------------------------------------------
inline Vector3f Transformation::GetTranslate() const
{
	return mMatrix.GetColumn(3);
}

//-------------------------------------------------------------------------
inline const Vector3f& Transformation::GetScale() const
{
//	WIRE_ASSERT(mIsRSMatrix);
	return mScale;
}

//-------------------------------------------------------------------------
inline Float Transformation::GetUniformScale() const
{
//	WIRE_ASSERT(mIsRSMatrix && mIsUniformScale);
	return mScale.X();
}
