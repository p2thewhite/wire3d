#include "WireDx9Renderer.h"

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
}

//----------------------------------------------------------------------------
Dx9Renderer::~Dx9Renderer()
{
}

//----------------------------------------------------------------------------
void Dx9Renderer::DisplayBackBuffer()
{
}

//----------------------------------------------------------------------------
void Dx9Renderer::DrawElements()
{
}
