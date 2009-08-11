#pragma once
#ifndef WIRECOLORRGB_H
#define WIRECOLORRGB_H

#include "WireTypes.h"

namespace Wire
{

class ColorRGB
{
public:
	ColorRGB();  // initial values (0, 0, 0)
	ColorRGB(Float red, Float green, Float blue);

	Float R() const;
	Float& R();
	Float G() const;
	Float& G();
	Float B() const;
	Float& B();

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
