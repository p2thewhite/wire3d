#include "Sample2.h"

using namespace Wire;

WIRE_APPLICATION(Sample2);

//----------------------------------------------------------------------------
Sample2::Sample2()
	:
	WIREAPPLICATION(
		ColorRGBA(0.0F, 0.0F, 0.2F, 1.0F),	// Background color.
		"Sample2 - Creating a Scenegraph",	// Name of the application.
		0, 0,								// Window position and
		640, 480)							// size (both ignored on the Wii).
{
}

//----------------------------------------------------------------------------
Bool Sample2::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspRoot = WIRE_NEW Node;
	mspRoot->AttachChild(CreateHelicopter());

	// Create a fog render state and attach it to the root node, i.e.
	// apply fog to all objects that are attached to this node.
	FogState* pFogState = WIRE_NEW FogState;
	pFogState->Enabled = true;
	pFogState->Color = ColorRGB(0, 0, 0.2F);
	pFogState->Start = 20.0F;
	pFogState->End = 40.0F;
	mspRoot->AttachGlobalState(pFogState);

	// Once we finished creating the scene graph, we update the graph's
	// render states. This propagates the fog state to all child nodes.
	// Whenever you change the graph's render state (using Attach-/
	// DetachGlobalState() on any of its children), you must call
	// UpdateRS().
	mspRoot->UpdateRS();

	// setup our camera at the origin
	// looking down the -z axis with y up
	Vector3F cameraLocation(0.0F, 1.0F, 25.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// specify FOV, aspect ratio, near and far plane of the frustum
	Float width = static_cast<Float>(mpRenderer->GetWidth());
	Float height = static_cast<Float>(mpRenderer->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);

	// the culler needs to know which camera to use when performing its task
	mCuller.SetCamera(mspCamera);

	mAngle = 0.0F;
	mLastTime = System::GetTime();
	return true;
}

//----------------------------------------------------------------------------
void Sample2::OnIdle()
{
	// The render loop.

	// measure how much time passed since the last call
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	
	System::Print("%f, %f\n", static_cast<Float>(time),
 		static_cast<Float>(elapsedTime));

	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	// position and rotate the helicopter
	Matrix34F rot(Vector3F::UNIT_Y, mAngle);
	mspRoot->Local.SetRotate(rot);
	Vector3F pos = Vector3F(-10, 0 ,0) * Matrix34F(Vector3F::UNIT_Y, -mAngle);
	mspRoot->Local.SetTranslate(pos);

	// rotate the rear rotor
	Matrix34F rearRot(Vector3F::UNIT_Z, MathF::FMod(mAngle*16, MathF::TWO_PI));
	mspRearRotor->Local.SetRotate(rearRot);

	// rotate the top rotor
	Matrix34F topRot(Vector3F::UNIT_Y, MathF::FMod(mAngle*16, MathF::TWO_PI));
	mspTopRotor->Local.SetRotate(topRot);

	// Since the local transformations of the helicopter and its rotors
	// changed, we need to call UpdateGS() (update geometric state) to 
	// calculate the world transformation and update the world bounding
	// volumes accordingly.
	// See WireSpatial.h for details.
 	mspRoot->UpdateGS(time);

	// The culler traverses the scene graph top-down and left to right, 
	// collecting all visible objects in a set. This is also the order in
	// which these visible objects are being drawn by the Renderer.
	mCuller.ComputeVisibleSet(mspRoot);

	mpRenderer->ClearBuffers();
	mpRenderer->PreDraw(mspCamera);
	mpRenderer->DrawScene(mCuller.GetVisibleSet());
	mpRenderer->PostDraw();
	mpRenderer->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample2::CreateCube(ColorRGBA top, ColorRGBA bottom)
{
	// Creation of Wire::Geometry objects is explained in detail in Sample1

	Float extent = 1.0F;
	Vector3F vertices[] = {
		Vector3F(-extent, -extent, -extent),
		Vector3F(+extent, -extent, -extent),
		Vector3F(+extent, +extent, -extent),
		Vector3F(-extent, +extent, -extent),
		Vector3F(-extent, -extent, +extent),
		Vector3F(+extent, -extent, +extent),
		Vector3F(+extent, +extent, +extent),
		Vector3F(-extent, +extent, +extent)
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);	// XYZ
	attributes.SetColorChannels(4);		// RGBA
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		if (vertices[i].Y() > 0)
		{
			pCubeVerts->Color4(i) = top;
		}
		else
		{
			pCubeVerts->Color4(i) = bottom;
		}
	}

	UInt indices[] = {
		0, 2, 1,
		0, 3, 2,
		0, 1, 5,
		0, 5, 4,
		0, 4, 7,
		0, 7, 3,
		6, 4, 5,
		6, 7, 4,
		6, 5, 1,
		6, 1, 2,
		6, 2, 3,
		6, 3, 7,
	};

	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for (UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);
	return pCube;
}

//----------------------------------------------------------------------------
Node* Sample2::CreateHelicopter()
{
	// Create the helicopter's root node.
	Node* pNode = WIRE_NEW Node;

	// All other nodes and geometric objects are attached to it.
	// Together they form a scene graph, which represents a hierarchy and
	// thus simplifies geometric transformations and visibility culling.

	// Create a cube using the given colors for top and bottom halves.
	Geometry* pBody1 = CreateCube(ColorRGBA::RED, ColorRGBA::RED*0.3F);

	// Scale the cube by a factor of 2 in x direction.
	pBody1->Local.SetScale(Vector3F(2, 1, 1));
	
	// Attach the cube to the root node
	pNode->AttachChild(pBody1);

	// Create all the other parts of the helicopter by generating cubes,
	// which are then scaled and translated with some modified vertices to
	// create the desired shapes.
	Geometry* pBody2 = CreateCube(ColorRGBA::RED, ColorRGBA::RED*0.3F);
	pBody2->Local.SetScale(Vector3F(1, 2, 1));
	pBody2->Local.SetTranslate(Vector3F(3, 1, 0));
	pBody2->VBuffer->Position3(1).Y() = -0.5F;
	pBody2->VBuffer->Position3(5).Y() = -0.5F;
	pNode->AttachChild(pBody2);

	Geometry* pTail = CreateCube(ColorRGBA::RED, ColorRGBA::RED*0.3F);
	pTail->Local.SetTranslate(Vector3F(5, 2, 0));
	pTail->VBuffer->Position3(2) = Vector3F(5, 0.6F, -0.1F);
	pTail->VBuffer->Position3(6) = Vector3F(5, 0.6F, 0.1F);
	pTail->VBuffer->Position3(1) = Vector3F(5, 0.1F, -0.1F);
	pTail->VBuffer->Position3(5) = Vector3F(5, 0.1F, 0.1F);
	pNode->AttachChild(pTail);

	Geometry* pNose = CreateCube(ColorRGBA::RED, ColorRGBA::RED*0.3F);
	pNose->Local.SetTranslate(Vector3F(-3, 0, 0));
	pNose->VBuffer->Position3(0) = Vector3F(-1, -0.75F, -0.35F);
	pNose->VBuffer->Position3(4) = Vector3F(-1, -0.75F, 0.35F);
	pNose->VBuffer->Position3(3) = Vector3F(-1, -0.25F, -0.35F);
	pNose->VBuffer->Position3(7) = Vector3F(-1, -0.25F, 0.35F);
	pNode->AttachChild(pNose);

	// We save a reference to the rotors, so we can easily access them later.
	mspRearRotor = CreateCube(ColorRGBA::WHITE*0.7F, ColorRGBA::WHITE*0.4F);
	mspRearRotor->Local.SetScale(Vector3F(1, 0.2F, 0.1F));
	mspRearRotor->Local.SetTranslate(Vector3F(9.75F, 2.5F, 0.55F));
	pNode->AttachChild(mspRearRotor);

	mspTopRotor = CreateCube(ColorRGBA::WHITE*0.8F, ColorRGBA::WHITE*0.3F);
	mspTopRotor->Local.SetScale(Vector3F(6, 0.1F, 0.3F));
	mspTopRotor->Local.SetTranslate(Vector3F(2, 3.2F, 0));
	pNode->AttachChild(mspTopRotor);

	Geometry* pCockpit = CreateCube(ColorRGBA(1, 1, 1, 0.3F),
		ColorRGBA(1, 1, 1, 0.5F));
	pCockpit->Local.SetScale(Vector3F(2, 1, 1));
	pCockpit->Local.SetTranslate(Vector3F(0, 2, 0));
	pCockpit->VBuffer->Position3(3) = Vector3F(-0.5F, 0.2F, -0.35F);
	pCockpit->VBuffer->Position3(7) = Vector3F(-0.5F, 0.2F, 0.35F);
	pNode->AttachChild(pCockpit);

	// The cockpit is supposed to be transparent, so we create an AlphaState 
	// and attach it to the cockpit object.
	AlphaState* pAlphaState = WIRE_NEW AlphaState;
	pAlphaState->BlendEnabled = true;
	pCockpit->AttachGlobalState(pAlphaState);

	// Now we tilt and orient the helicopter for later use.
	Matrix34F tilt(Vector3F(0, 0, 1), 0.2F);
	Matrix34F rotate(Vector3F(0, 1, 0), MathF::HALF_PI);
	tilt = rotate * tilt;
	pNode->Local.SetRotate(tilt);

	return pNode;
}
