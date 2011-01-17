#pragma once
#ifndef WIREVERTEXATTRIBUTES_H
#define WIREVERTEXATTRIBUTES_H

#include "WireTArray.h"

namespace Wire
{

class VertexAttributes
{

public:
	VertexAttributes();
	virtual ~VertexAttributes();

	inline UInt GetChannelQuantity() const;
	inline UInt GetColorChannelQuantity() const;
	inline UInt GetTCoordChannelQuantity() const;

	inline void SetInterleaved(Bool isChannelInterleaved);
	void SetPositionChannels(UInt positionChannels);
	void SetColorChannels(UInt colorChannels, UInt unit = 0);
	void SetNormalChannels(UInt normalChannels);
	void SetTCoordChannels(UInt tCoordChannels, UInt unit = 0);

	inline Bool HasPosition() const;
	inline Bool HasColor(UInt unit = 0) const;
	inline Bool HasNormal() const;
	inline Bool HasTCoord(UInt unit = 0) const;

	inline UInt GetPositionChannels() const;
	inline UInt GetColorChannels(UInt unit = 0) const;
	inline UInt GetNormalChannels() const;
	inline UInt GetTCoordChannels(UInt unit = 0) const;

	inline UInt GetPositionOffset() const;
	inline UInt GetColorOffset(UInt unit = 0) const;
	inline UInt GetNormalOffset() const;
	inline UInt GetTCoordOffset(UInt unit = 0) const;

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
