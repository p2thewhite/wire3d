#include "gtest/gtest.h"

namespace Wire
{

class InputDeviceExtensionTest : public ::testing::Test
{
protected:
	InputDeviceExtensionTest()
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
	FAIL();
}

}
