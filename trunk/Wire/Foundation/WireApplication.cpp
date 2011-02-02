#include "WireApplication.h"
#include "WireMain.h"

using namespace Wire;

Application* Application::smpApplication = NULL;

//----------------------------------------------------------------------------
Application::Application(const ColorRGBA backgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool isFullscreen)
	:
	mpRenderer(NULL),
	mBackgroundColor(backgroundColor),
	mpWindowTitle(pWindowTitle),
	mXPosition(xPosition),
	mYPosition(yPosition),
	mWidth(width),
	mHeight(height),
	mIsFullscreen(isFullscreen)
{
}

//----------------------------------------------------------------------------
Application::~Application()
{
}

//----------------------------------------------------------------------------
void Application::SetApplication(Application* pApplication)
{
	smpApplication = pApplication;
}

//----------------------------------------------------------------------------
Application* Application::GetApplication()
{
	return smpApplication;
}

//----------------------------------------------------------------------------
Int main(Int argc, Char** argv)
{
	Main::Initialize();

	Application* pApplication = Application::GetApplication();
	if (pApplication)
	{
		pApplication->Main(argc, argv);

		WIRE_DELETE pApplication;
		Application::SetApplication(NULL);
	}

	Main::Terminate();

	return 0;
}
