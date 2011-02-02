// Sample2 - Scene Graph
// This sample demonstrates how to create and render a scene graph.
// Scene graphs serve the purpose of organizing your 3D objects (together
// with render states, lights, effects and controllers) in a hierarchy.
// The design of the classes is explained in detail in:
// http://downloads.gamedev.net/pdf/Eberly-3.pdf

#include "Sample2.h"

using namespace Wire;

WIRE_APPLICATION(Sample2);

//----------------------------------------------------------------------------
Sample2::Sample2()
	:
	WIREAPPLICATION(
		ColorRGBA(0.0F, 0.0F, 0.2F, 1.0F),	// Background color.
		"Sample2 - Creating a Scene Graph",	// title of the window,
		0, 0,								// window position
		640, 480,							// window size
		false)								// fullscreen (ignored on the Wii).
{
}

//----------------------------------------------------------------------------
Bool Sample2::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	// Create a helicopter consisting of several objects and attach it to the
	// root node of the scene graph. Whenever we move or rotate this node,
	// every object attached to it, moves/rotates along.
	mspRoot = WIRE_NEW Node;
	mspRoot->AttachChild(CreateHelicopter());

	// Create a fog render state and attach it to the root node, i.e.
	// apply fog to all objects that are attached to this node.
	StateFog* pFog = WIRE_NEW StateFog;
	pFog->Enabled = true;
	pFog->Color = ColorRGB(0, 0, 0.2F); // dark blue fog
	pFog->Start = 20.0F; // camera space z value
	pFog->End = 40.0F;
	mspRoot->AttachState(pFog);

	// Once we finished creating the scene graph, we update the graph's
	// render states. This propagates the fog state to all child nodes.
	// Whenever you change the graph's render state (using Attach-/
	// DetachState() on any of its children), you must call UpdateRS().
	mspRoot->UpdateRS();

	// setup our camera at the origin, looking down the -z axis with y up
	Vector3F cameraLocation(0.0F, 1.0F, 25.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// specify FOV, aspect ratio, near and far plane of the frustum
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
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

	// Since the local transformations of the helicopter and its child objects
	// (i.e. rotors) changed, we need to call UpdateGS() (update geometric
	// state) to calculate the world transformation and update the world
	// bounding volumes accordingly.
	// See WireSpatial.h for details.
 	mspRoot->UpdateGS(time);

	// The culler traverses the scene graph top-down and left to right, 
	// collecting all visible objects in a set. This is also the order in
	// which the visible objects are being drawn by the Renderer.
	mCuller.ComputeVisibleSet(mspRoot);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSet());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample2::CreateCube(ColorRGBA top, ColorRGBA bottom)
{
	// Creation of Wire::Geometry objects is explained in detail in Sample1.
	UInt vertexColorChannels = 4;	// RGBA
	Geometry* pCube = StandardMesh::CreateCube8(vertexColorChannels);
	VertexBuffer* const pVBuffer = pCube->GetVBuffer();

	// initialize the cube's vertex colors with our supplied values
	for (UInt i = 0; i < pVBuffer->GetVertexQuantity(); i++)
	{
		if (pVBuffer->Position3(i).Y() > 0)
		{
			pVBuffer->Color4(i) = top;
		}
		else
		{
			pVBuffer->Color4(i) = bottom;
		}
 	}

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
	pBody2->GetVBuffer()->Position3(1).Y() = -0.5F;
	pBody2->GetVBuffer()->Position3(5).Y() = -0.5F;
	// we changed the vertices, the model bound has potentially changed, too
	pBody2->ModelBound->ComputeFromData(pBody2->GetVBuffer());
	pNode->AttachChild(pBody2);

	Geometry* pTail = CreateCube(ColorRGBA::RED, ColorRGBA::RED*0.3F);
	pTail->Local.SetTranslate(Vector3F(5, 2, 0));
	pTail->GetVBuffer()->Position3(2) = Vector3F(5, 0.6F, -0.1F);
	pTail->GetVBuffer()->Position3(6) = Vector3F(5, 0.6F, 0.1F);
	pTail->GetVBuffer()->Position3(1) = Vector3F(5, 0.1F, -0.1F);
	pTail->GetVBuffer()->Position3(5) = Vector3F(5, 0.1F, 0.1F);
	pTail->ModelBound->ComputeFromData(pTail->GetVBuffer());
	pNode->AttachChild(pTail);

	Geometry* pNose = CreateCube(ColorRGBA::RED, ColorRGBA::RED*0.3F);
	pNose->Local.SetTranslate(Vector3F(-3, 0, 0));
	pNose->GetVBuffer()->Position3(0) = Vector3F(-1, -0.75F, -0.35F);
	pNose->GetVBuffer()->Position3(4) = Vector3F(-1, -0.75F, 0.35F);
	pNose->GetVBuffer()->Position3(3) = Vector3F(-1, -0.25F, -0.35F);
	pNose->GetVBuffer()->Position3(7) = Vector3F(-1, -0.25F, 0.35F);
	pNose->ModelBound->ComputeFromData(pNose->GetVBuffer());
	pNode->AttachChild(pNose);

	// We save a reference to the rotors, so we can easily access them later
	// to rotate them in the main loop.
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
	pCockpit->GetVBuffer()->Position3(3) = Vector3F(-0.5F, 0.2F, -0.35F);
	pCockpit->GetVBuffer()->Position3(7) = Vector3F(-0.5F, 0.2F, 0.35F);
	pCockpit->ModelBound->ComputeFromData(pCockpit->GetVBuffer());
	pNode->AttachChild(pCockpit);

	// The cockpit is supposed to be transparent, so we create an StateAlpha 
	// and attach it to the cockpit object.
	StateAlpha* pAlpha = WIRE_NEW StateAlpha;
	pAlpha->BlendEnabled = true;
	pCockpit->AttachState(pAlpha);

	// Now we tilt and orient the helicopter for later use.
	Matrix34F tilt(Vector3F(0, 0, 1), 0.2F);
	Matrix34F rotate(Vector3F(0, 1, 0), MathF::HALF_PI);
	pNode->Local.SetRotate(rotate * tilt);

	return pNode;
}
