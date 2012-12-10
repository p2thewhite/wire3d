#include "gtest/gtest.h"

#include "../Stubs/WireMainInputDeviceStub.h"
#include "../Stubs/WireInputDeviceExtensionStub.h"

namespace Wire
{

class InputDeviceExtensionTest : public ::testing::Test
{
protected:
	MainInputDeviceStub mMainInputDevice;
	InputDeviceExtensionStub mInputDeviceExtension;

	InputDeviceExtensionTest()
		: mInputDeviceExtension(&mMainInputDevice)
	{
	}

	virtual ~InputDeviceExtensionTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(InputDeviceExtensionTest, should_know_main_input_device_which_is_connected_to)
{
	ASSERT_EQ(&mMainInputDevice, mInputDeviceExtension.GetMainInputDevice_TEST());
}

}
