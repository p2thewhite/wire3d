#pragma once
#ifndef BULLETUTILS_H
#define BULLETUTILS_H

#include "btBulletDynamicsCommon.h"
#include "WireMatrix3.h"
#include "WireMesh.h"

class BulletUtils
{
public:
	static inline btVector3 Convert(const Wire::Vector3F& vector)
	{
		return btVector3(vector.X(), vector.Y(), vector.Z());
	}

	static inline Wire::Vector3F Convert(const btVector3& vector)
	{
		return Wire::Vector3F(vector.getX(), vector.getY(), vector.getZ());
	}

	static inline btMatrix3x3 Convert(const Wire::Matrix3F& matrix)
	{
		return btMatrix3x3(
			matrix[0][0], matrix[0][1], matrix[0][2],
			matrix[1][0], matrix[1][1], matrix[1][2],
			matrix[2][0], matrix[2][1], matrix[2][2]);
	}

	static btTriangleIndexVertexArray* Convert(Wire::Mesh* pMesh);
	static Wire::Mesh* Convert(btTriangleMeshShape* pTriangleMeshShape);

private:
	BulletUtils();
	~BulletUtils();
};

#endif
