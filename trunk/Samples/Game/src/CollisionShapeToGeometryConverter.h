#pragma once
#ifndef COLLISIONSHAPETOGEOMETRYCONVERTER_H
#define COLLISIONSHAPETOGEOMETRYCONVERTER_H

#include "WireNode.h"

#include "btBulletDynamicsCommon.h"

using namespace Wire;

class CollisionShapeToGeometryConverter
{
public:
	static Node* Convert(btCollisionShape* pShape, const Color32& rColor);
	static Node* CreateWireframeBox(btBoxShape* pBoxShape, const Color32& rColor);
	static Node* CreateWireframeSphere(btSphereShape* pSphereShape, const Color32& rColor);
	static Node* CreateWireframeMesh(btBvhTriangleMeshShape* pTriangleMeshShape, const Color32& rColor);

private:
	static Float s_pCylinderBodyVerticesData[];
	static Float s_pCylinderTopCapVerticesData[];
	static Float s_pCylinderBottomCapVerticesData[];

	CollisionShapeToGeometryConverter();
	~CollisionShapeToGeometryConverter();
};

#endif