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
	if (!smpApplication->OnPrecreate())
	{
		return -1;
	}

	PdrRendererInput input;
	input.BackgroundColor = mBackgroundColor;
	mpRenderer = WIRE_NEW Renderer(input, 0, 0, mIsFullscreen);
	smpApplication->KEY_TERMINATE = Application::KEY_ESCAPE;

	// VIInit must be called before PADInit
	PADInit();
	WPAD_ScanPads();
	if (smpApplication->OnInitialize())
	{
		while (!(WPAD_ButtonsDown(0) & smpApplication->KEY_TERMINATE))
		{
			smpApplication->OnIdle();
			WPAD_ScanPads();
		}
	}

	smpApplication->OnTerminate();

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
