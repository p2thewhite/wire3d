// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9RenderTarget.h"

#include "WireDx9RendererData.h"
#include "WireDx9Texture2D.h"
#include "WireRenderer.h"
#include "WireRenderTarget.h"

using namespace Wire;

//----------------------------------------------------------------------------
PdrRenderTarget::PdrRenderTarget(Renderer* pRenderer, const RenderTarget*
	pRenderTarget)
	:
	mpDepthStencilTexture(NULL),
	mpDepthStencilSurface(NULL),
	mBufferSize(0),
	mpSaveColorSurface(NULL),
	mpSaveDepthStencilSurface(NULL)
{
	HRESULT hr;
	UInt targetQuantity = pRenderTarget->GetTargetQuantity();
	if (targetQuantity > pRenderer->GetMaxSimultaneousRenderTargets())
	{
		// Don't make assumptions on the number of supported render targets.
		// Query the renderer about how many RTs are supported by the device.
		WIRE_ASSERT(false);
		targetQuantity = pRenderer->GetMaxSimultaneousRenderTargets();
	}

	mColorTextures.SetMaxQuantity(targetQuantity);
	mColorSurfaces.SetMaxQuantity(targetQuantity);

	mWidth = pRenderTarget->GetWidth();
	mHeight = pRenderTarget->GetHeight();
	mFormat = pRenderTarget->GetFormat();
	mHasDepthStencil = pRenderTarget->HasDepthStencil();

	for (UInt i = 0; i < targetQuantity; ++i)
	{
		PdrTexture2D* pPdrTexture = WIRE_NEW PdrTexture2D(pRenderer,
			mFormat, mWidth, mHeight, pRenderTarget->HasMipmaps());

		mColorTextures.Append(pPdrTexture);
		mBufferSize += pPdrTexture->GetBufferSize();
		IDirect3DSurface9* pSurface;
 		hr = mColorTextures[i]->mpBuffer->GetSurfaceLevel(0, &pSurface);
		WIRE_ASSERT(SUCCEEDED(hr));
		mColorSurfaces.Append(pSurface);
	}

	if (mHasDepthStencil)
	{
		mpDepthStencilTexture = WIRE_NEW PdrTexture2D(pRenderer,
			Image2D::FM_D24S8, mWidth, mHeight, false);
		mBufferSize += mpDepthStencilTexture->GetBufferSize();

		hr = mpDepthStencilTexture->mpBuffer->GetSurfaceLevel(0,
			&mpDepthStencilSurface);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}

//----------------------------------------------------------------------------
PdrRenderTarget::~PdrRenderTarget()
{
	for (UInt i = 0; i < mColorTextures.GetQuantity(); i++)
	{
		mColorSurfaces[i]->Release();
		WIRE_DELETE mColorTextures[i];
	}

	if (mpDepthStencilTexture)
	{
		mpDepthStencilSurface->Release();
		WIRE_DELETE mpDepthStencilTexture;
	}
}

//----------------------------------------------------------------------------
void PdrRenderTarget::Enable(Renderer* pRenderer)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;

	hr = rDevice->GetRenderTarget(0, &mpSaveColorSurface);
	WIRE_ASSERT(SUCCEEDED(hr));

	// The viewport is automatically set by this call.
	for (UInt i = 0; i < mColorSurfaces.GetQuantity(); ++i)
	{
		hr = rDevice->SetRenderTarget((DWORD)i, mColorSurfaces[i]);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	if (mHasDepthStencil)
	{
		hr = rDevice->GetDepthStencilSurface(&mpSaveDepthStencilSurface);
		WIRE_ASSERT(SUCCEEDED(hr));

		hr = rDevice->SetDepthStencilSurface(mpDepthStencilSurface);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}

//----------------------------------------------------------------------------
void PdrRenderTarget::Disable(Renderer* pRenderer)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;

	// The viewport is automatically restored by this call.
	hr = rDevice->SetRenderTarget(0, mpSaveColorSurface);
	WIRE_ASSERT(SUCCEEDED(hr));

	mpSaveColorSurface->Release();

	for (UInt i = 1; i < mColorSurfaces.GetQuantity(); ++i)
	{
		hr = rDevice->SetRenderTarget((DWORD)i, NULL);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	if (mHasDepthStencil)
	{
		hr = rDevice->SetDepthStencilSurface(mpSaveDepthStencilSurface);
		WIRE_ASSERT(SUCCEEDED(hr));

		mpSaveDepthStencilSurface->Release();
	}
}

//----------------------------------------------------------------------------
Image2D* PdrRenderTarget::ReadColor(UInt i, Renderer* pRenderer)
{
	if (i >= mColorTextures.GetQuantity())
	{
		WIRE_ASSERT(false);
		return NULL;
	}

	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;

	// Enable the input render target surface.
	if (i == 0)
	{
		hr = rDevice->GetRenderTarget(0, &mpSaveColorSurface);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	hr = rDevice->SetRenderTarget((DWORD)i, mColorSurfaces[i]);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Make a duplicate in system memory.
	IDirect3DTexture9* pCopyTexture = NULL;
	// TODO:
	hr = D3DXCreateTexture
		(
		rDevice,
		(UINT)mWidth,
		(UINT)mHeight,
		0,
		0,
		PdrRendererData::IMAGE2D_FORMAT[mFormat],
		D3DPOOL_SYSTEMMEM,
		&pCopyTexture
		);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Get the surface associated with the copy.
	IDirect3DSurface9* pCopySurface = 0;
	hr = pCopyTexture->GetSurfaceLevel(0, &pCopySurface);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Copy the render target surface.
	hr = rDevice->GetRenderTargetData(mColorSurfaces[i], pCopySurface);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Get the data to write to disk.
	D3DLOCKED_RECT rect;
	hr = pCopySurface->LockRect(&rect, 0, 0);
	WIRE_ASSERT(SUCCEEDED(hr));

	size_t size = mWidth*mHeight*Image2D::GetBytesPerPixel(mFormat);
	UChar* pData = WIRE_NEW UChar[size];
	System::Memcpy(pData, size, rect.pBits, size);
	Image2D* pImage = WIRE_NEW Image2D(mFormat, mWidth, mHeight, pData,
		false, Buffer::UT_STATIC, 1);
	hr = pCopySurface->UnlockRect();
	WIRE_ASSERT(SUCCEEDED(hr));

	pCopySurface->Release();
	pCopyTexture->Release();

	// Restore the previous render target surface.
	if (i == 0)
	{
		hr = rDevice->SetRenderTarget(0, mpSaveColorSurface);
		WIRE_ASSERT(SUCCEEDED(hr));

		mpSaveColorSurface->Release();
	}

	return pImage;
}
