#pragma once
#ifndef WIRETRANSFORMATION_H
#define WIRETRANSFORMATION_H

#include "../Foundation/WireTypes.h"
#include "../Foundation/WireMatrix34.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ Transformation
{
public:
	Transformation();
 	~Transformation();

	// Hints about the structure of the transformation.  In the common case
	// of M = R*S, IsRSMatrix() returns true.
	Bool IsIdentity () const;
	Bool IsRSMatrix () const;
	Bool IsUniformScale () const;

	inline operator Matrix34f ();
 	inline operator const Matrix34f () const;

	void SetMatrix(const Matrix34f& rMatrix);
	const Matrix34f& GetMatrix() const;
	void SetTranslate(const Vector3f& rTranslate);
	Vector3f GetTranslate() const;
	void SetScale(const Vector3f& rScale);
	const Vector3f& GetScale() const;
	void SetUniformScale (Float scale);
	Float GetUniformScale() const;

	// Pack the transformation into a 3-by-4 matrix with implicit 4th row.
	void GetTransformation(Matrix34f& rMatrix) const;

private:
	Matrix34f mMatrix;
	Vector3f mScale;

	Bool mIsIdentity;
	Bool mIsRSMatrix;
	Bool mIsUniformScale;
};

#include "WireTransformation.inl"

}

#endif /* WIRETRANSFORMATION_H */
