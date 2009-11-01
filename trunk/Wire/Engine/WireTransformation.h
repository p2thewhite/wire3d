#pragma once
#ifndef WIRETRANSFORMATION_H
#define WIRETRANSFORMATION_H

#include "../Foundation/WireTypes.h"
#include "../Foundation/WireMatrix34.h"
#include "../Foundation/WireMatrix4.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Transformation
{
public:
	Transformation();
 	~Transformation();

	// Hints about the structure of the transformation.  In the common case
	// of M = R*S, IsRSMatrix() returns true.
	Bool IsIdentity() const;
	Bool IsRSMatrix() const;
	Bool IsUniformScale() const;

	inline operator Matrix34F ();
 	inline operator const Matrix34F () const;

	void SetRotate(const Matrix34F& rMatrix);
	void SetMatrix(const Matrix34F& rMatrix);
	const Matrix34F& GetMatrix() const;
	void SetTranslate(const Vector3F& rTranslate);
	Vector3F GetTranslate() const;
	void SetScale(const Vector3F& rScale);
	const Vector3F& GetScale() const;
	void SetUniformScale(Float scale);
	Float GetUniformScale() const;

	// Pack the transformation into a 3-by-4 matrix with implicit 4th row.
	void GetTransformation(Matrix34F& rMatrix) const;

	// Pack the transformation into a 4-by-4 matrix, stored so that it may be
	// applied to 1-by-4 vectors on the left.
	void GetHomogeneous(Matrix4F& rHMatrix) const;

	// For M = R*S, the largest value of S in absolute value is returned.
	// For general M, the max-row-sum norm is returned (and is guaranteed to
	// be larger or equal to the largest eigenvalue of S in absolute value).
	Float GetNorm() const;

	// Compute Y = M*X+T where X is the input point and Y is the output point.
	Vector3F ApplyForward(const Vector3F& rInput) const;

	// Compute C = A*B.
	void Product(const Transformation& rA, const Transformation& kB);

private:
	mutable Matrix34F mMatrix;	// TODO: fix for VC
	Vector3F mScale;

	Bool mIsIdentity;
	Bool mIsRSMatrix;
	Bool mIsUniformScale;
};

#include "WireTransformation.inl"

}

#endif
