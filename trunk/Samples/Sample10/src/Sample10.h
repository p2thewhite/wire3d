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
	Sample10();
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

	// frames per second text
	void DrawFPS(Double elapsed);
	CameraPtr mspTextCamera;
	StateAlphaPtr mspTextAlpha;
	Double mLastTime;

	static const UInt TextArraySize = 1000;
	Char text[TextArraySize];
};

WIRE_REGISTER_INITIALIZE(Sample10);

#endif
