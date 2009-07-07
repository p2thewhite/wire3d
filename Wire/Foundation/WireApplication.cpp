#include "WireApplication.h"

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

//-------------------------------------------------------------------------------
// Int main (Int argumentCount, Char* arguments[])
// {
// }
