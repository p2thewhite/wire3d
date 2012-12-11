#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWiiMoteDigitalPad.h>
#include <WireWiiInputDataBuffer.h>

namespace Wire
{

class WiiMoteDigitalPadTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	WiiMoteDigitalPad mWiiMoteDigitalPad;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	WiiMoteDigitalPadTest()
		: mWiiMoteDigitalPad(&mInputDevice)
	{
	}

	virtual ~WiiMoteDigitalPadTest()
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

TEST_F(WiiMoteDigitalPadTest, should_get_up_direction)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_UP;
	ASSERT_TRUE(mWiiMoteDigitalPad.GetUp());
}

TEST_F(WiiMoteDigitalPadTest, should_get_down_direction)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_DOWN;
	ASSERT_TRUE(mWiiMoteDigitalPad.GetDown());
}

TEST_F(WiiMoteDigitalPadTest, should_get_left_direction)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_LEFT;
	ASSERT_TRUE(mWiiMoteDigitalPad.GetLeft());
}

TEST_F(WiiMoteDigitalPadTest, should_get_right_direction)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_RIGHT;
	ASSERT_TRUE(mWiiMoteDigitalPad.GetRight());
}

}
