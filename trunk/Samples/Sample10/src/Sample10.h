#pragma once
#ifndef SAMPLE10_H
#define SAMPLE10_H

#include "WireApplication.h"

using namespace Wire;

class Sample10 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	Node* CreateScene();
	Geometry* CreateGeometryA();
	Geometry* CreateGeometryB();

	NodePtr mspRoot;

	MeshPtr mspMeshA;
	MaterialPtr mspMaterialA;
	StateMaterialPtr mspStateMaterialA;

	MeshPtr mspMeshB;
	MaterialPtr mspMaterialB;
	StateMaterialPtr mspStateMaterialB;

	CameraPtr mspCamera;
	Culler mCuller;
};

WIRE_REGISTER_INITIALIZE(Sample10);

#endif
