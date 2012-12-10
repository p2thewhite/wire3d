#include "gtest/gtest.h"

#include "../Stubs/WireInputDeviceStub.h"
#include "../Stubs/WireInputCapabilityStub.h"

namespace Wire
{

class InputCapabilityTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	InputCapabilityStub mInputCapability;

	InputCapabilityTest()
		: mInputCapability(&mInputDevice)
	{
	}

	virtual ~InputCapabilityTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(InputCapabilityTest, should_know_parent_device)
{
	ASSERT_EQ(&mInputDevice, mInputCapability.GetParent_TEST());
}

}
