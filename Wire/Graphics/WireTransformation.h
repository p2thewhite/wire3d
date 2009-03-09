#pragma once
#ifndef WIRETRANSFORMATION_H
#define WIRETRANSFORMATION_H


#include "WireMatrix34.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ Transformation
{

private:
	Matrix34f mTransformation;
	Vector3f mScale;
};

#include "WireTransformation.inl"

}

#endif /* WIRETRANSFORMATION_H */
