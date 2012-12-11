#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireNunchukShake.h>
#include <WireWiiInputDataBuffer.h>

namespace Wire
{

class NunchukShakeTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	NunchukShake mNunchukShake;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	NunchukShakeTest()
		: mNunchukShake(&mInputDevice)
	{
	}

	virtual ~NunchukShakeTest()
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

TEST_F(NunchukShakeTest, should_not_get_any_direction)
{
	ASSERT_EQ(0, mNunchukShake.GetX());
	ASSERT_EQ(0, mNunchukShake.GetY());
	ASSERT_EQ(0, mNunchukShake.GetZ());
}

}
