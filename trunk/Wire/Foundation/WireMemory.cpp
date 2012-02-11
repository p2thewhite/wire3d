// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#ifdef WIRE_DEBUG

#include "WireMemory.h"
#include "WireString.h"
#include "WireSystem.h"
#include <cstdlib>

//----------------------------------------------------------------------------
void* Wire::Memory::Allocate(size_t size, Char* pFile, UInt line, Bool isArray)
{
	void* pAddr = malloc(size);

	if (!pAddr)
	{
		if (isArray)
		{
			System::Assert("Could not allocate array.", pFile, line);
		}
		else
		{
			System::Assert("Could not allocate memory.", pFile, line);
		}
	}

	return pAddr;
}

//----------------------------------------------------------------------------
void Wire::Memory::Deallocate(void* pAddr, Bool)
{
	if (!pAddr)
	{
		return;
	}

	free(pAddr);
}

#ifdef WIRE_WII
//----------------------------------------------------------------------------
void* operator new (size_t size) throw (std::bad_alloc)
{
	return Wire::Memory::Allocate(size, 0, 0, false);
}

//----------------------------------------------------------------------------
void* operator new[](size_t size) throw (std::bad_alloc)
{
	return Wire::Memory::Allocate(size, 0, 0, true);
}

//----------------------------------------------------------------------------
void* operator new (size_t size, Char* pFile, UInt line) throw (std::bad_alloc)
{
	return Wire::Memory::Allocate(size, pFile, line, false);
}

//----------------------------------------------------------------------------
void* operator new[] (size_t size, Char* pFile, UInt line) throw (std::bad_alloc)
{
	return Wire::Memory::Allocate(size, pFile, line, true);
}

//----------------------------------------------------------------------------
void operator delete (void* pAddr) throw ()
{
	Wire::Memory::Deallocate(pAddr, false);
}

//----------------------------------------------------------------------------
void operator delete[] (void* pAddr) throw ()
{
	Wire::Memory::Deallocate(pAddr, true);
}
//----------------------------------------------------------------------------
void operator delete (void* pAddr, Char*, UInt) throw ()
{
	Wire::Memory::Deallocate(pAddr, false);
}
//----------------------------------------------------------------------------
void operator delete[] (void* pAddr, Char*, UInt) throw ()
{
	Wire::Memory::Deallocate(pAddr, true);
}
#else
//----------------------------------------------------------------------------
void* operator new (size_t size)
{
	return Wire::Memory::Allocate(size, 0, 0, false);
}

//----------------------------------------------------------------------------
void* operator new[](size_t size)
{
	return Wire::Memory::Allocate(size, 0, 0, true);
}

//----------------------------------------------------------------------------
void* operator new (size_t size, Char* pFile, UInt line)
{
	return Wire::Memory::Allocate(size, pFile, line, false);
}

//----------------------------------------------------------------------------
void* operator new[] (size_t size, Char* pFile, UInt line)
{
	return Wire::Memory::Allocate(size, pFile, line, true);
}

//----------------------------------------------------------------------------
void operator delete (void* pAddr)
{
	Wire::Memory::Deallocate(pAddr, false);
}

//----------------------------------------------------------------------------
void operator delete[] (void* pAddr)
{
	Wire::Memory::Deallocate(pAddr, true);
}
//----------------------------------------------------------------------------
void operator delete (void* pAddr, Char*, UInt)
{
	Wire::Memory::Deallocate(pAddr, false);
}
//----------------------------------------------------------------------------
void operator delete[] (void* pAddr, Char*, UInt)
{
	Wire::Memory::Deallocate(pAddr, true);
}
#endif

#endif
