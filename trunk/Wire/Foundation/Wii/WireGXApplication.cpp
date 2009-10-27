#include "WireGXApplication.h"
#include "../../Renderer/GXRenderer/WireGXRenderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
GXApplication::GXApplication(const ColorRGBA backgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, Int width, Int height)
	:
	Application(backgroundColor, pWindowTitle, xPosition, yPosition, width,
		height)
{
}

//----------------------------------------------------------------------------
GXApplication::~GXApplication()
{
}

//----------------------------------------------------------------------------
Int GXApplication::Main(Int argumentQuantity, Char* arguments[])
{
	mpRenderer = WIRE_NEW GXRenderer(mBackgroundColor);

	if (mpApplication->OnInitialize())
	{
		WPAD_ScanPads();
		while (!(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME))
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
	WIRE_DELETE mpRenderer;
	mpRenderer = NULL;
}

//----------------------------------------------------------------------------
Bool GXApplication::OnInitialize()
{
	return true;
}

//----------------------------------------------------------------------------
void GXApplication::OnIdle()
{
}
