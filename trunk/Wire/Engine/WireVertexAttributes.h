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

	UInt GetChannelQuantity() const { return mChannelQuantity; }

	void SetPositionChannels(UInt positionChannels);
	void SetColorChannels(UInt colorChannels);
	void SetNormalChannels(UInt normalChannels);
	void SetTCoordChannels(UInt tCoordChannels);

	UInt GetPositionChannels() const { return mPositionChannels; }
	UInt GetColorChannels() const { return mColorChannels; }
	UInt GetNormalChannels() const { return mNormalChannels; }
	UInt GetTCoordChannels() const { return mTCoordChannels; }

	UInt GetPositionOffset() const { return mPositionOffset; }
	UInt GetColorOffset() const { return mColorOffset; }
	UInt GetNormalOffset() const { return mNormalOffset; }
	UInt GetTCoordOffset() const { return mTCoordOffset; }

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
