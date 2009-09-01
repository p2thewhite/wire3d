#include "WireRenderer.h"
#include "WireCamera.h"

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

//----------------------------------------------------------------------------
void Renderer::OnFrameChange()
{
	Vector3F eye = mpCamera->GetLocation();
	Vector3F rVector = mpCamera->GetRVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F dVector = mpCamera->GetDVector();

	mViewMatrix[0][0] = rVector[0];
	mViewMatrix[1][0] = uVector[0];
	mViewMatrix[2][0] = dVector[0];
	mViewMatrix[0][1] = rVector[1];
	mViewMatrix[1][1] = uVector[1];
	mViewMatrix[2][1] = dVector[1];
	mViewMatrix[0][2] = rVector[2];
	mViewMatrix[1][2] = uVector[2];
	mViewMatrix[2][2] = dVector[2];
	mViewMatrix[0][3] = -rVector.Dot(eye);
	mViewMatrix[1][3] = -uVector.Dot(eye);
	mViewMatrix[2][3] = -dVector.Dot(eye);
}
