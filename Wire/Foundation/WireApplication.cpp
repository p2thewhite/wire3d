#include "WireApplication.h"
#include "WireSystem.h"

using namespace Wire;

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
Application* Application::GetApplication() const
{
	return mpApplication;
}

//-------------------------------------------------------------------------------
// Int main (Int argumentCount, Char* arguments[])
// {
// }
