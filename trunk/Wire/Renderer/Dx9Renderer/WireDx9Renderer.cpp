#include "WireDx9Renderer.h"

#include "WireDx9Resources.h"

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

	// Turn off culling
	msResult = mpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	WIRE_ASSERT(SUCCEEDED(msResult));

	// Turn on the zbuffer
	msResult = mpDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
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
Bool Dx9Renderer::BeginScene(Camera*)
{
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
void Dx9Renderer::OnLoadIBuffer(ResourceIdentifier*& rID,
	IndexBuffer* pIBuffer)
{
	// The index buffer is encountered the first time.
 	IBufferID* pResource = WIRE_NEW IBufferID;
 	rID = pResource;

	UInt quantity = pIBuffer->GetIndexQuantity();
	UInt* pIndices = pIBuffer->GetData();
	UInt indexBufferSize = quantity * sizeof(UInt);

	// Create the index buffer.
	D3DFORMAT format = mSupports32BitIndices ? D3DFMT_INDEX32 :
		D3DFMT_INDEX16;
	LPDIRECT3DINDEXBUFFER9 pD3DIBuffer;
	msResult = mpDevice->CreateIndexBuffer(indexBufferSize,
		D3DUSAGE_WRITEONLY, format, D3DPOOL_MANAGED, &pD3DIBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(msResult));
 
	// Copy the index buffer data from system memory to video memory.
	Char* pLockedIBuffer;
	msResult = pD3DIBuffer->Lock(0, indexBufferSize,
		(void**)(&pLockedIBuffer), 0);
	WIRE_ASSERT(SUCCEEDED(msResult));

	if (mSupports32BitIndices)
	{
		System::Memcpy(pLockedIBuffer, indexBufferSize, pIndices,
			indexBufferSize);
	}
	else
	{
		if ((quantity % 3) == 0)
		{
			for (UInt i = 0; i < quantity; i+=3)
			{
				((UShort*)pLockedIBuffer)[i] = static_cast<UShort>(
					pIndices[i]);
				((UShort*)pLockedIBuffer)[i+1] = static_cast<UShort>(
					pIndices[i+1]);
				((UShort*)pLockedIBuffer)[i+2] = static_cast<UShort>(
					pIndices[i+2]);
			}
		}
		else
		{
			for (UInt i = 0; i < quantity; i++)
			{
				((UShort*)pLockedIBuffer)[i] = static_cast<UShort>(
					pIndices[i]);
			}
		}
	}

	msResult = pD3DIBuffer->Unlock();
	WIRE_ASSERT(SUCCEEDED(msResult));

	// Generate the binding information and save it.
	pResource->ID = pD3DIBuffer;
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnReleaseIBuffer(ResourceIdentifier* pID)
{
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	pResource->ID->Release();
	WIRE_DELETE pResource;
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnEnableIBuffer(ResourceIdentifier* pID)
{
	// Bind the current index buffer.
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	msResult = mpDevice->SetIndices(pResource->ID);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::DrawElements()
{
}
