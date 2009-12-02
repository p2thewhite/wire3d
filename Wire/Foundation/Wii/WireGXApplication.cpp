#include "WireGXApplication.h"
#include "../../Renderer/GXRenderer/WireGXRenderer.h"

using namespace Wire;

const UInt Application::KEY_ESCAPE = WPAD_BUTTON_HOME;

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
	WIRE_DELETE mpRenderer;
	mpRenderer = NULL;
}

//----------------------------------------------------------------------------
Int GXApplication::Main(Int argumentQuantity, Char* arguments[])
{
	mpRenderer = WIRE_NEW GXRenderer(mBackgroundColor);
	mpApplication->KEY_TERMINATE = Application::KEY_ESCAPE;

	if (mpApplication->OnInitialize())
	{
		WPAD_ScanPads();
		while (!(WPAD_ButtonsDown(0) & mpApplication->KEY_TERMINATE))
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
