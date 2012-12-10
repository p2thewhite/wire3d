#include "gtest/gtest.h"

#include "../Stubs/WireInputSystemStub.h"
#include "../Stubs/WireMainInputDeviceStub.h"
#include "../Stubs/WireInputDeviceExtensionStub.h"

namespace Wire
{

class InputSystemTest : public ::testing::Test
{
protected:
	InputSystemStub* mpInputSystem;
	MainInputDevicePtr mpMainInputDevice;
	InputDeviceExtensionPtr mpInputDeviceExtension;
	TArray<MainInputDevicePtr> mInitialInputDevices;
	TArray<InputCapabilityPtr> mInitialInputCapabilities;

	InputSystemTest()
	{
	}

	virtual ~InputSystemTest()
	{
	}

	virtual void SetUp()
	{
		mpMainInputDevice = new MainInputDeviceStub();
		mpInputDeviceExtension = new InputDeviceExtensionStub(mpMainInputDevice);
		mpMainInputDevice->AddExtension(mpInputDeviceExtension);

		mInitialInputDevices.Append(mpMainInputDevice);

		mpInputSystem = new InputSystemStub(mInitialInputDevices);
		mpInputSystem->DiscoverDevices();
	}

	virtual void TearDown()
	{
		delete mpInputSystem;
	}

};

TEST_F(InputSystemTest, should_list_all_device_extensions)
{
	ASSERT_EQ(1, mpInputSystem->GetDeviceExtensionsCount());
	ASSERT_EQ(mpInputDeviceExtension, const_cast<InputDeviceExtension*>(mpInputSystem->GetDeviceExtension(0)));
}

TEST_F(InputSystemTest, should_get_device_extension_by_index)
{
	ASSERT_EQ(mpInputDeviceExtension, const_cast<InputDeviceExtension*>(mpInputSystem->GetDeviceExtension(0)));
}

TEST_F(InputSystemTest, should_list_all_main_input_devices)
{
	ASSERT_EQ(1, mpInputSystem->GetMainDevicesCount());
	ASSERT_EQ(mpMainInputDevice, const_cast<MainInputDevice*>(mpInputSystem->GetMainDevice(0)));
}

TEST_F(InputSystemTest, should_get_main_input_device_by_index)
{
	ASSERT_EQ(mpMainInputDevice, const_cast<MainInputDevice*>(mpInputSystem->GetMainDevice(0)));
}

TEST_F(InputSystemTest, should_list_all_input_devices)
{
	ASSERT_EQ(2, mpInputSystem->GetDevicesCount());
	ASSERT_EQ(mpMainInputDevice, const_cast<InputDevice*>(mpInputSystem->GetDevice(0)));
	ASSERT_EQ(mpInputDeviceExtension, const_cast<InputDevice*>(mpInputSystem->GetDevice(1)));
}

TEST_F(InputSystemTest, should_get_input_device_by_index)
{
	ASSERT_EQ(mpMainInputDevice, const_cast<InputDevice*>(mpInputSystem->GetDevice(0)));
}

TEST_F(InputSystemTest, should_add_devices_to_correct_collection)
{
	InputSystemStub* pInputSystem = new InputSystemStub();
	pInputSystem->AddDevice_TEST(mpMainInputDevice);
	ASSERT_EQ(1, pInputSystem->GetMainDevicesCount());
	ASSERT_EQ(1, pInputSystem->GetDeviceExtensionsCount());

	delete pInputSystem;
}

TEST_F(InputSystemTest, should_remove_devices_from_correct_collection)
{
	mpInputSystem->RemoveDevice_TEST(mpMainInputDevice);
	ASSERT_EQ(0, mpInputSystem->GetMainDevicesCount());
	ASSERT_EQ(0, mpInputSystem->GetDeviceExtensionsCount());
}

}
