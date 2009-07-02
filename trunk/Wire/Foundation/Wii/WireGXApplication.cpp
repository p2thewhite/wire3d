#include "WireGXApplication.h"
#include "../Foundation/WireSystem.h"
#include "../../Renderer/GXRenderer/WireGXRenderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
GXApplication::GXApplication()
{
}

//----------------------------------------------------------------------------
GXApplication::~GXApplication()
{
	OnTerminate();
}

//----------------------------------------------------------------------------
Int GXApplication::Run(Int argumentQuantity, Char* arguments[])
{
	return 0;
}

//----------------------------------------------------------------------------
Int GXApplication::Main()
{
	mpRenderer = WIRE_NEW GXRenderer;

	if (!mpApplication->OnInitialize())
	{

	}

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
