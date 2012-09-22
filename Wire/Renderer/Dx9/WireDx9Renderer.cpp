// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
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
#include "WireEffect.h"
#include "WireGeometry.h"
#include "WireIndexBuffer.h"
#include "WireLight.h"
#include "WireMesh.h"
#include "WireVertexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
Renderer::Renderer(PdrRendererInput& rInput, UInt width, UInt height,
	Bool isFullscreen, Bool useVSync)
	:
	mMaxAnisotropy(1.0F),
	mIndexBufferMap(1024),
	mVertexBufferMap(1024),
	mImage2DMap(256),
	mSupportsStaticBatching(true),
	mSupportsDynamicBatching(true)
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
	rPresent.BackBufferCount = 1;
	rPresent.hDeviceWindow = rInput.WindowHandle;
	rPresent.Flags = 0;
	if (useVSync)
	{
		rPresent.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		rPresent.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	if (isFullscreen)
	{
		rPresent.Windowed = FALSE;
		rPresent.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		rPresent.BackBufferFormat = D3DFMT_A8R8G8B8;
	}
	else
	{
		rPresent.Windowed = TRUE;
		rPresent.FullScreen_RefreshRateInHz = 0;
		rPresent.BackBufferFormat = D3DFMT_UNKNOWN;
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

	mMaxTextureWidth = deviceCaps.MaxTextureWidth;
	mMaxTextureHeight = deviceCaps.MaxTextureHeight;
	mTexture2Ds.SetQuantity(deviceCaps.MaxTextureBlendStages);
	mpData->SamplerStates.SetQuantity(deviceCaps.MaxTextureBlendStages);
	mpData->TextureStageStates.SetQuantity(deviceCaps.MaxTextureBlendStages);
	UInt maxLights = deviceCaps.MaxActiveLights;
	mLights.SetQuantity(maxLights <= 0 ? 8 : maxLights);

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

	hr = -1;

#ifdef WIRE_DEBUG
	// Look for 'NVIDIA PerfHUD' adapter
	// If it is present, override default settings
	for (UINT adapter = 0; adapter < rD3D->GetAdapterCount(); adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT res = rD3D->GetAdapterIdentifier(adapter, 0, &Identifier);
		WIRE_ASSERT(SUCCEEDED(res));
		if (strstr(Identifier.Description,"PerfHUD") != 0)
		{
			// try to create a NVPerfHUD compatible adapter
			hr = rD3D->CreateDevice(
				adapter,
				D3DDEVTYPE_REF,
				rInput.WindowHandle,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
				&rPresent,
				&rDevice);
			WIRE_ASSERT(SUCCEEDED(hr));
			break;
		}
	}
#endif

	if (FAILED(hr))
	{
		hr = rD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			rInput.WindowHandle, behaviorFlags, &rPresent, &rDevice);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	if ((deviceCaps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY ) > 0)
	{
		if (deviceCaps.TextureCaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
		{
			mpData->SupportsMinFAniso = true;
		}

		if (deviceCaps.TextureCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC)
		{
			mpData->SupportsMagFAniso = true;
		}

		mMaxAnisotropy = static_cast<Float>(deviceCaps.MaxAnisotropy);
	}

	// Turn off lighting (DX defaults to lighting on).
	hr = rDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));

	hr = rDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));
	mpData->UsesRenormalizeNormals = false;

	// Initialize global render state to default settings.
	Set(State::Default);
}

//----------------------------------------------------------------------------
Renderer::~Renderer()
{
	Terminate();

	if (mpData->D3DDevice)
	{
		mpData->D3DDevice->Release();
	}

	if (mpData->D3D)
	{
		mpData->D3D->Release();
	}

	WIRE_DELETE mpData;
}

//----------------------------------------------------------------------------
void Renderer::ClearBuffers()
{
	if (mpData->IsDeviceLost)
	{
		return;
	}

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
	if (mpData->IsDeviceLost)
	{
		return;
	}

	HRESULT hr;
	hr = mpData->D3DDevice->Present(NULL, NULL, NULL, NULL);

	if (hr == D3DERR_DEVICELOST)
	{
		mpData->IsDeviceLost = true;
	}
	else
	{
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}

//----------------------------------------------------------------------------
Bool Renderer::PreDraw(Camera* pCamera)
{
	// Reset state cache (state is not preserved outside Begin/EndScene()),
	// and release smart pointers cached by the Renderer.
	ReleaseResources();

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
		if (!mpData->IsDeviceLost)
		{
			mpData->IsDeviceLost = true;
			return false;
		}

		mpData->ResetDevice();
		break;

	case D3D_OK:
		if (mpData->IsDeviceLost)
		{
			mpData->ResetDevice();
		}

		break;

	default:
		WIRE_ASSERT(false);
    }

    hr = rDevice->BeginScene();
    WIRE_ASSERT(SUCCEEDED(hr));

	Set(State::Default);

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

	ReleaseResources();
	mspCamera = NULL;
}

//----------------------------------------------------------------------------
void Renderer::SetWorldTransformation(Transformation& rWorld, Bool
	usesNormals)
{
	Bool needsRenormalization = usesNormals ? true : false;
	if (rWorld.IsUniformScale())
	{
		if (rWorld.GetUniformScale() == 1.0F)
		{
			needsRenormalization = false;
		}
	}

	HRESULT hr;
	IDirect3DDevice9*& rDevice = mpData->D3DDevice;

	if (needsRenormalization != mpData->UsesRenormalizeNormals)
	{
		DWORD normalize = needsRenormalization ? TRUE : FALSE;
		hr = rDevice->SetRenderState(D3DRS_NORMALIZENORMALS, normalize);
		WIRE_ASSERT(SUCCEEDED(hr));
		mpData->UsesRenormalizeNormals = needsRenormalization;
	}

	Matrix4F world;
	rWorld.GetHomogeneous(world);
	hr = rDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(
		&world));
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::DrawElements(UInt indexCount, UInt startIndex)
{
	WIRE_ASSERT(mspVertexBuffer);
	WIRE_ASSERT(mspIndexBuffer);

	const UInt triangleCount = indexCount/3;
	const UInt vertexCount = mspVertexBuffer->GetQuantity();
	mStatistics.DrawCalls++;
	mStatistics.Triangles += triangleCount;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
	hr = rDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount,
		startIndex, triangleCount);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::DrawElements(UInt vertexCount, UInt indexCount,
	UInt startIndex)
{
	const UInt triangleCount = indexCount/3;
	mStatistics.DrawCalls++;
	mStatistics.Triangles += triangleCount;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
	hr = rDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount,
		startIndex, triangleCount);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::SetCamera(Camera* pCamera)
{
	if (!pCamera)
	{
		return;
	}

	mspCamera = pCamera;
	OnFrameChange();
	OnViewportChange();

	const Float n = pCamera->GetDMin();
	const Float f = pCamera->GetDMax();
	const Float b = pCamera->GetUMin();
	const Float t = pCamera->GetUMax();
	const Float l = pCamera->GetRMin();
	const Float r = pCamera->GetRMax();

	const Float w = 1.0F/(r-l);
	const Float h = 1.0F/(t-b);
	const Float d = 1.0F/(f-n);

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	
	if (pCamera->IsPerspective())
	{
		D3DMATRIX matProj = {
			2.0F*n*w, 0.0F,     0.0F,  0.0F,
			0.0F,     2.0F*n*h, 0.0F,  0.0F,
			-(r+l)*w, -(t+b)*h, f*d,   1.0F,
			0.0F,     0.0F,    -n*f*d, 0.0F };

		rDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}
	else
	{
		Matrix4F orthogonalProjection(
			2.0F*w,   0.0F,     0.0F, 0.0F,
			0.0F,     2.0F*h,   0.0F, 0.0F,
			0.0F,     0.0F,     d,    0.0F,
			-(l+r)*w, -(t+b)*h, -n*d, 1.0F );

		Matrix4F halfPixelOffset(false);
		halfPixelOffset[3][0] = 0.5F;
		halfPixelOffset[3][1] = 0.5F;
		orthogonalProjection = halfPixelOffset * orthogonalProjection;

		rDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(
			&orthogonalProjection));
	}

	rDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mpData->
		ViewMatrix));
}

//----------------------------------------------------------------------------
void Renderer::OnFrameChange()
{
	WIRE_ASSERT(mspCamera);
	Vector3F eye = mspCamera->GetLocation();
	Vector3F rVector = mspCamera->GetRVector();
	Vector3F uVector = mspCamera->GetUVector();
	Vector3F dVector = mspCamera->GetDVector();

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
	if (!mspCamera)
	{
		return;
	}

	Float left;
	Float right;
	Float top;
	Float bottom;

	mspCamera->GetViewport(left, right, top, bottom);

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
	WIRE_ASSERT(mpData);
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
	D3DPOOL_DEFAULT,	// Buffer::UT_DYNAMIC_WRITE_ONLY
	D3DPOOL_MANAGED		// Buffer::UT_STATIC_DISCARD_ON_BIND
};

//----------------------------------------------------------------------------
const DWORD PdrRendererData::USAGES[Buffer::UT_QUANTITY] =
{
	D3DUSAGE_WRITEONLY,						// Buffer::UT_STATIC
	D3DUSAGE_DYNAMIC,						// Buffer::UT_DYNAMIC
	D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,	// Buffer::UT_DYNAMIC_WRITE_ONLY
	D3DUSAGE_WRITEONLY						// Buffer::UT_STATIC_DISCARD_ON_BIND
};

//----------------------------------------------------------------------------
PdrRendererData::PdrRendererData(Renderer* pRenderer)
	:
	D3D(NULL),
	D3DDevice(NULL),
	Supports32BitIndices(false),
	SupportsMinFAniso(false),
	SupportsMagFAniso(false),
	IsDeviceLost(false),
	UsesRenormalizeNormals(false),
	mpRenderer(pRenderer)
{
}

//----------------------------------------------------------------------------
template <typename Resource, typename PdrResource>
void DestroyNonManagedResources(THashTable<const Resource*,
	PdrResource*>& rMap, TArray<const Resource*>& rSave)
{
	rSave.SetMaxQuantity(rMap.GetQuantity(), false);
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
	rSave.SetMaxQuantity(rMap.GetQuantity(), false);
	while (rMap.GetQuantity() > 0)
	{
		THashTable<const Resource*, PdrResource*>::Iterator it(&rMap);
		const Resource* pKey;
		it.GetFirst(&pKey);

		Renderer::UnbindAll(pKey);
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
	Renderer& rRenderer = *mpRenderer;
	TArray<const IndexBuffer*> saveIndexBuffers;
	TArray<const VertexBuffer*> saveVertexBuffers;
	TArray<const Image2D*> saveTexture2Ds;
	DestroyResources(rRenderer.mIndexBufferMap, saveIndexBuffers);
	DestroyResources(rRenderer.mVertexBufferMap, saveVertexBuffers);
	DestroyNonManagedResources(rRenderer.mImage2DMap, saveTexture2Ds);
	
	UInt batchingSize = 0;
	if (rRenderer.mBatchedIndexBuffer)
	{
		batchingSize = rRenderer.mBatchedIndexBuffer->GetBufferSize();
		rRenderer.DestroyBatchingBuffers();
	}

	HRESULT hr;
	hr = D3DDevice->Reset(&Present);
	WIRE_ASSERT(SUCCEEDED(hr));
	IsDeviceLost = false;

	rRenderer.CreateBatchingBuffers(batchingSize);

	RecreateResources(&rRenderer, saveIndexBuffers);
	RecreateResources(&rRenderer, saveVertexBuffers);
	RecreateResources(&rRenderer, saveTexture2Ds);

	rRenderer.OnViewportChange();

	hr = D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));

	hr = D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	WIRE_ASSERT(SUCCEEDED(hr));
	UsesRenormalizeNormals = false;

	for (UInt i = 0; i < rRenderer.mLights.GetQuantity(); i++)
	{
		hr = D3DDevice->LightEnable(i, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	AlphaState.IsValid = false;
	CullState.IsValid = false;
	FogState.IsValid = false;
	LightState.IsValid = false;
	WireframeState.IsValid = false;
	ZBufferState.IsValid = false;
	for (UInt i = 0; i < TextureStageStates.GetQuantity(); i++)
	{
		TextureStageStates[i].IsValid = false;
	}

	for (UInt i = 0; i < SamplerStates.GetQuantity(); i++)
	{
		SamplerStates[i].IsValid = false;
	}

	rRenderer.Set(rRenderer.mspStates);
}
