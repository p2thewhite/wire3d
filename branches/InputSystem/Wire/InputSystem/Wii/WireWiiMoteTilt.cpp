#include "WireWiiMoteTilt.h"

WIRE_IMPLEMENT_RTTI(Wire, WiiMoteTilt, Tilt);

namespace Wire
{
	
WiiMoteTilt::WiiMoteTilt()
{
}

WiiMoteTilt::~WiiMoteTilt()
{
}

Float WiiMoteTilt::GetBackward() const
{
	return 0;
}

Float WiiMoteTilt::GetForward() const
{
	return 0;
}

Float WiiMoteTilt::GetLeft() const
{
	return 0;
}

Float WiiMoteTilt::GetRight() const
{
	return 0;
}

}