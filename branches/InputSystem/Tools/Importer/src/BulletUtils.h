#pragma once
#ifndef BULLETUTILS_H__
#define BULLETUTILS_H__

#include "WireVector3.h"
#include "WireMatrix3.h"
#include "btBulletDynamicsCommon.h"

using namespace Wire;

class BulletUtils
{
public:
	static inline btVector3 Convert(Vector3F vector)
	{
		return btVector3(vector.X(), vector.Y(), vector.Z());
	}

	static inline Vector3F Convert(btVector3& vector)
	{
		return Vector3F(vector.getX(), vector.getY(), vector.getZ());
	}

	static inline btMatrix3x3 Convert(Matrix3F matrix)
	{
		return btMatrix3x3(matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2]);
	}

private:
	BulletUtils();
	~BulletUtils();

};

#endif