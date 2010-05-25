#pragma once
#ifndef WIREPDRRENDERERDATA_H
#define WIREPDRRENDERERDATA_H

namespace Wire
{

class PdrRendererData
{
public:
	PdrRendererData() {}
	~PdrRendererData() {}

	// Platform-dependent data
	void* mFifoBuffer;
	void* mFrameBuffer[2];

	VBufferID* mpVBufferID;
	IBufferID* mpIBufferID;

	GXRenderModeObj* mRMode;
	GXColor mGXClearColor;

	// Transforms world space to camera space.
	Matrix34F mViewMatrix;

	UInt mFrameBufferIndex;
	Bool mIsFrameBufferDirty;
};

}

#endif
