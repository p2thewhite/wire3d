#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireNunchukTilt.h>
#include <WireWiiInputDataBuffer.h>

namespace Wire
{

class NunchukTiltTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	NunchukTilt mNunchukTilt;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	NunchukTiltTest()
		: mNunchukTilt(&mInputDevice)
	{
	}

	virtual ~NunchukTiltTest()
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

TEST_F(NunchukTiltTest, should_not_get_any_direction)
{
	mpWiiInputDataBuffer->GetData()->orient.roll = 100.0f;
	mpWiiInputDataBuffer->GetData()->orient.pitch = 100.0f;
	ASSERT_FLOAT_EQ(0, mNunchukTilt.GetLeft());
	ASSERT_FLOAT_EQ(0, mNunchukTilt.GetRight());
	ASSERT_FLOAT_EQ(0, mNunchukTilt.GetBackward());
	ASSERT_FLOAT_EQ(0, mNunchukTilt.GetForward());
}

}
