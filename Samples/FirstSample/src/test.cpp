#include "DemoInit.h"

DemoInit DEMO;

using namespace Wire;

void CameraInit(Matrix34f& rView);
Geometry* CreateCube();
Geometry* CreatePyramid();

//-------------------------------------------------------------------------
Geometry* CreateCube()
{
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(3);
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes, 24);

	Vector3f colors[] = {
		Vector3f(0.0f,1.0f,0.0f),
		Vector3f(0.0f,1.0f,0.0f),
		Vector3f(0.0f,1.0f,0.0f),
		Vector3f(0.0f,1.0f,0.0f),

		Vector3f(1.0f,0.5f,0.0f),
		Vector3f(1.0f,0.5f,0.0f),
		Vector3f(1.0f,0.5f,0.0f),
		Vector3f(1.0f,0.5f,0.0f),

		Vector3f(1.0f,0.0f,0.0f),
		Vector3f(1.0f,0.0f,0.0f),
		Vector3f(1.0f,0.0f,0.0f),
		Vector3f(1.0f,0.0f,0.0f),

		Vector3f(1.0f,1.0f,0.0f),
		Vector3f(1.0f,1.0f,0.0f),
		Vector3f(1.0f,1.0f,0.0f),
		Vector3f(1.0f,1.0f,0.0f),

		Vector3f(0.0f,0.0f,1.0f),
		Vector3f(0.0f,0.0f,1.0f),
		Vector3f(0.0f,0.0f,1.0f),
		Vector3f(0.0f,0.0f,1.0f),

		Vector3f(1.0f,0.0f,1.0f),
		Vector3f(1.0f,0.0f,1.0f),
		Vector3f(1.0f,0.0f,1.0f),
		Vector3f(1.0f,0.0f,1.0f)
	};

	Vector3f vertices[] = {
		Vector3f(1.0f, 1.0f,-1.0f),
		Vector3f(-1.0f, 1.0f,-1.0f),
		Vector3f(-1.0f, 1.0f, 1.0f),
		Vector3f(1.0f, 1.0f, 1.0f),

		Vector3f(1.0f,-1.0f, 1.0f),
		Vector3f(-1.0f,-1.0f, 1.0f),
		Vector3f(-1.0f,-1.0f,-1.0f),
		Vector3f(1.0f,-1.0f,-1.0f),

		Vector3f(1.0f, 1.0f, 1.0f),
		Vector3f(-1.0f, 1.0f, 1.0f),
		Vector3f(-1.0f,-1.0f, 1.0f),
		Vector3f(1.0f,-1.0f, 1.0f),

		Vector3f(1.0f,-1.0f,-1.0f),
		Vector3f(-1.0f,-1.0f,-1.0f),
		Vector3f(-1.0f, 1.0f,-1.0f),
		Vector3f(1.0f, 1.0f,-1.0f),

		Vector3f(-1.0f, 1.0f, 1.0f),
		Vector3f(-1.0f, 1.0f,-1.0f),
		Vector3f(-1.0f,-1.0f,-1.0f),
		Vector3f(-1.0f,-1.0f, 1.0f),

		Vector3f(1.0f, 1.0f,-1.0f),
		Vector3f(1.0f, 1.0f, 1.0f),
		Vector3f(1.0f,-1.0f, 1.0f),
		Vector3f(1.0f,-1.0f,-1.0f)
	};

	for (unsigned int i = 0; i < pCubeVerts->GetQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		pCubeVerts->Color3(i) = colors[i];
	}

	Geometry* pCube = WIRE_NEW QuadMesh(pCubeVerts);
	return pCube;
}

//-------------------------------------------------------------------------
Geometry* CreatePyramid()
{
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(3);
	VertexBuffer* pPyramidVerts = WIRE_NEW VertexBuffer(attributes, 12);

	Vector3f colors[] = {
		Vector3f(1.0f,0.0f,0.0f),			// Set The Color To Red
		Vector3f(0.0f,1.0f,0.0f),			// Set The Color To Green
		Vector3f(0.0f,0.0f,1.0f),			// Set The Color To Blue

		Vector3f(1.0f,0.0f,0.0f),			// Set The Color To Red
		Vector3f(0.0f,0.0f,1.0f),			// Set The Color To Blue
		Vector3f(0.0f,1.0f,0.0f),			// Set The Color To Green

		Vector3f(1.0f,0.0f,0.0f),			// Set The Color To Red
		Vector3f(0.0f,0.0f,1.0f),			// Set The Color To Blue
		Vector3f(0.0f,1.0f,0.0f),			// Set The Color To Green

		Vector3f(1.0f,0.0f,0.0f),			// Set The Color To Red
		Vector3f(0.0f,0.0f,1.0f),			// Set The Color To Blue
		Vector3f(0.0f,1.0f,0.0f)			// Set The Color To Green
	};

	Vector3f vertices[] = {
		Vector3f( 0.0f, 1.0f, 0.0f),	// Top of Triangle (front)
		Vector3f(-1.0f,-1.0f, 1.0f),	// Left of Triangle (front)
		Vector3f( 1.0f,-1.0f, 1.0f),	// Right of Triangle (front)

		Vector3f( 0.0f, 1.0f, 0.0f),		// Top of Triangle (Right)
		Vector3f( 1.0f,-1.0f, 1.0f),	// Left of Triangle (Right)
		Vector3f( 1.0f,-1.0f,-1.0f),	// Right of Triangle (Right)

		Vector3f( 0.0f, 1.0f, 0.0f),		// Top of Triangle (Back)
		Vector3f(-1.0f,-1.0f,-1.0f),	// Left of Triangle (Back)
		Vector3f( 1.0f,-1.0f,-1.0f),	// Right of Triangle (Back)

		Vector3f( 0.0f, 1.0f, 0.0f),	// Top of Triangle (Left)
		Vector3f(-1.0f,-1.0f,-1.0f),	// Left of Triangle (Left)
		Vector3f(-1.0f,-1.0f, 1.0f)	// Right of Triangle (Left)
	};

	for (unsigned int i = 0; i < pPyramidVerts->GetQuantity(); i++)
	{
		pPyramidVerts->Position3(i) = vertices[i];
		pPyramidVerts->Color3(i) = colors[i];
	}

	Geometry* pPyramid = WIRE_NEW TriMesh(pPyramidVerts);
	return pPyramid;
}

//-------------------------------------------------------------------------
int main(int argc, char** argv)
{
	Renderer* pRenderer = WIRE_NEW GXRenderer;

	// The smart pointer automatically deletes the object when it goes out
	// of scope and no other references to the object exist.
	GeometryPtr spCube = CreateCube();
	GeometryPtr spPyramid = CreatePyramid();

	DEMO.Init();
	Matrix34f view;
 	CameraInit(view);
 	pRenderer->SetClearColor(ColorRGBA::BLACK);

	float rtri = 0.0f , rquad = 0.0f, angle = 0.0f;

	WPAD_ScanPads();

	while(!(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME))
	{
		float scaleFactor = Mathf::Sin(angle);
		angle += M_PI / 180.0f;
		angle = Mathf::FMod(angle, M_PI);

		pRenderer->View = &view;
		pRenderer->BeginScene();
		DEMO.BeforeRender();

		Matrix34f model(Vector3f(0, 1, 0),DegToRad(rtri));
		spPyramid->Local.SetRotate(model);
		spPyramid->Local.SetTranslate(Vector3f(-1.5f,0.0f,-6.0f));
		spPyramid->Local.SetUniformScale(scaleFactor + 0.5f);
		GXSetCullMode(GX_CULL_NONE);
		pRenderer->Draw(spPyramid);

		model.FromAxisAngle(Vector3f(1, 1, 1),DegToRad(rquad));
		spCube->Local.SetRotate(model);
		spCube->Local.SetTranslate(Vector3f(1.5f,0.0f,-7.0f));
		spCube->Local.SetScale(Vector3f(scaleFactor + 0.5f, 1.0f, 1.0f));
		GXSetCullMode(GX_CULL_BACK);
		pRenderer->Draw(spCube);

		pRenderer->EndScene();
		pRenderer->DisplayBackBuffer();
		DEMO.DoneRender();

		rtri+=0.5f;		// Increase The Rotation Variable For The Triangle
		rquad-=0.15f;	// Decrease The Rotation Variable For The Quad
		WPAD_ScanPads();
	}

	WIRE_DELETE pRenderer;

	return 0;
}

//-------------------------------------------------------------------------
void CameraInit(Matrix34f& rView)
{
	// setup our camera at the origin
	// looking down the -z axis with y up
	Vector3f cam(0.0f, 0.0f, 0.0f);
	Vector3f up(0.0f, 1.0f, 0.0f);
	Vector3f look(0.0f, 0.0f, -1.0f);

	// die casterei hier bleibt nat�rlich nicht so.
	MTXLookAt(rView,
		reinterpret_cast<Vector*>(static_cast<float*>(cam)), 
		reinterpret_cast<Vector*>(static_cast<float*>(up)), 
		reinterpret_cast<Vector*>(static_cast<float*>(look)));

	// setup our projection matrix
	// this creates a perspective matrix with a view angle of 90,
	// and aspect ratio based on the display resolution
	f32 w = DEMO.GetRenderMode()->viWidth;
	f32 h = DEMO.GetRenderMode()->viHeight;
	Matrix4f perspective;
	MTXPerspective(perspective, 45, (f32)w/h, 0.1F, 300.0F);
	GXSetProjection(perspective, GX_PERSPECTIVE);
}
