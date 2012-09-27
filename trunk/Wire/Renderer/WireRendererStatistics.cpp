// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRendererStatistics.h"

#include "WireMesh.h"
#include "WireRenderer.h"
#include "WireText.h"

using namespace Wire;

//----------------------------------------------------------------------------
RendererStatistics::RendererStatistics()
	:
	mpRenderer(NULL),
	mFpsSamplesIndex(0)
{
	for (UInt i = 0; i < FPS_SAMPLE_QUANTITY; i++)
	{
		mFpsSamples[i] = 0;
	}
}

//----------------------------------------------------------------------------
void RendererStatistics::Reset()
{
	mDrawCalls = 0;
	mTriangles = 0;
	mBatchedStatic = 0;
	mBatchedDynamic = 0;
}

//----------------------------------------------------------------------------
void RendererStatistics::AppendToText(Text* pText)
{
	const UInt textArraySize = 256;
	Char text[textArraySize];
	const Char msg[] = "Draw Calls: %d, Triangles: %d\nBatched Static: "
		"%d, Batched Dynamic: %d\nVBOs: %d, VBOSize: %.2f KB\nIBOs: %d, "
		"IBOSize: %.2f KB\nTextures: %d, TextureSize: %.2f MB";

	const Float kb = 1024.0F;

	System::Sprintf(text, textArraySize, msg, mDrawCalls, mTriangles, 
		mBatchedStatic, mBatchedDynamic, mVBOCount, mVBOTotalSize / kb,
		mIBOCount, mIBOTotalSize / kb, mTextureCount, mTextureTotalSize /
		(kb * kb));

	pText->Append(text);
}

//----------------------------------------------------------------------------
void RendererStatistics::AppendToText(Text* pText, Float fps,
	Bool useAverageFps)
{
	const UInt textArraySize = 32;
	Char text[textArraySize];
	Int roundedFps = useAverageFps ? AverageFps(fps) : static_cast<Int>(
		MathF::Round(fps)); 

	System::Sprintf(text, textArraySize, "FPS: %d\n", roundedFps);
	pText->Append(text);
	AppendToText(pText);
}

//----------------------------------------------------------------------------
void RendererStatistics::Draw(Text* pText, Float fps, Bool useAverageFps,
	Camera* pCamera)
{
	if (!mpRenderer)
	{
		return;
	}

	if (!pCamera)
	{
		if (!mspCamera)
		{
			mspCamera = WIRE_NEW Camera(/* isPerspective */ false);
		}

		pCamera = mspCamera;
		Float screenHeight = static_cast<Float>(mpRenderer->GetHeight());
		Float screenWidth = static_cast<Float>(mpRenderer->GetWidth());
		pCamera->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);

		pText->SetLineWidth(screenWidth);
		pText->Clear();
		pText->SetPen(0, screenHeight - pText->GetFontHeight() - 32.0F);
	}

	AppendToText(pText, fps, useAverageFps);
	pText->Update(mpRenderer);

	Camera* pCurrentCamera = mpRenderer->GetCamera();
	mpRenderer->SetCamera(pCamera);
	mpRenderer->DisableLighting();
	mpRenderer->Draw(pText);
	mpRenderer->SetCamera(pCurrentCamera);
}

//----------------------------------------------------------------------------
Int RendererStatistics::AverageFps(Float currentFps)
{
	WIRE_ASSERT(mFpsSamplesIndex < FPS_SAMPLE_QUANTITY);

	mFpsSamples[mFpsSamplesIndex] = MathF::Round(currentFps);
	Float averageFps = 0;
	for (UInt i = 0; i < FPS_SAMPLE_QUANTITY; i++)
	{
		averageFps += mFpsSamples[i];
	}

	mFpsSamplesIndex++;
	if (mFpsSamplesIndex >= FPS_SAMPLE_QUANTITY)
	{
		mFpsSamplesIndex = 0;
	}

	averageFps /= FPS_SAMPLE_QUANTITY;
	return static_cast<Int>(MathF::Round(averageFps));
}
