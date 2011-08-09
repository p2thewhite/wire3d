// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireDx9IndexBuffer.h"
#include "WireDx9RendererData.h"
#include "WireDx9RendererInput.h"
#include "WireDx9Texture2D.h"
#include "WireDx9VertexBuffer.h"
#include "WireGeometry.h"

using namespace Wire;

//----------------------------------------------------------------------------
Renderer::Renderer(PdrRendererInput& rInput, UInt width, UInt height,
	Bool isFullscreen)
	:
	mIndexBufferMap(1024),
	mVertexBufferMap(1024),
	mTexture2DMap(256)
{
	Initialize(width, height);

	mpData = WIRE_NEW PdrRendererData(this);
	WIRE_ASSERT(mpData);

	IDirect3D9*& rD3D = mpData->D3D;
	rD3D = Direct3DCreate9(D3D_SDK_VERSION);
	WIRE_ASSERT(rD3D);

	D3DPRESENT_PARAMETERS& rPresent = mpData->Present;
	rPresent.BackBufferWidth = width;
	rPresent.BackBufferHeight = height;
	rPresent.BackBufferFormat = D3DFMT_A8R8G8B8;
	rPresent.BackBufferCount = 1;
	rPresent.hDeviceWindow = rInput.WindowHandle;
	rPresent.Windowed = isFullscreen ? FALSE : TRUE;
	rPresent.Flags = 0;
	rPresent.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //D3DPRESENT_INTERVAL_IMMEDIATE;
	if (isFullscreen)
	{
		rPresent.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}
	{
		rPresent.FullScreen_RefreshRateInHz = 0;
	}

	// Request depth and stencil buffers. The parameters are not independent
	// for DirectX. For now, just grab a 24-bit depth buffer and an 8-bit
	// stencil buffer.
	rPresent.EnableAutoDepthStencil = TRUE;
	rPresent.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Request multi sampling.
	rPresent.SwapEffect = D3DSWAPEFFECT_FLIP; 
	rPresent.MultiSampleType = D3DMULTISAMPLE_NONE;
	rPresent.MultiSampleQuality = 0;

	// Query the device for its capabilities.
	D3DCAPS9 deviceCaps;
	HRESULT hr;
	hr = rD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		&deviceCaps);
	WIRE_ASSERT(SUCCEEDED(hr));

	mpData->Supports32BitIndices = deviceCaps.MaxVertexIndex > 0xffff ?
		true : false;

	mMaxTextureStages = deviceCaps.MaxTextureBlendStages;
	mMaxLights = deviceCaps.MaxActiveLights;
	if (mMaxLights <= 0)
	{
		mMaxLights = 8;
	}

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

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	hr = rD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		rInput.WindowHandle, behaviorFlags, &rPresent, &rDevice);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Turn off lighting (DX defaults to lighting on).
	hr = rDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));

	if ((deviceCaps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY ) > 0)
	{
		mMaxAnisotropy = static_cast<Float>(deviceCaps.MaxAnisotropy);
	}

	// Initialize global render state to default settings.
	SetStates(State::Default);
}

//----------------------------------------------------------------------------
Renderer::~Renderer()
{
	if (mpData->D3DDevice)
	{
		mpData->D3DDevice->Release();
	}

	if (mpData->D3D)
	{
		mpData->D3D->Release();
	}

	WIRE_DELETE mpData;

	Terminate();
}

//----------------------------------------------------------------------------
void Renderer::ClearBuffers()
{
	DWORD clearColor = D3DCOLOR_COLORVALUE(mClearColor.R(),
		mClearColor.G(), mClearColor.B(), mClearColor.A());

	HRESULT hr;
	hr = mpData->D3DDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER /*| D3DCLEAR_STENCIL*/,
		clearColor, 1.0F/*mClearDepth*/, static_cast<DWORD>(0/*mClearStencil*/));
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::DisplayBackBuffer()
{
	HRESULT hr;
	hr = mpData->D3DDevice->Present(NULL, NULL, NULL, NULL);
	if (hr != D3DERR_DEVICELOST)
	{
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}

//----------------------------------------------------------------------------
Bool Renderer::PreDraw(Camera* pCamera)
{
	SetCamera(pCamera);

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
	hr = rDevice->TestCooperativeLevel();
    
    switch (hr)
    {
    case D3DERR_DEVICELOST:
		mpData->IsDeviceLost = true;
		return false;

	case D3DERR_DEVICENOTRESET:
		mpData->ResetDevice();
		break;
    }

    hr = rDevice->BeginScene();
    WIRE_ASSERT(SUCCEEDED(hr));

	// reset state cache (state is not preserved outside Begin/EndScene())
	for (UInt i = 0; i < State::MAX_STATE_TYPE; i++)
	{
		mspStates[i] = NULL;
	}

	SetStates(State::Default);

	return true;
}

//----------------------------------------------------------------------------
void Renderer::PostDraw()
{
	HRESULT hr;
	hr = mpData->D3DDevice->EndScene();
	if (!mpData->IsDeviceLost)
	{
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}

//----------------------------------------------------------------------------
void Renderer::DrawElements()
{
	// Set up world matrix
	Matrix4F world;
	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	mpGeometry->World.GetHomogeneous(world);
	rDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&world));

	HRESULT hr;
	hr = rDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		mpGeometry->GetVBuffer()->GetVertexQuantity(), 0,
		mpGeometry->GetIBuffer()->GetIndexQuantity()/3);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::SetCamera(Camera* pCamera)
{
	if (!pCamera)
	{
		return;
	}

	mpCamera = pCamera;
	OnFrameChange();
	OnViewportChange();

	Float n = pCamera->GetDMin();
	Float f = pCamera->GetDMax();
	Float b = pCamera->GetUMin();
	Float t = pCamera->GetUMax();
	Float l = pCamera->GetRMin();
	Float r = pCamera->GetRMax();
	D3DMATRIX matProj = {
		2.0F*n/(r-l),	0.0F,			(r+l)/(r-l),	0.0F,
		0.0F,			2.0F*n/(t-b),	(t+b)/(t-b),	0.0F,
		0.0F,			0.0F,			f/(f-n),		1.0F,
		0.0F,			0.0F,			-n*f/(f-n),		0.0F};

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	rDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	rDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mpData->
		ViewMatrix));
}

//----------------------------------------------------------------------------
void Renderer::OnFrameChange()
{
	Vector3F eye = mpCamera->GetLocation();
	Vector3F rVector = mpCamera->GetRVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F dVector = mpCamera->GetDVector();

	Matrix4F& rViewMatrix = mpData->ViewMatrix;
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
void Renderer::OnViewportChange()
{
	if (!mpCamera)
	{
		return;
	}

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
	hr = mpData->D3DDevice->SetViewport(&viewport);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::SetClearColor(const ColorRGBA& rClearColor)
{
	mClearColor = rClearColor;
}

//----------------------------------------------------------------------------
void Renderer::Resize(UInt width, UInt height)
{
	mWidth = width;
	mHeight = height;

	mpData->Present.BackBufferWidth = width;
	mpData->Present.BackBufferHeight = height;

	mpData->ResetDevice();
}

//----------------------------------------------------------------------------
const DWORD PdrRendererData::BUFFER_LOCKING[Buffer::LM_QUANTITY] = 
{
	D3DLOCK_READONLY,           // Buffer::LM_READ_ONLY
	D3DLOCK_DISCARD,            // Buffer::LM_WRITE_ONLY
	0                           // Buffer::LM_READ_WRITE
};

//----------------------------------------------------------------------------
const D3DPOOL PdrRendererData::POOLS[Buffer::UT_QUANTITY] =
{
	D3DPOOL_MANAGED,	// Buffer::UT_STATIC
	D3DPOOL_DEFAULT,	// Buffer::UT_DYNAMIC
	D3DPOOL_DEFAULT		// Buffer::UT_DYNAMIC_WRITE_ONLY
};

//----------------------------------------------------------------------------
const DWORD PdrRendererData::USAGES[Buffer::UT_QUANTITY] =
{
	D3DUSAGE_WRITEONLY,						// Buffer::UT_STATIC
	D3DUSAGE_DYNAMIC,						// Buffer::UT_DYNAMIC
	D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY	// Buffer::UT_DYNAMIC_WRITE_ONLY
};

//----------------------------------------------------------------------------
PdrRendererData::PdrRendererData(Renderer* pRenderer)
	:
	IsDeviceLost(false),
	mpRenderer(pRenderer)
{
}

//----------------------------------------------------------------------------
template <typename Resource, typename PdrResource>
void DestroyNonManagedResources(THashTable<const Resource*,
	PdrResource*>& rMap, TArray<const Resource*>& rSave)
{
	rSave.SetMaxQuantity(rMap.GetQuantity());
	const Resource* pKey;
	THashTable<const Resource*, PdrResource*>::Iterator it(&rMap);

	for (PdrResource** pValue = it.GetFirst(&pKey); pValue;
		pValue = it.GetNext(&pKey))
	{
		const Buffer* pBuffer = reinterpret_cast<const Buffer*>(pKey);
		if (PdrRendererData::POOLS[pBuffer->GetUsage()] != D3DPOOL_MANAGED)
		{
			WIRE_DELETE *pValue;
			rSave.Append(pKey);
			rMap.Remove(pKey);
		}
	}
}

//----------------------------------------------------------------------------
template <typename Resource, typename PdrResource>
void DestroyResources(THashTable<const Resource*,
	PdrResource*>& rMap, TArray<const Resource*>& rSave)
{
	rSave.SetMaxQuantity(rMap.GetQuantity());
	const Resource* pKey;
	for (PdrResource** pValue = rMap.GetFirst(&pKey); pValue;
		pValue = rMap.GetNext(&pKey))
	{
		WIRE_DELETE *pValue;
		rSave.Append(pKey);
	}

	rMap.RemoveAll();
}

//----------------------------------------------------------------------------
template <typename Resource>
void RecreateResources(Renderer* pRenderer, TArray<const Resource*>& rSave)
{
	for (UInt i = 0; i < rSave.GetQuantity(); i++)
	{
		pRenderer->Bind(rSave[i]);
	}
}

//----------------------------------------------------------------------------
void PdrRendererData::ResetDevice()
{
	Renderer& renderer = *mpRenderer;
	TArray<const IndexBuffer*> saveIndexBuffers;
	TArray<const VertexBuffer*> saveVertexBuffers;
	TArray<const Texture2D*> saveTexture2Ds;
	DestroyNonManagedResources(renderer.mIndexBufferMap, saveIndexBuffers);
	DestroyNonManagedResources(renderer.mVertexBufferMap, saveVertexBuffers);
	DestroyNonManagedResources(renderer.mTexture2DMap, saveTexture2Ds);

	HRESULT hr;
	hr = D3DDevice->Reset(&Present);
	WIRE_ASSERT(SUCCEEDED(hr));
	IsDeviceLost = false;

	RecreateResources(&renderer, saveIndexBuffers);
	RecreateResources(&renderer, saveVertexBuffers);
	RecreateResources(&renderer, saveTexture2Ds);

	renderer.OnViewportChange();

	hr = D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));

	for (UInt i = 0; i < renderer.mMaxLights; i++)
	{
		hr = D3DDevice->LightEnable(i, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	renderer.SetStates(renderer.mspStates);
}
