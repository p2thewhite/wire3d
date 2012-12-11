#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireNunchukButtons.h>
#include <WireWiiInputDataBuffer.h>
#include <WireButtons.h>

namespace Wire
{

class NunchukButtonsTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	NunchukButtons mNunchukButtons;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	NunchukButtonsTest()
		: mNunchukButtons(&mInputDevice)
	{
	}

	virtual ~NunchukButtonsTest()
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

TEST_F(NunchukButtonsTest, should_get_C_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_C;
	ASSERT_TRUE(mNunchukButtons.GetButton(Buttons::BUTTON_C));
}

TEST_F(NunchukButtonsTest, should_get_Z_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_Z;
	ASSERT_TRUE(mNunchukButtons.GetButton(Buttons::BUTTON_Z));
}

TEST_F(NunchukButtonsTest, should_not_get_A_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_A;
	ASSERT_FALSE(mNunchukButtons.GetButton(Buttons::BUTTON_A));
}

TEST_F(NunchukButtonsTest, should_not_get_B_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_B;
	ASSERT_FALSE(mNunchukButtons.GetButton(Buttons::BUTTON_B));
}

TEST_F(NunchukButtonsTest, should_not_get_1_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_1;
	ASSERT_FALSE(mNunchukButtons.GetButton(Buttons::BUTTON_1));
}

TEST_F(NunchukButtonsTest, should_not_get_2_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_2;
	ASSERT_FALSE(mNunchukButtons.GetButton(Buttons::BUTTON_2));
}

TEST_F(NunchukButtonsTest, should_not_get_minus_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_MINUS;
	ASSERT_FALSE(mNunchukButtons.GetButton(Buttons::BUTTON_MINUS));
}

TEST_F(NunchukButtonsTest, should_not_get_plus_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_PLUS;
	ASSERT_FALSE(mNunchukButtons.GetButton(Buttons::BUTTON_PLUS));
}

TEST_F(NunchukButtonsTest, should_not_get_home_button)
{
	mpWiiInputDataBuffer->GetData()->btns_h = WPAD_WRAPPER_BUTTON_HOME;
	ASSERT_FALSE(mNunchukButtons.GetButton(Buttons::BUTTON_HOME));
}

}
