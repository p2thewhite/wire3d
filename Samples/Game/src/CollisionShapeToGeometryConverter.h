#pragma once
#ifndef COLLISIONSHAPETOGEOMETRYCONVERTER_H
#define COLLISIONSHAPETOGEOMETRYCONVERTER_H

#include "WireGeometry.h"

#include "btBulletDynamicsCommon.h"

using namespace Wire;

class CollisionShapeToGeometryConverter
{
public:
	static Geometry* Convert(btCollisionShape* pShape, const Color32& rColor);
	static Geometry* CreateWireframeBox(btBoxShape* pBoxShape, const Color32& rColor);
	static Geometry* CreateWireframeMesh(btBvhTriangleMeshShape* pTriangleMeshShape, const Color32& rColor);

private:
	CollisionShapeToGeometryConverter();
	~CollisionShapeToGeometryConverter();

};

#endif