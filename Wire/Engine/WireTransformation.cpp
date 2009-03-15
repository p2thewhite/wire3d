#include "WireTransformation.h"

using namespace Wire;

//-------------------------------------------------------------------------
Transformation::Transformation()
//	TODO: mScale(Vector3f::ONE)
	:
	mScale(Vector3f(1.0f, 1.0f, 1.0f)),
	mIsIdentity(true),
	mIsRSMatrix(true),
	mIsUniformScale(true)
{
	mMatrix.MakeIdentity();
	mScale = Vector3f(1.0f, 1.0f, 1.0f);  // see TODO
}

//-------------------------------------------------------------------------
Transformation::~Transformation()
{

}

//-------------------------------------------------------------------------
void Transformation::SetMatrix(const Matrix34f& rMatrix)
{
	mMatrix = rMatrix;
	mIsIdentity = false;
	mIsRSMatrix = true;	
}

//-------------------------------------------------------------------------
void Transformation::SetTranslate(const Vector3f& rTranslate)
{
	mIsIdentity = false;
	mMatrix.SetColumn(3, rTranslate);
}

//-------------------------------------------------------------------------
void Transformation::SetScale(const Vector3f& rScale)
{
// 	WIRE_ASSERT(mIsRSMatrix && rScale.X() != 0.0f && rScale.Y() != 0.0f
// 		&& rScale.Z() != 0.0f);

	mScale = rScale;
	mIsIdentity = false;
	mIsUniformScale = false;
}

//-------------------------------------------------------------------------
void Transformation::SetUniformScale(Float scale)
{
//	WIRE_ASSERT(mIsRSMatrix && (scale != 0.0f));

	mScale = Vector3f(scale, scale, scale);
	mIsIdentity = false;
	mIsUniformScale = true;
}

//-------------------------------------------------------------------------
void Transformation::GetTransformation(Matrix34f& rMatrix) const
{
	rMatrix = mMatrix;
}
