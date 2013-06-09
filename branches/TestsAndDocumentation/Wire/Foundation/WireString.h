// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESTRING_H
#define WIRESTRING_H

#include "WireSystem.h"

namespace Wire
{

class String
{
public:
	// construction and destruction
	String(const Char* pText = NULL);
	String(UInt length, const Char* pText);
	String(const String& rString);
	~String();

    // member access
	inline UInt GetLength() const;
	inline operator const Char* () const;

    // assignment, comparisons
	String& operator= (const String& rString);
	String& operator+= (const String& rString);
	String operator+ (const String& rString) const;
	Bool operator== (const String& rString) const;
	Bool operator!= (const String& rString) const;

	// implicit conversion (support for hash tables)
	operator UInt () const;

	bool StartsWith(const String& rString) const;

    // case-related
	String ToUpper() const;
	String ToLower() const;

private:
	UInt VerifyLength(UInt length, const Char* pText) const;
	
	UInt mLength;
	Char* mpText;
	static const Char* s_pEmpty;
};

#include "WireString.inl"

}

#endif