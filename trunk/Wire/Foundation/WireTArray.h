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
	TArray(Int quantity = 0, Int growBy = 1);
	TArray(const TArray& rObject);
	virtual ~TArray();

	// assignment
	TArray& operator= (const TArray& rObject);

	// element access, index i must be in range
	Int GetQuantity() const;
	T* GetArray();
	const T* GetArray() const;
	T& operator[] (Int i);
	const T& operator[] (Int i) const;

	// add new element, array will dynamically grow if necessary
	void Append(const T& rElement);
	void SetElement(Int i, const T& rElement);

	// Remove the element at the specified index. The elements occurring
	// after that one are shifted so that the array remains contiguous. After
	// the shift, but before the decrement on quantity, array[quantity-1] is
	// a duplicate of array[quantity-2]. The element at index quantity-1 is
	// reconstructed using the default constructor for class T. Then the
	// quantity is decremented.
	void Remove(Int i);

	// all elements are set to the default object of class T
	void RemoveAll();

	// dynamic growth, new array elements are default constructed
	void SetMaxQuantity(Int newMaxQuantity, Bool copy = true);
	Int GetMaxQuantity() const;
	void SetGrowBy(Int growBy);
	Int GetGrowBy() const;

private:
	Int mQuantity;
	Int mMaxQuantity;
	Int mGrowBy;
	T* mpArray;
};

#include "WireTArray.inl"

}

#endif
