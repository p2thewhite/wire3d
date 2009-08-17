#include "WireTransformation.h"

using namespace Wire;

//----------------------------------------------------------------------------
Transformation::Transformation()
	:
	mScale(Vector3F::ONE),
	mIsIdentity(true),
	mIsRSMatrix(true),
	mIsUniformScale(true)
{
	mMatrix.MakeIdentity();
}

//----------------------------------------------------------------------------
Transformation::~Transformation()
{
}

//----------------------------------------------------------------------------
void Transformation::SetRotate(const Matrix34F& rMatrix)
{
	Vector3F translate = GetTranslate();
	mMatrix = rMatrix;
	SetTranslate(translate);
	mIsIdentity = false;
	mIsRSMatrix = true;
}

//----------------------------------------------------------------------------
void Transformation::SetMatrix(const Matrix34F& rMatrix)
{
	mMatrix = rMatrix;
	mIsIdentity = false;
	mIsRSMatrix = false;
	mIsUniformScale = false;
}

//----------------------------------------------------------------------------
void Transformation::SetTranslate(const Vector3F& rTranslate)
{
	mIsIdentity = false;
	mMatrix.SetColumn(3, rTranslate);
}

//----------------------------------------------------------------------------
void Transformation::SetScale(const Vector3F& rScale)
{
 	WIRE_ASSERT(mIsRSMatrix && rScale.X() != 0.0F && rScale.Y() != 0.0F
 		&& rScale.Z() != 0.0F);

	mScale = rScale;
	mIsIdentity = false;
	mIsUniformScale = false;
}

//----------------------------------------------------------------------------
void Transformation::SetUniformScale(Float scale)
{
	WIRE_ASSERT(mIsRSMatrix && (scale != 0.0F));

	mScale = Vector3F(scale, scale, scale);
	mIsIdentity = false;
	mIsUniformScale = true;
}

//----------------------------------------------------------------------------
void Transformation::GetTransformation(Matrix34F& rMatrix) /*const*/
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

//----------------------------------------------------------------------------
void Transformation::Product (const Transformation& rA,
	const Transformation& rB)
{
	if (rA.IsIdentity())
	{
		*this = rB;
		return;
	}

	if (rB.IsIdentity())
	{
		*this = rA;
		return;
	}

	if (rA.IsRSMatrix() && rB.IsRSMatrix())
	{
		if (rA.IsUniformScale())
		{
			SetRotate(rA.mMatrix * rB.mMatrix);

			SetTranslate(rA.GetUniformScale() * (
				rA.mMatrix * rB.GetTranslate()) + rA.GetTranslate());

			if (rB.IsUniformScale())
			{
				SetUniformScale(rA.GetUniformScale() * rB.GetUniformScale());
			}
			else
			{
				SetScale(rA.GetUniformScale() * rB.GetScale());
			}

			return;
		}
	}

	// In all remaining cases, the matrix cannot be written as R*S*X+T.
	Matrix34F A = (rA.IsRSMatrix() ?
		rA.GetMatrix().TimesDiagonal(rA.GetScale()) : rA.GetMatrix());

	Matrix34F B = (rB.IsRSMatrix() ?
		rB.GetMatrix().TimesDiagonal(rB.GetScale()) : rB.GetMatrix());

	SetMatrix(A * B);
	SetTranslate(A * rB.GetTranslate() + rA.GetTranslate());
}
