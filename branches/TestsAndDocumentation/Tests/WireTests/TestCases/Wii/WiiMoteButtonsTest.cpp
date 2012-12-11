#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWiiMoteButtons.h>
#include <WireWiiInputDataBuffer.h>
#include <WireButtons.h>

namespace Wire
{

class WiiMoteButtonsTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	WiiMoteButtons mWiiMoteButtons;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	WiiMoteButtonsTest()
		: mWiiMoteButtons(&mInputDevice)
	{
	}

	virtual ~WiiMoteButtonsTest()
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

TEST_F(WiiMoteButtonsTest, should_get_A_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_A;
	ASSERT_TRUE(mWiiMoteButtons.GetButton(Buttons::BUTTON_A));
}

TEST_F(WiiMoteButtonsTest, should_get_B_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_B;
	ASSERT_TRUE(mWiiMoteButtons.GetButton(Buttons::BUTTON_B));
}

TEST_F(WiiMoteButtonsTest, should_get_1_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_1;
	ASSERT_TRUE(mWiiMoteButtons.GetButton(Buttons::BUTTON_1));
}

TEST_F(WiiMoteButtonsTest, should_get_2_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_2;
	ASSERT_TRUE(mWiiMoteButtons.GetButton(Buttons::BUTTON_2));
}

TEST_F(WiiMoteButtonsTest, should_get_minus_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_MINUS;
	ASSERT_TRUE(mWiiMoteButtons.GetButton(Buttons::BUTTON_MINUS));
}

TEST_F(WiiMoteButtonsTest, should_get_plus_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_PLUS;
	ASSERT_TRUE(mWiiMoteButtons.GetButton(Buttons::BUTTON_PLUS));
}

TEST_F(WiiMoteButtonsTest, should_get_home_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_HOME;
	ASSERT_TRUE(mWiiMoteButtons.GetButton(Buttons::BUTTON_HOME));
}

TEST_F(WiiMoteButtonsTest, should_not_get_C_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_C;
	ASSERT_FALSE(mWiiMoteButtons.GetButton(Buttons::BUTTON_C));
}

TEST_F(WiiMoteButtonsTest, should_not_get_Z_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_Z;
	ASSERT_FALSE(mWiiMoteButtons.GetButton(Buttons::BUTTON_Z));
}

}
