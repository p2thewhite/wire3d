#include "WireGXApplication.h"
#include "../../Renderer/GXRenderer/WireGXRenderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
GXApplication::GXApplication()
{
}

//----------------------------------------------------------------------------
GXApplication::~GXApplication()
{
}

//----------------------------------------------------------------------------
Int GXApplication::Main(Int argumentQuantity, Char* arguments[])
{
	mpRenderer = WIRE_NEW GXRenderer;

	if (mpApplication->OnInitialize())
	{
		WPAD_ScanPads();
		while(!(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME))
		{
			mpApplication->OnIdle();
			WPAD_ScanPads();
		}
	}

	mpApplication->OnTerminate();

	return 0;
}

//----------------------------------------------------------------------------
void GXApplication::OnTerminate()
{
	if (mpRenderer)
	{
		WIRE_DELETE mpRenderer;
		mpRenderer = NULL;
	}
}

//----------------------------------------------------------------------------
Bool GXApplication::OnInitialize()
{
	mpRenderer->SetClearColor(ColorRGBA::BLACK);
	return true;
}

//----------------------------------------------------------------------------
void GXApplication::OnIdle()
{
}
