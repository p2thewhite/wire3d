#pragma once
#ifndef WIREGXTEXTURE2D_H
#define WIREGXTEXTURE2D_H

#include "WireSystem.h"

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

private:
	void* mpImage;
	GXTexObj mTexObj;
};

}

#endif
