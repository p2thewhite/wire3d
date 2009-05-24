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
void Transformation::SetRotate(const Matrix34f& rMatrix)
{
	Vector3f translate = GetTranslate();
	mMatrix = rMatrix;
	SetTranslate(translate);
	mIsIdentity = false;
	mIsRSMatrix = true;
}

//-------------------------------------------------------------------------
void Transformation::SetMatrix(const Matrix34f& rMatrix)
{
	mMatrix = rMatrix;
	mIsIdentity = false;
	mIsRSMatrix = false;
	mIsUniformScale = false;
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
 	WIRE_ASSERT(mIsRSMatrix && rScale.X() != 0.0f && rScale.Y() != 0.0f
 		&& rScale.Z() != 0.0f);

	mScale = rScale;
	mIsIdentity = false;
	mIsUniformScale = false;
}

//-------------------------------------------------------------------------
void Transformation::SetUniformScale(Float scale)
{
	WIRE_ASSERT(mIsRSMatrix && (scale != 0.0f));

	mScale = Vector3f(scale, scale, scale);
	mIsIdentity = false;
	mIsUniformScale = true;
}

//-------------------------------------------------------------------------
void Transformation::GetTransformation(Matrix34f& rMatrix) const
{
	if (mIsRSMatrix)
	{
		rMatrix[0][0] = mMatrix[0][0] * mScale.X();
		rMatrix[0][1] = mMatrix[0][1] * mScale.Y();
		rMatrix[0][2] = mMatrix[0][2] * mScale.Z();
		rMatrix[0][3] = mMatrix[0][3];

		rMatrix[1][0] = mMatrix[1][0] * mScale.X();
		rMatrix[1][1] = mMatrix[1][1] * mScale.Y();
		rMatrix[1][2] = mMatrix[1][2] * mScale.Z();
		rMatrix[1][3] = mMatrix[1][3];

		rMatrix[2][0] = mMatrix[2][0] * mScale.X();
		rMatrix[2][1] = mMatrix[2][1] * mScale.Y();
		rMatrix[2][2] = mMatrix[2][2] * mScale.Z();
		rMatrix[2][3] = mMatrix[2][3];
	}
	else
	{
		rMatrix = mMatrix;
	}
}
