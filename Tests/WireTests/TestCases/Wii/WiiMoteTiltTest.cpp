#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWiiMoteTilt.h>
#include <WireWiiInputDataBuffer.h>

namespace Wire
{

class WiiMoteTiltTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	WiiMoteTilt mWiiMoteTilt;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	WiiMoteTiltTest()
		: mWiiMoteTilt(&mInputDevice)
	{
	}

	virtual ~WiiMoteTiltTest()
	{
	}

	virtual void SetUp()
	{
		mpWiiInputDataBuffer = new WiiInputDataBuffer();
		mInputDevice.SetInputDataBuffer(mpWiiInputDataBuffer);
	}

	virtual void TearDown()
	{
		delete mpWiiInputDataBuffer;
	}

};

TEST_F(WiiMoteTiltTest, should_get_left_direction)
{
	mpWiiInputDataBuffer->GetData()->orient.roll = 100.0f;
	ASSERT_EQ(100.0f, mWiiMoteTilt.GetLeft());
}

TEST_F(WiiMoteTiltTest, should_get_right_direction)
{
	mpWiiInputDataBuffer->GetData()->orient.roll = -100.0f;
	ASSERT_EQ(100.0f, mWiiMoteTilt.GetRight());
}

TEST_F(WiiMoteTiltTest, should_get_forward_direction)
{
	mpWiiInputDataBuffer->GetData()->orient.pitch = -100.0f;
	ASSERT_EQ(100.0f, mWiiMoteTilt.GetForward());
}

TEST_F(WiiMoteTiltTest, should_get_backward_direction)
{
	mpWiiInputDataBuffer->GetData()->orient.pitch = 100.0f;
	ASSERT_EQ(100.0f, mWiiMoteTilt.GetBackward());
}

}
