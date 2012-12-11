#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWiiMoteShake.h>
#include <WireWiiInputDataBuffer.h>

namespace Wire
{

class WiiMoteShakeTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	WiiMoteShake mWiiMoteShake;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	WiiMoteShakeTest()
		: mWiiMoteShake(&mInputDevice)
	{
	}

	virtual ~WiiMoteShakeTest()
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

TEST_F(WiiMoteShakeTest, should_not_get_directions)
{
	ASSERT_EQ(0, mWiiMoteShake.GetX());
	ASSERT_EQ(0, mWiiMoteShake.GetY());
	ASSERT_EQ(0, mWiiMoteShake.GetZ());
}

}
