#pragma once
#ifndef WIRECOLORRGBA_H
#define WIRECOLORRGBA_H

#include "WireTypes.h"

namespace Wire
{

class ColorRGBA
{
public:
	ColorRGBA();  // initial values (0, 0, 0, 0)
	ColorRGBA(Float red, Float green, Float blue, Float alpha);

	inline Float R() const;
	inline Float& R();
	inline Float G() const;
	inline Float& G();
	inline Float B() const;
	inline Float& B();
	inline Float A() const;
	inline Float& A();

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
