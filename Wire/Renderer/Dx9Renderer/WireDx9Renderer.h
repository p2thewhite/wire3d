#pragma once
#ifndef WIREDX9RENDERER_H
#define WIREDX9RENDERER_H

#include "WireRenderer.h"

namespace Wire
{

class PdrRendererData;
class PdrRendererInput;

class /*WIRE_RENDERER_ITEM*/ Dx9Renderer : public Renderer
{

public:
	Dx9Renderer(PdrRendererInput& rInput);
	virtual ~Dx9Renderer();

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
	virtual void OnDisableIBuffer(ResourceIdentifier*);

	virtual void OnEnableVBuffer(ResourceIdentifier* pID);
	virtual void OnDisableVBuffer(ResourceIdentifier*);

	virtual void OnEnableTexture(ResourceIdentifier* pID);
	virtual void OnDisableTexture(ResourceIdentifier* pID);
};

}

#endif
