#pragma once
#ifndef WIREGXRENDERER_H
#define WIREGXRENDERER_H

#include "WireRenderer.h"

namespace Wire
{

class PdrRendererData;
class PdrRendererInput;

class /*WIRE_RENDERER_ITEM*/ GXRenderer : public Renderer
{

public:
	GXRenderer(PdrRendererInput& rInput);
	virtual ~GXRenderer();

protected:
	// Resource loading and releasing (to/from video memory).
	virtual void OnLoadIBuffer(ResourceIdentifier*& rID,
		IndexBuffer* pBuffer);
	virtual void OnReleaseIBuffer(ResourceIdentifier* pID);

	virtual void OnLoadVBuffer(ResourceIdentifier*& rID,
		VertexBuffer* pVBuffer);
	virtual void OnReleaseVBuffer(ResourceIdentifier* pID);

	virtual void OnLoadTexture(ResourceIdentifier*& rID, Texture* pTexture);
	virtual void OnReleaseTexture(ResourceIdentifier* pID);

	// Resource enabling and disabling.
	virtual void OnEnableIBuffer(ResourceIdentifier* pID);
	virtual void OnDisableIBuffer(ResourceIdentifier* pID);

	virtual void OnEnableVBuffer(ResourceIdentifier* pID);
	virtual void OnDisableVBuffer(ResourceIdentifier* pID);

	virtual void OnEnableTexture(ResourceIdentifier* pID);
	virtual void OnDisableTexture(ResourceIdentifier* pID);
};

}

#endif
