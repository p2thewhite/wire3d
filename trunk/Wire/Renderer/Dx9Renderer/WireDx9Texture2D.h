#pragma once
#ifndef WIREDX9TEXTURE2D_H
#define WIREDX9TEXTURE2D_H

#include "WireBuffer.h"
#include "WireTypes.h"

struct IDirect3DTexture9;

namespace Wire
{

class Renderer;
class Texture2D;

class PdrTexture2D
{
public:
	PdrTexture2D(Renderer* pRenderer, const Texture2D* pTexture);
	~PdrTexture2D();

	void Enable(Renderer* pRenderer, const Texture2D* pTexture, UInt unit);
	void Disable(Renderer* pRenderer, UInt unit);
	void* Lock(Buffer::LockingMode mode, UInt level = 0);
	void Unlock(UInt level = 0);

private:
	IDirect3DTexture9* mpTexture;
};

}

#endif
