#pragma once
#ifndef CURSORS_H
#define CURSORS_H

#include "WireGeometry.h"
#include "WireMaterial.h"
#include "WireNode.h"
#include "WireStateAlpha.h"

using namespace Wire;

class Cursors
{
public:
	enum CursorMode
	{
		CM_POINTING,
		CM_PRIMARY_BUTTON_PRESSED,
		CM_SECONDARY_BUTTON_PRESSED,
		CM_OFF
	};

	Cursors();

	void SetCursor(Float x, Float y, CursorMode mode = CM_POINTING,
		UInt playerNo = 0, Float zRollInRadian = 0);
	Node* GetRoot();

private:
	void InitCursors();
	Geometry* CreateCursor(Float uOffset, Float vOffset);

	NodePtr mspCursors;
	MaterialPtr mspMaterial;

	static const unsigned char s_Png[];
	static const int s_PngSize;
};

#endif
