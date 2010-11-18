#pragma once
#ifndef WIRECOLORRGB_H
#define WIRECOLORRGB_H

#include "WireTypes.h"

namespace Wire
{

class /*WIRE_FOUNDATION_ITEM*/ ColorRGB
{
public:
	ColorRGB();  // initial values (0, 0, 0)
	ColorRGB(Float red, Float green, Float blue);

	inline Float R() const;
	inline Float& R();
	inline Float G() const;
	inline Float& G();
	inline Float B() const;
	inline Float& B();

	// arithmetic operations
	ColorRGB operator* (Float scalar) const;
	friend ColorRGB operator* (Float scalar, const ColorRGB& rCol);

	// arithmetic updates
	ColorRGB& operator*= (Float scalar);

	static const ColorRGB BLACK;
	static const ColorRGB WHITE;
	static const ColorRGB RED;
	static const ColorRGB GREEN;
	static const ColorRGB BLUE;

private:
	Float mTuple[3];
};

#include "WireColorRGB.inl"

}

#endif
