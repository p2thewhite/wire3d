#include "WireWin32EmulatedWiiControllersDiscovery.h"
#include "WireWin32EmulatedWiiMote.h"
#include "WireWin32EmulatedNunchuk.h"

namespace Wire
{

Win32EmulatedWiiControllersDiscovery::Win32EmulatedWiiControllersDiscovery()
{
}

Win32EmulatedWiiControllersDiscovery::~Win32EmulatedWiiControllersDiscovery()
{
}

void Win32EmulatedWiiControllersDiscovery::Discover()
{
	// FIXME: since there's no way to know how many keyboards and mouses are connected using Win32, assume there'll be always 1 emulated controller ever!
	if (GetMainInputDevices().size() == 1)
	{
		return;
	}

	Win32EmulatedWiiMote* pEmulatedWiiMote = WIRE_NEW Win32EmulatedWiiMote();
	AddInputDevice(pEmulatedWiiMote);
	Win32EmulatedNunchuk* pEmulatedNunchuk = WIRE_NEW Win32EmulatedNunchuk(pEmulatedWiiMote);
	AddInputDevice(pEmulatedNunchuk);

	pEmulatedWiiMote->SetUp();
	pEmulatedNunchuk->SetUp();

	NotifyListeners();
}

}