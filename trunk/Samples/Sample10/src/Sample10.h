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
	Node* CreateObjectA();
	Node* CreateObjectB();

	NodePtr mspRoot;
	LightPtr mspLight;

	// object A's shared resources
	MeshPtr mspMeshA;
	MaterialPtr mspMaterialA;
	StateAlphaPtr mspAlphaA;
	StateCullPtr mspCullA;
	StateMaterialPtr mspStateMaterialA;
	StateZBufferPtr mspZBufferA;
	VertexBufferPtr mspVertexBufferA;

	// object B's shared resources
	MeshPtr mspMeshB;
	MaterialPtr mspMaterialB;
	StateMaterialPtr mspStateMaterialB;

	CameraPtr mspCamera;
	Culler mCuller;
	CullerSorting mCullerSorting;

	// frames per second text
	void DrawFPS(Double elapsed, Bool usesSorting);
	CameraPtr mspTextCamera;
	TextPtr mspText;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample10);

#endif
