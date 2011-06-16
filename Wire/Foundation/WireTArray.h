// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRETARRAY_H
#define WIRETARRAY_H

#include "WireSystem.h"

namespace Wire
{

template <class T>
class TArray
{
public:
	// construction and destruction
	TArray(UInt quantity = 0, UInt growBy = 1);
	TArray(const TArray& rObject);
	~TArray();

	// assignment
	TArray& operator= (const TArray& rObject);

	// element access, index i must be in range
	inline UInt GetQuantity() const;
	inline T* GetArray();
	inline const T* GetArray() const;
	inline T& operator[] (UInt i);
	inline const T& operator[] (UInt i) const;

	// add new element, array will dynamically grow if necessary
	void Append(const T& rElement);
	void Insert(UInt i, const T& rElement);
	void SetElement(UInt i, const T& rElement);

	// Remove the element or at the specified index. The elements occurring
	// after that one are shifted so that the array remains contiguous. After
	// the shift, but before the decrement on quantity, array[quantity-1] is
	// a duplicate of array[quantity-2]. The element at index quantity-1 is
	// reconstructed using the default constructor for class T. Then the
	// quantity is decremented.

	// Remove the element. Return value indicates if the operation succeeded.
	Bool Remove(const T& rElement);

	// Remove the element at the specified index. 
	void RemoveAt(UInt i);

	// Remove the last element of the array.
	void RemoveLast();

	// All elements are set to the default object of class T.
	void RemoveAll();

	// dynamic growth, new array elements are default constructed
	void SetMaxQuantity(UInt newMaxQuantity, Bool copy = true);
	inline UInt GetMaxQuantity() const;
	inline void SetGrowBy(UInt growBy);
	inline UInt GetGrowBy() const;

	// set the quantity without con-/destructing array elements
	void SetQuantity(UInt newQuantity, Bool copy = true);

private:
	UInt mQuantity;
	UInt mMaxQuantity;
	UInt mGrowBy;
	T* mpArray;
};

#include "WireTArray.inl"

}

#endif
