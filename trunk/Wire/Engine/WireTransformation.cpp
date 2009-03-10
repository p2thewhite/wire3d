#include "WireTransformation.h"

using namespace Wire;

//-------------------------------------------------------------------------
Transformation::Transformation()
	: mScale(Vector3f::ONE)
{
	mTransformation->MakeIdentity();
}

//-------------------------------------------------------------------------
Transformation::~Transformation()
{

}
