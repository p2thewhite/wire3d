#include "WireBaseVirtualInputDevice.h"
#include <vector>
#include <string>

using namespace std;

namespace Wire
{

BaseVirtualInputDevice::BaseVirtualInputDevice()
{
}

BaseVirtualInputDevice::~BaseVirtualInputDevice()
{
}

const char* BaseVirtualInputDevice::GetName() const
{
	string name("[");

	//if (!mInputDevices.IsEmpty())
	if (mInputDevices.size() > 0)
	{
		//for (UInt i = 0; i < mInputDevices.GetQuantity(); i++)
		for (UInt i = 0; i < mInputDevices.size(); i++)
		{
			InputDevice* pInputDevice = mInputDevices[i];
			name += pInputDevice->GetName();
			name += ", ";
		}

		name = name.substr(0, name.length() - 2);
	}

	name += "]";

	return name.c_str();
}

Bool BaseVirtualInputDevice::GetButton(Button button) const
{
	//for (UInt i = 0; i < mInputDevices.GetQuantity(); i++)
	for (UInt i = 0; i < mInputDevices.size(); i++)
	{
		InputDevice* pInputDevice = mInputDevices[i];
		if (pInputDevice->HasCapability(BUTTONS))
		{
			if (pInputDevice->GetButton(button))
			{
				return true;
			}
		}
	}

	return false;
}

Float BaseVirtualInputDevice::GetIRAxis(IRAxis axis) const
{
	//for (UInt i = 0; i < mInputDevices.GetQuantity(); i++)
	for (UInt i = 0; i < mInputDevices.size(); i++)
	{
		InputDevice* pInputDevice = mInputDevices[i];
		if (pInputDevice->HasCapability(IR_AXIS))
		{
			Float axisValue = pInputDevice->GetIRAxis(axis);
			if (axisValue != 0)
			{
				return axisValue;
			}
		}
	}

	return 0;
}

Float BaseVirtualInputDevice::GetDigitalAxis(DigitalAxis axis) const
{
	//for (UInt i = 0; i < mInputDevices.GetQuantity(); i++)
	for (UInt i = 0; i < mInputDevices.size(); i++)
	{
		InputDevice* pInputDevice = mInputDevices[i];
		if (pInputDevice->HasCapability(DIGITAL_AXIS))
		{
			Float axisValue = pInputDevice->GetDigitalAxis(axis);
			if (axisValue != 0)
			{
				return axisValue;
			}
		}
	}

	return 0;
}

Float BaseVirtualInputDevice::GetAnalogAxis(AnalogAxis axis) const
{
	//for (UInt i = 0; i < mInputDevices.GetQuantity(); i++)
	for (UInt i = 0; i < mInputDevices.size(); i++)
	{
		InputDevice* pInputDevice = mInputDevices[i];
		if (pInputDevice->HasCapability(ANALOG_AXIS))
		{
			Float axisValue = pInputDevice->GetAnalogAxis(axis);
			if (axisValue != 0)
			{
				return axisValue;
			}
		}
	}

	return 0;
}

void BaseVirtualInputDevice::AddInputDevice(InputDevice* pInputDevice)
{
	//mInputDevices.Append(pInputDevice);
	mInputDevices.push_back(pInputDevice);

	UpdateCapabilities();
}

void BaseVirtualInputDevice::AddInputDevice(InputDevice* pInputDevice, UInt index)
{
	//mInputDevices.Insert(index, pInputDevice);
	mInputDevices.insert(mInputDevices.begin() + index, pInputDevice);

	UpdateCapabilities();
}

void BaseVirtualInputDevice::UpdateCapabilities()
{
	//mCapabilities.RemoveAll();
	mCapabilities.clear();

	//for (UInt i = 0; i < mInputDevices.GetQuantity(); i++)
	for (UInt i = 0; i < mInputDevices.size(); i++)
	{
		InputDevice* pInputDevice = mInputDevices[i];
		/*THashSet<InputCapability>::Iterator iterator(&pInputDevice->GetCapabilities());

		InputCapability* pCapability;
		while ((pCapability = iterator.GetNext()) != NULL)
		{
			mCapabilities.Insert(*pCapability);
		}*/

		const set<InputCapability> &mInputDeviceCapabilities = pInputDevice->GetCapabilities();
		set<InputCapability>::const_iterator iterator =	mInputDeviceCapabilities.begin();
		while (iterator != mInputDeviceCapabilities.end())
		{
			InputCapability capability = (*iterator);
			mCapabilities.insert(capability);
			iterator++;
		}
	}
}

}
