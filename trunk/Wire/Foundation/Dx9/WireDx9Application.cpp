#include "WireDx9Application.h"
#include "../../Renderer/Dx9Renderer/WireDx9Renderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
Dx9Application::Dx9Application()
{
}

//----------------------------------------------------------------------------
Dx9Application::~Dx9Application()
{
}

//----------------------------------------------------------------------------
Int Dx9Application::Main(Int argumentQuantity, Char* arguments[])
{
	// Avoid VC warning until this is actually being used
	argumentQuantity;
	arguments;
	
	mpRenderer = WIRE_NEW Dx9Renderer;

	if (mpApplication->OnInitialize())
	{
		while (true)
		{
			mpApplication->OnIdle();
		}
	}

	mpApplication->OnTerminate();

	return 0;
}

//----------------------------------------------------------------------------
void Dx9Application::OnTerminate()
{
	WIRE_DELETE mpRenderer;
	mpRenderer = NULL;
}

//----------------------------------------------------------------------------
Bool Dx9Application::OnInitialize()
{
	mpRenderer->SetClearColor(ColorRGBA::BLACK);
	return true;
}

//----------------------------------------------------------------------------
void Dx9Application::OnIdle()
{
}
