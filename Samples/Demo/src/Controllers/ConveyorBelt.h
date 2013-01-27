#pragma once
#ifndef CONVEYORBELT_H
#define CONVEYORBELT_H

#include "WireController.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"

using namespace Wire;

class ConveyorBelt : public Controller
{
public:
	ConveyorBelt(RenderObject* pRenderObject, Renderer* pRenderer);

	virtual Bool Update(Double appTime);
	virtual Bool OnGetVisibleUpdate(const Camera* pCamera);

private:
	VertexBufferPtr mspVertexBufferCopy;
	Renderer* mpRenderer;

	Float mOffset;
};

#endif
