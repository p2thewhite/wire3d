#include "WireGXApplication.h"

#include "WireGXRendererInput.h"
#include "WireRenderer.h"

using namespace Wire;

const UInt Application::KEY_ESCAPE = WPAD_BUTTON_HOME;

//----------------------------------------------------------------------------
GXApplication::GXApplication(const ColorRGBA backgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool)
	:
	Application(backgroundColor, pWindowTitle, xPosition, yPosition, width,
		height, true)
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
	// allow work to be done before the renderer is created
	if (!mpApplication->OnPrecreate())
	{
		return -1;
	}

	PdrRendererInput input;
	input.BackgroundColor = mBackgroundColor;
	mpRenderer = WIRE_NEW Renderer(input, 0, 0, mIsFullscreen);
	mpApplication->KEY_TERMINATE = Application::KEY_ESCAPE;

	// VIInit must be called before PADInit
	PADInit();
	WPAD_ScanPads();
	if (mpApplication->OnInitialize())
	{
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
Bool GXApplication::OnPrecreate()
{
	return true;
}

//----------------------------------------------------------------------------
Bool GXApplication::OnInitialize()
{
	return true;
}

//----------------------------------------------------------------------------
void GXApplication::OnTerminate()
{
}

//----------------------------------------------------------------------------
void GXApplication::OnIdle()
{
}
