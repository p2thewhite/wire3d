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
	Spatial* CreateGeometryA();
	Spatial* CreateGeometryB();

	NodePtr mspRoot;

	MeshPtr mspMeshA;
	MaterialPtr mspMaterialA;
	StateAlphaPtr mspAlphaA;
	StateCullPtr mspCullA;
	StateMaterialPtr mspStateMaterialA;
	StateZBufferPtr mspZBufferA;
	VertexBufferPtr mspVertexBufferA;

	MeshPtr mspMeshB;
	MaterialPtr mspMaterialB;
	StateMaterialPtr mspStateMaterialB;

	CameraPtr mspCamera;
	Culler mCuller;
	CullerSorting mCullerSorting;

	// frames per second text
	void DrawFPS(Double elapsed, Bool usesSorting);
	CameraPtr mspTextCamera;
	StateAlphaPtr mspTextAlpha;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample10);

#endif
