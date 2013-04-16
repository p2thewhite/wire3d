// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9Shader.h"

#include "WireRenderer.h"
#include "WireShader.h"

using namespace Wire;

//----------------------------------------------------------------------------
PdrShader::PdrShader(Renderer* pRenderer, const Shader* pShader)
	:
	mpConstantTable(NULL),
	mpPixelShader(NULL),
	mpVertexShader(NULL)
{
	WIRE_ASSERT(pShader);
	HRESULT hr;

	LPD3DXBUFFER pCode = NULL;
	DWORD dwShaderFlags = 0;
	LPD3DXBUFFER pBufferErrors = NULL;

	hr = D3DXCompileShader(pShader->GetProgram(), pShader->GetProgramLength(),
		NULL, NULL, pShader->GetFunction(), pShader->GetProfile(),
		dwShaderFlags, &pCode, &pBufferErrors, &mpConstantTable);

	if (FAILED(hr))
	{
		LPVOID pCompileErrors = pBufferErrors->GetBufferPointer();
		MessageBox(NULL, static_cast<const char*>(pCompileErrors),
			"Shader Compile Error", MB_OK|MB_ICONEXCLAMATION);
	}

	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;

	if (pShader->GetProfile()[0] == 'p')
	{
		hr = rDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(),
			&mpPixelShader);
	}
	else
	{
		WIRE_ASSERT(pShader->GetProfile()[0] == 'v');
		hr = rDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(),
			&mpVertexShader);
	}

	WIRE_ASSERT(SUCCEEDED(hr));

	pCode->Release();
	if (pBufferErrors)
	{
		pBufferErrors->Release();
	}
}

//----------------------------------------------------------------------------
PdrShader::~PdrShader()
{
	if (mpPixelShader)
	{
		mpPixelShader->Release();
	}
	else
	{
		WIRE_ASSERT(mpVertexShader);
		mpVertexShader->Release();
	}

	mpConstantTable->Release();
}

//----------------------------------------------------------------------------
void PdrShader::Enable(Renderer* pRenderer)
{
	HRESULT hr;
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	if (mpPixelShader)
	{
		hr = rDevice->SetPixelShader(mpPixelShader);
	}
	else
	{
		WIRE_ASSERT(mpVertexShader);
		hr = rDevice->SetVertexShader(mpVertexShader);
	}

	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrShader::Disable(Renderer* pRenderer)
{
	HRESULT hr;
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	if (mpPixelShader)
	{
		hr = rDevice->SetPixelShader(NULL);
	}
	else
	{
		WIRE_ASSERT(mpVertexShader);
		hr = rDevice->SetVertexShader(NULL);
	}

	WIRE_ASSERT(SUCCEEDED(hr));
}
