#pragma once
#ifndef DEMOINIT_H
#define DEMOINIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#include "../../../Wire/Foundation/WireFoundation.h"
#include "../../../Wire/Engine/WireEngine.h"

#define DEFAULT_FIFO_SIZE	(256*1024)

class DemoInit
{
public:
	DemoInit();
	virtual ~DemoInit();

	void Init();
	void StartVI();
	void InitGX();
	void ConfigureMem();
	void BeforeRender();
	void DoneRender();
	void SwapBuffers();
	GXRenderModeObj* GetRenderMode() { return mRmode; }

private:
	void* mFrameBuffer[2];
	void* mDemoFifoBuffer;
	unsigned int mFrameBufferIndex;
	GXRenderModeObj* mRmode;
};

#endif /* DEMOINIT_H */
