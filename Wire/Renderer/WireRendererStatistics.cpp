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

#ifdef WIRE_WII
#include "WireGXRendererData.h"
#else
#include "WireDx9RendererData.h"
#endif

using namespace Wire;

//----------------------------------------------------------------------------
RendererStatistics::RendererStatistics()
	:
	mVBOCount(0),
	mVBOsSize(0),
	mIBOCount(0),
	mIBOsSize(0),
	mTextureCount(0),
	mTexturesSize(0),
	mBatchIBOSize(0),
	mBatchedIBOData(0),
	mBatchVBOCount(0),
	mBatchVBOsSize(0),
	mBatchedVBOData(0),
	mVertexFormatCount(0),
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
	mBatchCount = 0;
	mBatchedStatic = 0;
	mBatchedDynamic = 0;
	mTriangles = 0;
	mBatchedVBOData = 0;
	mBatchedIBOData = 0;

	if (mpRenderer && mpRenderer->GetRendererData())
	{
		mpRenderer->GetRendererData()->ResetStatistics();
	}
}

//----------------------------------------------------------------------------
void RendererStatistics::AppendToText(Text* pText)
{
	const UInt textArraySize = 512;
	Char text[textArraySize];
	const Float kb = 1024.0F;
	const Float mb = kb * kb;

	const Char msg[] = "Draw Calls: %d, Triangles: %d\n"
		"VBOs: %d / %.2f KB, VFs: %d\n"
		"IBOs: %d / %.2f KB\n"
		"Textures: %d / %.2f MB\n";

	System::Sprintf(text, textArraySize, msg,
		mDrawCalls, mTriangles, 
		mVBOCount, mVBOsSize / kb, mVertexFormatCount,
		mIBOCount, mIBOsSize / kb,
		mTextureCount, mTexturesSize / mb);

	pText->Append(text);

	WIRE_ASSERT(mpRenderer);
	if (mpRenderer->UsesBatching())
	{
		const Char msg[] =
			"Batched Static: %d, Batched Dynamic: %d, Batches: %d\n"
			"BatchIBO: %.2f KB, Batched IBO Data: %.2f KB\n";
		System::Sprintf(text, textArraySize, msg,
			mBatchedStatic, mBatchedDynamic, mBatchCount,
			mBatchIBOSize / kb, mBatchedIBOData / kb);
		pText->Append(text);

		if (mBatchVBOCount > 0)
		{
			const Char msg[] =
				"BatchVBOs: %.2f KB x%d, Batched VBO Data: %.2f KB\n";
			System::Sprintf(text, textArraySize, msg,
				(mBatchVBOsSize/mBatchVBOCount)/kb, mBatchVBOCount,
				mBatchedVBOData / kb);

			pText->Append(text);

		}
	}

 	PdrRendererData* pRendererData = mpRenderer->GetRendererData();
 	pRendererData->AppendStatistics(pText);
}

//----------------------------------------------------------------------------
void RendererStatistics::AppendToText(Text* pText, Float fps,
	Bool useAverageFps)
{
	const UInt textArraySize = 32;
	Char text[textArraySize];
	fps = useAverageFps ? AverageFps(fps) : fps;
	Int roundedFps = static_cast<Int>(MathF::Round(fps)); 

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
Float RendererStatistics::AverageFps(Float currentFps)
{
	WIRE_ASSERT(mFpsSamplesIndex < FPS_SAMPLE_QUANTITY);

	mFpsSamples[mFpsSamplesIndex] = currentFps;
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

	return averageFps /= FPS_SAMPLE_QUANTITY;
}
