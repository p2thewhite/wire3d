// Sample10 - Material Sorting & Batching
// This sample demonstrates sorting materials for minimizing state changes
// and correct transparency/opaque geometry order, as well as draw call
// batching of static and dynamic objects.
// Note: Batching is disabled on the Wii by default. It does not suffer
// from operating system and driver overhead issues like the PC, thus
// batching does not improve performance on the Wii.

#include "Sample10.h"

WIRE_APPLICATION(Sample10);

//----------------------------------------------------------------------------
Sample10::Sample10()
	:
	WIREAPPLICATION(
	ColorRGBA(0.0F, 0.0F, 0.0F, 1.0F),	// background color
	"Sample10 - Material Sorting",	// title of the window,
	0, 0,		// window position
	640, 480,	// window size; (use (0,0) for current desktop resolution)
	false,		// fullscreen
	false)		// vsync
{
}

//----------------------------------------------------------------------------
Bool Sample10::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspRoot = CreateScene();

	// Apply Local to World transformation
	mspRoot->UpdateGS();

	// Prepare for static batching by applying World transformation to
	// actual vertices (duplicating vertex buffers if they are shared).
	// If forceStatic is false, only objects which have WorldIsCurrent &&
	// WorldBoundIsCurrent set to true will be processed.
	Bool forceStatic = true;
	mspRoot->MakeStatic(forceStatic); // remove this line for dynamic batching

	Vector3F cameraLocation(0.0F, 0.0F, 10.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);
	mspCamera->SetFrustum(45.0F, GetWidthF()/GetHeightF(), 0.1F, 300.0F);

	mCuller.SetCamera(mspCamera);
	mCullerSorting.SetCamera(mspCamera);

	mspTextCamera = WIRE_NEW Camera(/* isPerspective */ false);
	mspText = StandardMesh::CreateText();

	// Create buffers (size in bytes) for batching draw calls.
   	GetRenderer()->CreateBatchingBuffers(10000, 50000);
	return true;
}

//----------------------------------------------------------------------------
void Sample10::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	// disable batching by setting thresholds to 0
	GetRenderer()->SetVertexBatchingThreshold(0);
	GetRenderer()->SetIndexBatchingThreshold(0);

	// Every 5 seconds we alternate between one of the following 3 states:
	// Culler with no batching (to produce a visible set of objects in the
	//   order of the objects in the scene graph)
	// CullerSorting with no batching (which produces 2 sets of visible
	//   objects: one set of opaque objects sorted by render state, material
	//   and depth (front to back), and one set of transparent objects sorted
	//   likewise (but back to front for correct visibility)).
	// CullerSorting with batching (after sorting the renderer tries to
	//   batch as many objects with the same properties together as possible)
	 
	Bool usesSorting = false;
	Culler* pCuller = &mCuller;
	if (MathF::FMod(static_cast<Float>(time), 15) > 5)
	{
		pCuller = &mCullerSorting;
		usesSorting = true;

		if (MathF::FMod(static_cast<Float>(time), 15) > 10)
		{
			// There are 2 methods of batching supported:
			// a) Batching of index buffers only:
			//    This is used if objects are batchable, share the same
			//    vertexbuffer and the same static transformation.
			//    This is the preferred way, but requires use of submeshes
			//    to share vertexbuffers amongst objects.
			//    (Use the "Combine static meshes" option in the exporter to
			//    prepare your scene objects for this method.)
            //
			// b) Batching of vertex and index buffers:
			//    This is used if objects are batchable and do not share the
			//    same vertex buffer. If the objects do not have static
			//    transformation, manual transformation is applied. This is
			//    more CPU intensive, thus the VertexBatchingThreshold should
			//    be kept low, or else more time might be wasted than gained.

			// Set thresholds for batching
			GetRenderer()->SetVertexBatchingThreshold(200);
			GetRenderer()->SetIndexBatchingThreshold(2000);
		}
	}

	Float angle = static_cast<Float>(MathD::FMod(time, MathD::TWO_PI));
	Float sinus = MathF::Sin(angle); 
	Float d = 6.0F;
	Vector3F camPos(sinus*d, sinus*d, MathF::Cos(angle)*d);
	mspCamera->LookAt(camPos, Vector3F::ZERO, Vector3F::UNIT_Y);
	mCuller.SetCamera(mspCamera);
	mspLight->Direction = -camPos;

	pCuller->ComputeVisibleSet(mspRoot);

	GetRenderer()->GetStatistics()->Reset();
	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->Draw(pCuller->GetVisibleSets());
	DrawFPS(elapsedTime, usesSorting);
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Node* Sample10::CreateScene()
{
	// Create a scene consisting of cubes. The cubes consists of 2 different
	// materials in total and are placed in the scene graph in alternating
	// order to create a worst-case scenario (the renderer has to switch
	// materials and render states in every draw call (unless CullerSorting
	// is used to alter the order of objects being rendered))

	Node* pRoot = WIRE_NEW Node;

	const UInt xCount = 5;
	const UInt yCount = 5;
	const UInt zCount = 5;
	Bool useA = true;

	Float z = static_cast<Float>(zCount-1)*-0.5F;
	for (UInt k = 0; k < yCount; k++)
	{
		Float y = static_cast<Float>(yCount-1)*-0.5F;
		for (UInt j = 0; j < yCount; j++)
		{
			Float x = static_cast<Float>(xCount-1)*-0.5F;
			for (UInt i = 0; i < xCount; i++)
			{
				Node* pNode = useA ? CreateObjectA() : CreateObjectB();
				pNode->Local.SetTranslate(Vector3F(x+i, y+j, z+k));
				pRoot->AttachChild(pNode);
				useA = !useA;
			}
		}
	}

	mspLight = WIRE_NEW Light;
	pRoot->AttachLight(mspLight);

	pRoot->UpdateRS();
	return pRoot;
}

//----------------------------------------------------------------------------
Node* Sample10::CreateObjectA()
{
	Node* pRootA = WIRE_NEW Node;

	// Create the Mesh, Material and associated render States only once and
	// use them for all instances of ObjectA. This saves memory and enables
	// the renderer to avoid redundant state and buffer changes when several
	// instances of ObjectA are rendered in succession. Use CullerSorting
	// to create such a sequence of instances to minimize changes.
	if (!mspMeshA)
	{
		RenderObjectPtr spTmp = StandardMesh::CreateCube24(0, 1, true, 0.35F);
		mspMeshA = spTmp->GetMesh();
		mspMeshA->GenerateNormals();
	}

	if (!mspMaterialA)
	{
		const UInt width = 256;
		const UInt height = 256;
		const Image2D::FormatMode format = Image2D::FM_RGBA8888;
		const UInt bpp = Image2D::GetBytesPerPixel(format);

		NoisePerlin<Float> perlin;
		const Float hf = static_cast<Float>(height) * 0.15F;
		const Float wf = static_cast<Float>(width) * 0.15F;
		UChar* const pDst = WIRE_NEW UChar[width * height * bpp];

		for (UInt y = 0; y < height; y++)
		{
			for (UInt x = 0; x < width; x++)
			{
				Float xf = static_cast<Float>(x);
				Float yf = static_cast<Float>(y);

				Float n = perlin.Noise(xf/wf, yf/hf)*0.5F
					+ 0.5F;
				UChar t = static_cast<UChar>(n * 255.0F);
				pDst[(y*width + x)*bpp] = t;
				pDst[(y*width + x)*bpp+1] = t;
				pDst[(y*width + x)*bpp+2] = t;
				pDst[(y*width + x)*bpp+3] = t;
			}
		}

		Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst);
		Texture2D* pTexture = WIRE_NEW Texture2D(pImage);

		mspMaterialA = WIRE_NEW Material;
		mspMaterialA->AddTexture(pTexture, Material::BM_MODULATE);
	}

	if (!mspStateMaterialA)
	{
		mspStateMaterialA = WIRE_NEW StateMaterial;
		mspStateMaterialA->Ambient = ColorRGBA(0.9F, 1, 1, 1);
	}

	if (!mspCullA)
	{
		mspCullA = WIRE_NEW StateCull;
		mspCullA->CullFace = StateCull::CM_FRONT;
	}

	if (!mspAlphaA)
	{
		mspAlphaA = WIRE_NEW StateAlpha;
		mspAlphaA->BlendEnabled = true;
	}

	if (!mspZBufferA)
	{
		mspZBufferA = WIRE_NEW StateZBuffer;
		mspZBufferA->Writable = false;
	}

	if (!mspVertexBufferA)
	{
		WIRE_ASSERT(mspMeshA);
		mspVertexBufferA = WIRE_NEW VertexBuffer(mspMeshA->GetVertexBuffer());
		WIRE_ASSERT(mspVertexBufferA->GetAttributes().GetNormalChannels() > 0);
		for (UInt i = 0; i < mspVertexBufferA->GetQuantity(); i++)
		{
			mspVertexBufferA->Normal3(i) = -mspVertexBufferA->Normal3(i);
		}
	}

	Node* pFront = WIRE_NEW Node(mspMeshA, mspMaterialA);
	pFront->AttachState(mspAlphaA);
	pFront->AttachState(mspZBufferA);

	Node* pBack = WIRE_NEW Node(mspVertexBufferA, mspMeshA->
		GetIndexBuffer(), mspMaterialA);
	pBack->AttachState(mspCullA);

	pRootA->AttachChild(pBack);
	pRootA->AttachChild(pFront);
	pRootA->AttachState(mspStateMaterialA);

	return pRootA;
}

//----------------------------------------------------------------------------
Node* Sample10::CreateObjectB()
{
	// See CreateObjectA() for details
	if (!mspMeshB)
	{
		RenderObjectPtr spTmp = StandardMesh::CreateCube24(0, 1, true, 0.35F);
		mspMeshB = spTmp->GetMesh();
	}

	if (!mspMaterialB)
	{
		const UInt width = 256;
		const UInt height = 256;
		const Image2D::FormatMode format = Image2D::FM_RGBA8888;
		const UInt bpp = Image2D::GetBytesPerPixel(format);

		NoisePerlin<Float> perlin;
		const Float hf = static_cast<Float>(height) * 0.2F;
		const Float wf = static_cast<Float>(width) * 0.2F;
		UChar* const pDst = WIRE_NEW UChar[width * height * bpp];

		for (UInt y = 0; y < height; y++)
		{
			for (UInt x = 0; x < width; x++)
			{
				Float xf = static_cast<Float>(x);
				Float yf = static_cast<Float>(y);

				Float n = perlin.Noise(xf/wf, yf/hf)*0.5F + 0.5F;
				UChar t = static_cast<UChar>(n * 255.0F);
				pDst[(y*width + x)*bpp] = t;
				pDst[(y*width + x)*bpp+1] = t;
				pDst[(y*width + x)*bpp+2] = t;
				pDst[(y*width + x)*bpp+3] = t;
			}
		}

		Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst);
		Texture2D* pTexture = WIRE_NEW Texture2D(pImage);

		mspMaterialB = WIRE_NEW Material;
		mspMaterialB->AddTexture(pTexture, Material::BM_MODULATE);
	}

	if (!mspStateMaterialB)
	{
		mspStateMaterialB = WIRE_NEW StateMaterial;
		mspStateMaterialB->Ambient = ColorRGBA(1, 1, 0.9F, 1);
	}

	Node* pObjectB = WIRE_NEW Node(mspMeshB, mspMaterialB);
	pObjectB->AttachState(mspStateMaterialB);

	return pObjectB;
}

//----------------------------------------------------------------------------
void Sample10::DrawFPS(Double elapsed, Bool usesSorting)
{
	// set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	mspTextCamera->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);
	GetRenderer()->SetCamera(mspTextCamera);

	// set to screen width (might change any time in window mode)
	mspText->SetLineWidth(GetWidthF());
	mspText->SetColor(Color32::WHITE);
	// Text uses OpenGL convention of (0,0) being left bottom of window
	mspText->Set("Sorting: ", 0, GetHeightF()-mspText->GetFontHeight()-32.0F);

	if (usesSorting)
	{
		mspText->Append("ON", Color32::GREEN);
	}
	else
	{
		mspText->Append("OFF", Color32::RED);
	}

	mspText->Append(", Batching: ", Color32::WHITE);
	if (GetRenderer()->UsesBatching())
	{
		mspText->Append("ON", Color32::GREEN);
	}
	else
	{
		mspText->Append("OFF", Color32::RED);
	}

	const UInt TextArraySize = 1000;
	Char text[TextArraySize];
	UInt fps = static_cast<UInt>(1/elapsed);
	String msg1 = "\nFPS: %d\nDraw Calls: %d, Triangles: %d\nBatched Static: "
		"%d, Batched Dynamic: %d\nVBOs: %d, VBOSize: %.2f KB\nIBOs: %d, "
		"IBOSize: %.2f KB\nTextures: %d, TextureSize: %.2f MB";
	Float kb = 1024.0F;
	const RendererStatistics* pStats = GetRenderer()->GetStatistics();
	System::Sprintf(text, TextArraySize, static_cast<const Char*>(msg1), fps,
		pStats->GetDrawCalls(), pStats->GetTriangles(), pStats->
		GetBatchedStatic(), pStats->GetBatchedDynamic(), pStats->
		GetVBOCount(), pStats->GetVBOsSize()/kb, pStats->GetIBOCount(),
		pStats->GetIBOsSize()/kb, pStats->GetTextureCount(), pStats->
		GetTexturesSize()/(kb*kb));
	mspText->SetColor(Color32::WHITE);
	mspText->Append(text);

	mspText->Update(GetRenderer());
	GetRenderer()->DisableLighting();
	GetRenderer()->Draw(mspText);
}
