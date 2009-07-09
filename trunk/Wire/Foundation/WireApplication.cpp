#include "WireApplication.h"
#include "WireMain.h"

using namespace Wire;

Application* Application::mpApplication = NULL;

//----------------------------------------------------------------------------
Application::Application()
	:
	mpRenderer(NULL)
{
}

//----------------------------------------------------------------------------
Application::~Application()
{
}

//----------------------------------------------------------------------------
void Application::SetApplication(Application* pApplication)
{
	mpApplication = pApplication;
}

//----------------------------------------------------------------------------
Application* Application::GetApplication()
{
	return mpApplication;
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

	return 0;
}
