#include "WireVertexBuffer.h"

#include "../Foundation/WireFoundation.h"

using namespace Wire;

//-------------------------------------------------------------------------
VertexBuffer::VertexBuffer(unsigned int vertexQuantity)
	:
	mVertexSize(0),
	mVertexQuantity(vertexQuantity),
	mpChannel(NULL)
{
	mVertexSize = sizeof(Vector3f);
}

//-------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{

}

//-------------------------------------------------------------------------
Vector3f& VertexBuffer::Position3(int i)
{
	Float* pmChannel = &mChannel[0];
	pmChannel += mVertexSize/sizeof(Float) * i;

	return *(reinterpret_cast<Vector3f*>(pmChannel));
}

//-------------------------------------------------------------------------
Vector3f VertexBuffer::Position3(int i) const
{
	const Float* pmChannel = &mChannel[0];
	pmChannel += mVertexSize/sizeof(Float) * i;

	return *(reinterpret_cast<Vector3f*>(const_cast<Float*>(pmChannel)));
}
