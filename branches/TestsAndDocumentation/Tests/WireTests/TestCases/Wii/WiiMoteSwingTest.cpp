#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWiiMoteSwing.h>
#include <WireWiiInputDataBuffer.h>

namespace Wire
{

class WiiMoteSwingTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	WiiMoteSwing mWiiMoteSwing;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	WiiMoteSwingTest()
		: mWiiMoteSwing(&mInputDevice)
	{
	}

	virtual ~WiiMoteSwingTest()
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

TEST_F(WiiMoteSwingTest, should_not_get_directions)
{
	ASSERT_EQ(0, mWiiMoteSwing.GetLeft());
	ASSERT_EQ(0, mWiiMoteSwing.GetRight());
	ASSERT_EQ(0, mWiiMoteSwing.GetUp());
	ASSERT_EQ(0, mWiiMoteSwing.GetDown());
	ASSERT_EQ(0, mWiiMoteSwing.GetBackward());
	ASSERT_EQ(0, mWiiMoteSwing.GetForward());
}

}
