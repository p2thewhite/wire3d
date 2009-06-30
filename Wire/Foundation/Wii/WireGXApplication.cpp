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
	// TODO: move to OnTerminate
	if (mpRenderer)
	{
		WIRE_DELETE mpRenderer;
		mpRenderer = NULL;
	}
}

//----------------------------------------------------------------------------
Int GXApplication::Main()
{
	mpRenderer = WIRE_NEW GXRenderer;
	mpRenderer->SetClearColor(ColorRGBA::BLACK);

	return 0;
}

//----------------------------------------------------------------------------
Bool GXApplication::OnInitialize()
{
	return true;
}
