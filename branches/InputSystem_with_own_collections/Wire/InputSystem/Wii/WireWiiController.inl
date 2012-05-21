inline void WiiController::SetWiiMote(WiiMote* pWiiMote)
{
	AddInputDevice(pWiiMote, WIIMOTE_INDEX);
}

inline void WiiController::SetWiiExtension(WiiExtension* pWiiExtension)
{
	AddInputDevice(pWiiExtension, WIIEXTENSION_INDEX);
}

inline WiiMote* WiiController::GetWiiMote()
{
	InputDevice* pInputDevice = GetInputDevice(WIIMOTE_INDEX);

	if (pInputDevice == NULL)
	{
		return NULL;
	}
	else
	{
		return static_cast<WiiMote*>(pInputDevice);
	}
}

inline WiiExtension* WiiController::GetWiiExtension()
{
	InputDevice* pInputDevice = GetInputDevice(WIIEXTENSION_INDEX);

	if (pInputDevice == NULL)
	{
		return NULL;
	}
	else
	{
		return static_cast<WiiExtension*>(pInputDevice);
	}
}

inline Bool WiiController::HasWiiExtension()
{
	return (GetWiiExtension() != NULL);
}