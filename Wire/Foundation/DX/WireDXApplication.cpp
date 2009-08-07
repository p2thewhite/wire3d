#include "WireDXApplication.h"
#include "../../Renderer/DXRenderer/WireDXRenderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
DXApplication::DXApplication()
{
}

//----------------------------------------------------------------------------
DXApplication::~DXApplication()
{
}

//----------------------------------------------------------------------------
Int DXApplication::Main(Int argumentQuantity, Char* arguments[])
{
	// Avoid VC warning until this is actually being used
	argumentQuantity;
	arguments;
	
	mpRenderer = WIRE_NEW DXRenderer;

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
void DXApplication::OnTerminate()
{
	if (mpRenderer)
	{
		WIRE_DELETE mpRenderer;
		mpRenderer = NULL;
	}
}

//----------------------------------------------------------------------------
Bool DXApplication::OnInitialize()
{
	mpRenderer->SetClearColor(ColorRGBA::BLACK);
	return true;
}

//----------------------------------------------------------------------------
void DXApplication::OnIdle()
{
}
