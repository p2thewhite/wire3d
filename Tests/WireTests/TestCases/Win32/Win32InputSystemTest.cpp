#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/Win32/WireWin32InputSystemStub.h"

namespace Wire
{

class Win32InputSystemTest : public ::testing::Test
{
protected:
	Win32InputSystemStub mInputSystem;

	Win32InputSystemTest()
	{
	}

	virtual ~Win32InputSystemTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32InputSystemTest, should_decode_mouse_button_system_messages)
{
	mInputSystem.OnSystemMessage(WM_MBUTTONDOWN, 0, 0);
	ASSERT_TRUE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_MBUTTON));
	mInputSystem.OnSystemMessage(WM_MBUTTONUP, 0, 0);
	ASSERT_FALSE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_MBUTTON));
	mInputSystem.OnSystemMessage(WM_RBUTTONDOWN, 0, 0);
	ASSERT_TRUE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_RBUTTON));
	mInputSystem.OnSystemMessage(WM_RBUTTONUP, 0, 0);
	ASSERT_FALSE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_RBUTTON));
	mInputSystem.OnSystemMessage(WM_LBUTTONDOWN, 0, 0);
	ASSERT_TRUE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_LBUTTON));
	mInputSystem.OnSystemMessage(WM_LBUTTONUP, 0, 0);
	ASSERT_FALSE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_LBUTTON));
}

TEST_F(Win32InputSystemTest, should_decode_mouse_wheel_system_message)
{
	mInputSystem.OnSystemMessage(WM_MOUSEWHEEL, (100 << 16) * WHEEL_DELTA, 0);
	ASSERT_FLOAT_EQ(100, mInputSystem.GetBackInputDataBuffer_TEST()->GetMouseWheel());
}

TEST_F(Win32InputSystemTest, should_decode_mouse_movement_system_message)
{
	mInputSystem.OnSystemMessage(WM_MOUSEMOVE, 0, ((200 << 16) | 300));
	ASSERT_EQ(200, mInputSystem.GetBackInputDataBuffer_TEST()->GetMouseY());
	ASSERT_EQ(300, mInputSystem.GetBackInputDataBuffer_TEST()->GetMouseX());
}

TEST_F(Win32InputSystemTest, should_decode_keyboard_system_messages)
{
	mInputSystem.OnSystemMessage(WM_KEYDOWN, VK_RETURN, 0);
	ASSERT_TRUE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_RETURN));
	mInputSystem.OnSystemMessage(WM_KEYUP, VK_RETURN, 0);
	ASSERT_FALSE(mInputSystem.GetBackInputDataBuffer_TEST()->GetKey(VK_RETURN));
}

}
