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

	Float R() const;
	Float& R();
	Float G() const;
	Float& G();
	Float B() const;
	Float& B();
	Float A() const;
	Float& A();

	static const ColorRGBA BLACK;
	static const ColorRGBA WHITE;

private:
	Float mTuple[4];
};

#include "WireColorRGBA.inl"

}

#endif /* WIRECOLORRGBA_H */
