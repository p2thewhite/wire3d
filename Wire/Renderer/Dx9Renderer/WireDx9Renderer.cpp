#include "WireDx9Renderer.h"

#include "WireCamera.h"
#include "WireDx9RendererData.h"
#include "WireGeometry.h"
#include <d3dx9.h>

using namespace Wire;

//----------------------------------------------------------------------------
Dx9Renderer::Dx9Renderer(HWND hWnd, Int width, Int height)
	:
	Renderer(width, height)
{
	mpData = WIRE_NEW PdrRendererData;
	WIRE_ASSERT(mpData);

	IDirect3D9*& rD3D = mpData->mpD3D;
	rD3D = Direct3DCreate9(D3D_SDK_VERSION);
	WIRE_ASSERT(rD3D);

	D3DPRESENT_PARAMETERS& rPresent = mpData->mPresent;
	rPresent.BackBufferWidth = width;
	rPresent.BackBufferHeight = height;
	rPresent.BackBufferFormat = D3DFMT_A8R8G8B8;
	rPresent.BackBufferCount = 1;
	rPresent.hDeviceWindow = hWnd;
	rPresent.Windowed = true;
	rPresent.Flags = 0;
	rPresent.FullScreen_RefreshRateInHz = 0;
	rPresent.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Request depth and stencil buffers. The parameters are not independent
	// for DirectX. For now, just grab a 24-bit depth buffer and an 8-bit
	// stencil buffer.
	rPresent.EnableAutoDepthStencil = TRUE;
	rPresent.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Request multi sampling.
	rPresent.SwapEffect = D3DSWAPEFFECT_FLIP; 
	rPresent.MultiSampleType = D3DMULTISAMPLE_NONE;
	rPresent.MultiSampleQuality = 0;

	// TODO: support multi sampling

	// Query the device for its capabilities.
	D3DCAPS9 deviceCaps;
	HRESULT hr;
	hr = rD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		&deviceCaps);
	WIRE_ASSERT(SUCCEEDED(hr));

	mpData->mSupports32BitIndices = deviceCaps.MaxVertexIndex > 0xffff ?
		true : false;

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

	IDirect3DDevice9*& rDevice = mpData->mpD3DDevice;
	hr = rD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		behaviorFlags, &rPresent, &rDevice);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Turn off lighting (DX defaults to lighting on).
	hr = rDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));

	msMaxAnisotropy = static_cast<Float>(deviceCaps.MaxAnisotropy);

	// Initialize global render state to default settings.
	SetGlobalState(GlobalState::Default);
}

//----------------------------------------------------------------------------
Dx9Renderer::~Dx9Renderer()
{
	if (mpData->mpD3DDevice)
	{
		mpData->mpD3DDevice->Release();
	}

	if (mpData->mpD3D)
	{
		mpData->mpD3D->Release();
	}

	WIRE_DELETE mpData;
}

//----------------------------------------------------------------------------
void Dx9Renderer::ClearBuffers()
{
	DWORD clearColor = D3DCOLOR_COLORVALUE(mClearColor.R(),
		mClearColor.G(), mClearColor.B(), mClearColor.A());

	HRESULT hr;
	hr = mpData->mpD3DDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		clearColor, 1.0F/*mClearDepth*/, static_cast<DWORD>(0/*mClearStencil*/));
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Dx9Renderer::DisplayBackBuffer()
{
	HRESULT hr;
	hr = mpData->mpD3DDevice->Present(NULL, NULL, NULL, NULL);
	if (hr != D3DERR_DEVICELOST)
	{
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}

//----------------------------------------------------------------------------
void Dx9Renderer::ResetDevice()
{
	HRESULT hr;
	hr = mpData->mpD3DDevice->Reset(&mpData->mPresent);
	WIRE_ASSERT(SUCCEEDED(hr));

	OnViewportChange();
	OnFrameChange();
}

//----------------------------------------------------------------------------
Bool Dx9Renderer::BeginScene(Camera* pCamera)
{
	OnBeginScene(pCamera);

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

	IDirect3DDevice9*& rDevice = mpData->mpD3DDevice;
	rDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	rDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mpData->
		mViewMatrix));

	HRESULT hr;
	hr = rDevice->TestCooperativeLevel();
    
    switch (hr)
    {
    case D3DERR_DEVICELOST:
		// TODO: handle device lost
		return false;

	case D3DERR_DEVICENOTRESET:
        ResetDevice();
        break;
    }

    hr = rDevice->BeginScene();
    WIRE_ASSERT(SUCCEEDED(hr));

	return true;
}

//----------------------------------------------------------------------------
void Dx9Renderer::EndScene()
{
	HRESULT hr;
	hr = mpData->mpD3DDevice->EndScene();
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Dx9Renderer::DrawElements()
{
	// Set up world matrix
	Matrix4F world;
	IDirect3DDevice9*& rDevice = mpData->mpD3DDevice;
	mpGeometry->World.GetHomogeneous(world);
	rDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&world));

	HRESULT hr;
	hr = rDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		mpGeometry->VBuffer->GetVertexQuantity(), 0,
		mpGeometry->IBuffer->GetIndexQuantity()/3);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnFrameChange()
{
	Vector3F eye = mpCamera->GetLocation();
	Vector3F rVector = mpCamera->GetRVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F dVector = mpCamera->GetDVector();

	Matrix4F& rViewMatrix = mpData->mViewMatrix;
	rViewMatrix[0][0] = rVector[0];
	rViewMatrix[0][1] = uVector[0];
	rViewMatrix[0][2] = dVector[0];
	rViewMatrix[0][3] = 0.0F;
	rViewMatrix[1][0] = rVector[1];
	rViewMatrix[1][1] = uVector[1];
	rViewMatrix[1][2] = dVector[1];
	rViewMatrix[1][3] = 0.0F;
	rViewMatrix[2][0] = rVector[2];
	rViewMatrix[2][1] = uVector[2];
	rViewMatrix[2][2] = dVector[2];
	rViewMatrix[2][3] = 0.0F;
	rViewMatrix[3][0] = -rVector.Dot(eye);
	rViewMatrix[3][1] = -uVector.Dot(eye);
	rViewMatrix[3][2] = -dVector.Dot(eye);
	rViewMatrix[3][3] = 1.0F;
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnViewportChange()
{
	Float left;
	Float right;
	Float top;
	Float bottom;

	mpCamera->GetViewport(left, right, top, bottom);

	// DirectX defines the full-sized viewport to have origin at the upper
	// left corner of the screen. Wire uses the OpenGL convention that
	// 'bottom' specifies the relative distance from the bottom of the
	// screen. DirectX needs a specification of relative distance from the
	// top of the screen, which is 1 - 'top'.
	D3DVIEWPORT9 viewport;
	viewport.X = static_cast<DWORD>(left * mWidth);
	viewport.Y = static_cast<DWORD>((1.0F - top) * mHeight);// See note above.
	viewport.Width = static_cast<DWORD>((right - left) * mWidth);
	viewport.Height = static_cast<DWORD>((top - bottom) * mHeight);
	viewport.MinZ = 0.0F;
	viewport.MaxZ = 1.0F;

	HRESULT hr;
	hr = mpData->mpD3DDevice->SetViewport(&viewport);
	WIRE_ASSERT(SUCCEEDED(hr));
}
