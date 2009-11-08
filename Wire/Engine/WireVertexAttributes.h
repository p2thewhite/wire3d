#pragma once
#ifndef WIREVERTEXATTRIBUTES_H
#define WIREVERTEXATTRIBUTES_H

#include "../Foundation/WireSystem.h"
#include "../Foundation/WireTArray.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ VertexAttributes
{

public:
	VertexAttributes();
	virtual ~VertexAttributes();

	UInt GetChannelQuantity() const;
	UInt GetColorChannelQuantity() const;

	void SetInterleaved(Bool isChannelInterleaved);
	void SetPositionChannels(UInt positionChannels);
	void SetColorChannels(UInt colorChannels, UInt unit = 0);
	void SetNormalChannels(UInt normalChannels);
	void SetTCoordChannels(UInt tCoordChannels, UInt unit = 0);

	UInt GetPositionChannels() const;
	UInt GetColorChannels(UInt unit = 0) const;
	UInt GetNormalChannels() const;
	UInt GetTCoordChannels(UInt unit = 0) const;

	UInt GetPositionOffset() const;
	UInt GetColorOffset(UInt unit = 0) const;
	UInt GetNormalOffset() const;
	UInt GetTCoordOffset(UInt unit = 0) const;

private:
	void UpdateOffsets();
	void ResetOffsets();

	UInt mChannelQuantity;

	Int mPositionChannels;
	Int mPositionOffset;

	TArray<Int> mColorChannels;
	TArray<Int> mColorOffset;

	Int mNormalChannels;
	Int mNormalOffset;

	TArray<Int> mTCoordChannels;
	TArray<Int> mTCoordOffset;

	Bool mIsChannelInterleaved;
};

#include "WireVertexAttributes.inl"

}

#endif
