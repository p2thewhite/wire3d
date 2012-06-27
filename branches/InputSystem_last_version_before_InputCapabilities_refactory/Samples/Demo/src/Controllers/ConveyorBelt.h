#pragma once
#ifndef CONVEYORBELT_H
#define CONVEYORBELT_H

#include "WireController.h"
#include "WireGeometry.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"

using namespace Wire;

class ConveyorBelt : public Controller
{
public:
	ConveyorBelt(Geometry* pGeometry, Renderer* pRenderer);

	virtual Bool Update(Double appTime);
	virtual Bool RenderUpdate();

private:
	VertexBufferPtr mspVertexBufferCopy;
	Renderer* mpRenderer;

	Float mOffset;
};

#endif
