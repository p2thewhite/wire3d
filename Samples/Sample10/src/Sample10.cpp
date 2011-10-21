// Sample10 - Material Sorting

#include "Sample10.h"

using namespace Wire;

WIRE_APPLICATION(Sample10);		// This macro creates our user application.

//----------------------------------------------------------------------------
Sample10::Sample10()
	:
	WIREAPPLICATION(
	ColorRGBA(0.0F, 0.0F, 0.0F, 1.0F),	// background color
	// The following parameters are PC only:
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

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, 0.0F, 5.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	UInt width = GetRenderer()->GetWidth();
	UInt height = GetRenderer()->GetHeight();
	Float fieldOfView = 45.0F;
	Float aspectRatio = static_cast<Float>(width)/static_cast<Float>(height);
	Float nearPlane = 0.1F;
	Float farPlane = 300.0F;
	mspCamera->SetFrustum(fieldOfView, aspectRatio, nearPlane, farPlane);
	mCuller.SetCamera(mspCamera);
	mCullerSorting.SetCamera(mspCamera);

	mspTextCamera = WIRE_NEW Camera(/* isPerspective */ false);
	mspTextAlpha = WIRE_NEW StateAlpha();
	mspTextAlpha->BlendEnabled = true;

	return true;
}

//----------------------------------------------------------------------------
void Sample10::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	Matrix34F rotate(Vector3F(0.2F, 0.7F, 0.1F),
		MathF::FMod(static_cast<Float>(time), MathF::TWO_PI));
	mspRoot->Local.SetRotate(rotate);

	mspRoot->UpdateGS(time);

	Bool usesSorting = false;
	Culler* pCuller = &mCuller;
	if (MathF::FMod(static_cast<Float>(time), 10) > 5)
	{
		pCuller = &mCullerSorting;
		usesSorting = true;
	}

	pCuller->ComputeVisibleSet(mspRoot);

	GetRenderer()->ResetStatistics();
	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->DrawScene(pCuller->GetVisibleSets());
	DrawFPS(elapsedTime, usesSorting);
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Node* Sample10::CreateScene()
{
	Node* pRoot = WIRE_NEW Node;

	const UInt xCount = 5;
	const UInt yCount = 5;
	const UInt zCount = 5;
	Bool useA = true;

	Float z = static_cast<Float>(zCount)*-0.5F;
	for (UInt k = 0; k < yCount; k++)
	{
		Float y = static_cast<Float>(yCount)*-0.5F;
		for (UInt j = 0; j < yCount; j++)
		{
			Float x = static_cast<Float>(xCount)*-0.5F;
			for (UInt i = 0; i < xCount; i++)
			{
				Spatial* pGeo = useA ? CreateGeometryA() : CreateGeometryB();
				pGeo->Local.SetTranslate(Vector3F(x+i, y+j, z+k));
				pRoot->AttachChild(pGeo);
				useA = !useA;
			}
		}
	}

	Light* pLight = WIRE_NEW Light;
	pRoot->AttachLight(pLight);

	pRoot->UpdateRS();
	return pRoot;
}

//----------------------------------------------------------------------------
Spatial* Sample10::CreateGeometryA()
{
	Node* pRootA = WIRE_NEW Node;

	if (!mspMeshA)
	{
		GeometryPtr spTmp = StandardMesh::CreateCube24(0, 1, true, 0.35F);
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
		const Float hf = static_cast<Float>(height) * 0.25F;
		const Float wf = static_cast<Float>(width) * 0.25F;
		UChar* const pDst = WIRE_NEW UChar[width * height * bpp];

		for (UInt y = 0; y < height; y++)
		{
			for (UInt x = 0; x < width; x++)
			{
				Float xf = static_cast<Float>(x);
				Float yf = static_cast<Float>(y);

				UChar t = static_cast<UChar>((perlin.Noise(xf/wf, yf/hf)*0.5F
					+ 0.5F) * 255.0F);
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

	Geometry* pGeo = WIRE_NEW Geometry(mspMeshA, mspMaterialA);
	pGeo->AttachState(mspStateMaterialA);

	Geometry* pGeo2 = WIRE_NEW Geometry(mspVertexBufferA, mspMeshA->
		GetIndexBuffer(), mspMaterialA);
	pGeo2->AttachState(mspStateMaterialA);
	pGeo2->AttachState(mspCullA);

	pRootA->AttachChild(pGeo);
	pRootA->AttachChild(pGeo2);

	pRootA->AttachState(mspAlphaA);
	pRootA->AttachState(mspZBufferA);

	return pRootA;
}

//----------------------------------------------------------------------------
Spatial* Sample10::CreateGeometryB()
{
	if (!mspMeshB)
	{
		GeometryPtr spTmp = StandardMesh::CreateCube24(0, 1, true, 0.35F);
		mspMeshB = spTmp->GetMesh();
	}

	if (!mspMaterialB)
	{
		const UInt width = 512;
		const UInt height = 128;

		Image2D::FormatMode format = Image2D::FM_RGB888;
		const UInt bpp = Image2D::GetBytesPerPixel(format);

		UChar* pMap = WIRE_NEW UChar[width * height * bpp];
		UInt seed = 7;
		for (UInt x = 0; x < width*bpp; x++)
		{
			pMap[x] = seed;
		}

		for (UInt i = width; i < (width * height)-1; i++)
		{
			seed *= 0x06255;
			seed = (seed >> 7) | (seed << (32-7));
			UShort t0 = ((pMap[(i-width)*bpp] + pMap[(i-width+1)*bpp]) >> 1);
			t0 += (7 & seed) - 1;
			UChar texel = t0 > 255 ? 255 : t0;
			for (UInt j = 0; j < bpp; j++)
			{
				pMap[i*bpp+j] = texel;
			}
		}

		Image2D* pImage = WIRE_NEW Image2D(format, width, height, pMap);
		Texture2D* pTexture = WIRE_NEW Texture2D(pImage);

		mspMaterialB = WIRE_NEW Material;
		mspMaterialB->AddTexture(pTexture, Material::BM_MODULATE);
	}

	if (!mspStateMaterialB)
	{
		mspStateMaterialB = WIRE_NEW StateMaterial;
		mspStateMaterialB->Ambient = ColorRGBA(1, 1, 0.9F, 1);
	}

	Geometry* pGeo = WIRE_NEW Geometry(mspMeshB, mspMaterialB);
	pGeo->AttachState(mspStateMaterialB);

	return pGeo;
}

//----------------------------------------------------------------------------
void Sample10::DrawFPS(Double elapsed, Bool usesSorting)
{
	// set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
	mspTextCamera->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);
	GetRenderer()->SetCamera(mspTextCamera);

	Renderer::Statistics statistics = GetRenderer()->GetStatistics();

	UInt fps = static_cast<UInt>(1/elapsed);
	const UInt TextArraySize = 1000;
	Char text[TextArraySize];

	String msg0 = "\n\n\n\n\n\nSorting: ";
	String msg1 = "\2\nDraw Calls: %d\nTriangles: %d, Vertices: %d\nFPS: %d";
	String str;

	if (usesSorting)
	{
		str = msg0 + "\x01\xff\x01\x01\xffON" + msg1;
		System::Sprintf(text, TextArraySize, static_cast<const Char*>(str),
			statistics.DrawCalls, statistics.Triangles, statistics.Vertices, fps);
		WIRE_ASSERT(text[msg0.GetLength()] == 1);
		WIRE_ASSERT(text[msg0.GetLength()+1] == 0x00);
	}
	else
	{
		str = msg0 + "\x01\x01\x7f\x01\x7fOFF" + msg1;
		System::Sprintf(text, TextArraySize, static_cast<const Char*>(str),
			statistics.DrawCalls, statistics.Triangles, statistics.Vertices, fps);
	}


	GeometryPtr spText = StandardMesh::CreateText(text, screenWidth,
		screenHeight, ColorRGBA::WHITE);
	spText->AttachState(mspTextAlpha);
	spText->UpdateRS();

	GetRenderer()->DisableLighting();
	GetRenderer()->Draw(spText);
}
