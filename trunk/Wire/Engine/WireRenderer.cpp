#include "WireRenderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
Renderer::Renderer()
{
}

//----------------------------------------------------------------------------
Renderer::~Renderer()
{
}

//----------------------------------------------------------------------------
void Renderer::Draw(Geometry* pGeometry)
{
	mpGeometry = pGeometry;

	DrawElements();
}

//----------------------------------------------------------------------------
void Renderer::SetClearColor(const ColorRGBA& rClearColor)
{
	mClearColor = rClearColor;
}
