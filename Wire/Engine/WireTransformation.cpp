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
void Transformation::Product(const Transformation& rA,
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

//----------------------------------------------------------------------------
Vector3F Transformation::ApplyForward(const Vector3F& rInput) const
{
	if (mIsIdentity)
	{
		// Y = X
		return rInput;
	}

	if (mIsRSMatrix)
	{
		// Y = R*S*X + T
		Vector3F output(mScale.X() * rInput.X(), mScale.Y() * rInput.Y(),
			mScale.Z() * rInput.Z());
		output = mMatrix * output + GetTranslate();
		return output;
	}

	// Y = M*X + T
	Vector3F output = mMatrix * rInput + GetTranslate();
	return output;
}

//----------------------------------------------------------------------------
Float Transformation::GetNorm() const
{
	if (mIsRSMatrix)
	{
		Float max = MathF::FAbs(mScale.X());
		if (MathF::FAbs(mScale.Y()) > max)
		{
			max = MathF::FAbs(mScale.Y());
		}
		if (MathF::FAbs(mScale.Z()) > max)
		{
			max = MathF::FAbs(mScale.Z());
		}
		return max;
	}

//TODO implement operator[] for Matrix34

	// A general matrix.  Use the max-row-sum matrix norm.  The spectral
	// norm (the maximum absolute value of the eigenvalues) is smaller or
	// equal to this norm.  Therefore, this function returns an approximation
	// to the maximum scale.
// 	Float maxRowSum =
// 		MathF::FAbs(mMatrix[0][0]) +
// 		MathF::FAbs(mMatrix[0][1]) +
// 		MathF::FAbs(mMatrix[0][2]);
// 
// 	Float rowSum =
// 		MathF::FAbs(mMatrix[1][0]) +
// 		MathF::FAbs(mMatrix[1][1]) +
// 		MathF::FAbs(mMatrix[1][2]);
// 
// 	if (rowSum > maxRowSum)
// 	{
// 		maxRowSum = rowSum;
// 	}
// 
// 	rowSum =
// 		MathF::FAbs(mMatrix[2][0]) +
// 		MathF::FAbs(mMatrix[2][1]) +
// 		MathF::FAbs(mMatrix[2][2]);
// 
// 	if (rowSum > maxRowSum)
// 	{
// 		maxRowSum = rowSum;
// 	}
// 
// 	return maxRowSum;

	return 0.0F;
}
