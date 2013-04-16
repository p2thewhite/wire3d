// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREGXSHADER_H
#define WIREGXSHADER_H

namespace Wire
{

class Renderer;
class Shader;

class PdrShader
{

public:
	PdrShader(Renderer* pRenderer, const Shader* pShader) {}
	~PdrShader() {}

	void Enable(Renderer* pRenderer) {}
	void Disable(Renderer* pRenderer) {}
	UInt GetBufferSize() { return 0; }
};

}

#endif
