#include "WireDx9Renderer.h"

#include <d3dx9.h>

using namespace Wire;

HRESULT Dx9Renderer::msResult = 0;

//----------------------------------------------------------------------------
Dx9Renderer::Dx9Renderer(HWND hWnd, Int width, Int height)
	:
	Renderer(width, height)
{
	mpMain = Direct3DCreate9(D3D_SDK_VERSION);
	WIRE_ASSERT(mpMain);

	mPresent.BackBufferWidth = width;
	mPresent.BackBufferHeight = height;
	mPresent.BackBufferFormat = D3DFMT_A8R8G8B8;
	mPresent.BackBufferCount = 1;
	mPresent.hDeviceWindow = hWnd;
	mPresent.Windowed = true;
	mPresent.Flags = 0;
	mPresent.FullScreen_RefreshRateInHz = 0;
	mPresent.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Request depth and stencil buffers. The parameters are not independent
	// for DirectX. For now, just grab a 24-bit depth buffer and an 8-bit
	// stencil buffer.
	mPresent.EnableAutoDepthStencil = TRUE;
	mPresent.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Request multi sampling.
	mPresent.SwapEffect = D3DSWAPEFFECT_FLIP; 
	mPresent.MultiSampleType = D3DMULTISAMPLE_NONE;
	mPresent.MultiSampleQuality = 0;

	// TODO: support multi sampling

	// Query the device for its capabilities.
	D3DCAPS9 deviceCaps;
	msResult = mpMain->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		&deviceCaps);
	WIRE_ASSERT(SUCCEEDED(msResult));

	mSupports32BitIndices = deviceCaps.MaxVertexIndex > 0xffff ? true : false;

	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders
	// in HW then switch to SWVP.
	DWORD behaviorFlags = D3DCREATE_FPU_PRESERVE;
	if((deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		deviceCaps.VertexShaderVersion < D3DVS_VERSION(1, 1))
	{
		behaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		behaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	msResult = mpMain->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, hWnd, behaviorFlags, &mPresent, &mpDevice);
	WIRE_ASSERT(SUCCEEDED(msResult));

	// Turn off lighting (DX defaults to lighting on).
	msResult = mpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(msResult));

	// Set culling to clockwise
	msResult = mpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
Dx9Renderer::~Dx9Renderer()
{
}

//----------------------------------------------------------------------------
void Dx9Renderer::ClearBuffers()
{
	DWORD clearColor = D3DCOLOR_COLORVALUE(mClearColor.R(),
		mClearColor.G(), mClearColor.B(), mClearColor.A());

	msResult = mpDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		clearColor, 1.0F/*mClearDepth*/, static_cast<DWORD>(0/*mClearStencil*/));
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::DisplayBackBuffer()
{
	msResult = mpDevice->Present(NULL, NULL, NULL, NULL);
	if (msResult != D3DERR_DEVICELOST)
	{
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
}

//----------------------------------------------------------------------------
void Dx9Renderer::ResetDevice()
{
	msResult = mpDevice->Reset(&mPresent);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
Bool Dx9Renderer::BeginScene(Camera* pCamera)
{
	Parent::BeginScene(pCamera);

	Float n = pCamera->GetDMin();
	Float f = pCamera->GetDMax();
	Float b = pCamera->GetUMin();
	Float t = pCamera->GetUMax();
	Float l = pCamera->GetRMin();
	Float r = pCamera->GetRMax();
	D3DXMATRIXA16 matProj(
		2.0F*n/(r-l),	0.0F,			(r+l)/(r-l),	0.0F,
		0.0F,			2.0F*n/(t-b),	(t+b)/(t-b),	0.0F,
		0.0F,			0.0F,			f/(f-n),		1.0F,
		0.0F,			0.0F,			-n*f/(f-n),		0.0F);

	mpDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	mpDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(
		&mViewMatrix));


	msResult = mpDevice->TestCooperativeLevel();
    
    switch (msResult)
    {
    case D3DERR_DEVICELOST:
		// TODO: handle device lost
		return false;

	case D3DERR_DEVICENOTRESET:
        ResetDevice();
        break;
    }

    msResult = mpDevice->BeginScene();
    WIRE_ASSERT(SUCCEEDED(msResult));

	return true;
}

//----------------------------------------------------------------------------
void Dx9Renderer::EndScene()
{
	msResult = mpDevice->EndScene();
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::DrawElements()
{
	// Set up world matrix
	Matrix4F world;
	mpGeometry->World.GetHomogeneous(world);
	mpDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&world));

	msResult = mpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		mpGeometry->VBuffer->GetVertexQuantity(), 0,
		mpGeometry->IBuffer->GetIndexQuantity()/3);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnFrameChange()
{
	Vector3F eye = mpCamera->GetLocation();
	Vector3F rVector = mpCamera->GetRVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F dVector = mpCamera->GetDVector();

	mViewMatrix[0][0] = rVector[0];
	mViewMatrix[0][1] = uVector[0];
	mViewMatrix[0][2] = dVector[0];
	mViewMatrix[0][3] = 0.0F;
	mViewMatrix[1][0] = rVector[1];
	mViewMatrix[1][1] = uVector[1];
	mViewMatrix[1][2] = dVector[1];
	mViewMatrix[1][3] = 0.0F;
	mViewMatrix[2][0] = rVector[2];
	mViewMatrix[2][1] = uVector[2];
	mViewMatrix[2][2] = dVector[2];
	mViewMatrix[2][3] = 0.0F;
	mViewMatrix[3][0] = -rVector.Dot(eye);
	mViewMatrix[3][1] = -uVector.Dot(eye);
	mViewMatrix[3][2] = -dVector.Dot(eye);
	mViewMatrix[3][3] = 1.0F;
}
