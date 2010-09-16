#pragma once
#ifndef WIRECOLORRGBA_H
#define WIRECOLORRGBA_H

#include "WireTypes.h"

namespace Wire
{

class /*WIRE_FOUNDATION_ITEM*/ ColorRGBA
{
public:
	ColorRGBA();  // initial values (0, 0, 0, 0)
	ColorRGBA(Float red, Float green, Float blue, Float alpha);

	Float R() const;
	Float& R();
	Float G() const;
	Float& G();
	Float B() const;
	Float& B();
	Float A() const;
	Float& A();

	// arithmetic operations
	ColorRGBA operator* (Float scalar) const;
	friend ColorRGBA operator* (Float scalar, const ColorRGBA& rCol);

	// arithmetic updates
	ColorRGBA& operator*= (Float scalar);

	static const ColorRGBA BLACK;
	static const ColorRGBA WHITE;
	static const ColorRGBA RED;
	static const ColorRGBA GREEN;
	static const ColorRGBA BLUE;

private:
	Float mTuple[4];
};

#include "WireColorRGBA.inl"

}

#endif
