#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/WireInputDeviceStub.h"
#include <Win32/WireWin32KeyMappedButtons.h>
#include <Win32/WireWin32InputDataBuffer.h>
#include <WireButtons.h>

namespace Wire
{

class Win32KeyMappedButtonsTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedButtons mWin32KeyMappedButtons;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32KeyMappedButtonsTest()
		: mWin32KeyMappedButtons(&mInputDevice)
	{
	}

	virtual ~Win32KeyMappedButtonsTest()
	{
	}

	virtual void SetUp()
	{
		mpWin32InputDataBuffer = new Win32InputDataBuffer();
		mInputDevice.SetInputDataBuffer(mpWin32InputDataBuffer);
	}

	virtual void TearDown()
	{
		delete mpWin32InputDataBuffer;
	}

};

TEST_F(Win32KeyMappedButtonsTest, should_map_left_mouse_button_to_A_button)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_LBUTTON);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_A));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_right_mouse_button_to_B_button)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_RBUTTON);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_B));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_C_key_to_C_button)
{
	mpWin32InputDataBuffer->SetKeyDown(0x43);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_C));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_escape_key_to_home_button)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_ESCAPE);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_HOME));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_subtract_key_to_minus_button)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_SUBTRACT);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_MINUS));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_add_key_to_plus_button)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_ADD);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_PLUS));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_Z_key_to_Z_button)
{
	mpWin32InputDataBuffer->SetKeyDown(0x5A);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_Z));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_1_key_to_1_button)
{
	mpWin32InputDataBuffer->SetKeyDown(0x31);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_1));
}

TEST_F(Win32KeyMappedButtonsTest, should_map_2_key_to_2_button)
{
	mpWin32InputDataBuffer->SetKeyDown(0x32);
	ASSERT_TRUE(mWin32KeyMappedButtons.GetButton(Buttons::BUTTON_2));
}

}
