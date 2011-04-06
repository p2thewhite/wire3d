#include "WireFoundation.h"
#include "WireEngine.h"
#include "WireGXRendererInput.h"

using namespace Wire;

//----------------------------------------------------------------------------
Int main(Int argc, Char** argv)
{
	Main::Initialize();

	PdrRendererInput input;
	Renderer* pRenderer = WIRE_NEW Renderer(input, 0, 0, true);

	// Renderer must be created before calling PADInit
	PADInit();
	WPAD_ScanPads();

	GeometryPtr spCube = StandardMesh::CreateCube8(/* RGB(A) channels */ 4);
	Vector3F cameraLocation(0.0F, 0.0F, 10.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	CameraPtr spCamera = WIRE_NEW Camera(cameraLocation, viewDirection, up);

	while (!(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME))
	{
		Matrix34F rotate(Vector3F(0.2F, 0.7F, 0.1F), MathF::FMod(
			static_cast<Float>(System::GetTime()), MathF::TWO_PI));
		spCube->World.SetRotate(rotate);

		pRenderer->ClearBuffers();
		pRenderer->PreDraw(spCamera);
		pRenderer->Draw(spCube);
		pRenderer->PostDraw();
		pRenderer->DisplayBackBuffer();
		
		WPAD_ScanPads();
	}

	// dereference (i.e. destroy) before destroying the renderer
	spCamera = NULL;
	spCube = NULL;

	WIRE_DELETE pRenderer;
	Main::Terminate();

	return 0;
}
