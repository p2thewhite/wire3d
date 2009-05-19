#pragma once
#ifndef WIREVERTEXATTRIBUTES_H
#define WIREVERTEXATTRIBUTES_H

#include "../Foundation/WireSystem.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ VertexAttributes
{

public:
	VertexAttributes();
	virtual ~VertexAttributes();

	UInt GetChannelQuantity();

	void SetPositionChannels(UInt positionChannels);
	void SetColorChannels(UInt colorChannels);
	void SetNormalChannels(UInt normalChannels);
	void SetTCoordChannels(UInt tCoordChannels);

	UInt GetPositionChannels() { return mPositionChannels; }
	UInt GetColorChannels() { return mColorChannels; }
	UInt GetNormalChannels() { return mNormalChannels; }
	UInt GetTCoordChannels() { return mTCoordChannels; }

private:
	void UpdateOffsets();
	void ResetOffsets();

	UInt mChannelQuantity;

	Int mPositionChannels;
	Int mPositionOffset;

	Int mColorChannels;
	Int mColorOffset;

	Int mNormalChannels;
	Int mNormalOffset;

	Int mTCoordChannels;
	Int mTCoordOffset;
};

}

#endif /* WIREVERTEXATTRIBUTES_H */
