#include "gtest/gtest.h"

#include "../Stubs/WireInputDeviceStub.h"
#include "../Stubs/WireInputCapabilityStub.h"
#include "../Stubs/WireInputDataBufferStub.h"
#include "WireButtons.h"

namespace Wire
{

class InputDeviceTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;

	InputDeviceTest()
	{
	}

	virtual ~InputDeviceTest()
	{
	}

	virtual void SetUp()
	{
		mInputDevice.SetUp();
	}

	virtual void TearDown()
	{
	}

};

TEST_F(InputDeviceTest, should_list_all_capabilities)
{
	ASSERT_EQ(1, mInputDevice.GetCapabilities().GetQuantity());
}

TEST_F(InputDeviceTest, should_answer_if_has_capability)
{
	ASSERT_TRUE(mInputDevice.HasCapability(InputCapabilityStub::TYPE));
	ASSERT_FALSE(mInputDevice.HasCapability(Buttons::TYPE));
}

TEST_F(InputDeviceTest, should_get_and_set_input_data_buffer)
{
	InputDataBufferStub* pInputDataBuffer = new InputDataBufferStub();

	mInputDevice.SetInputDataBuffer(pInputDataBuffer);
	ASSERT_EQ(pInputDataBuffer, mInputDevice.GetInputDataBuffer());

	delete pInputDataBuffer;
}

TEST_F(InputDeviceTest, should_get_capability_by_type)
{
	const InputCapability* pInputCapability = mInputDevice.GetCapability(InputCapabilityStub::TYPE);
	ASSERT_TRUE(pInputCapability != NULL);
}

}
