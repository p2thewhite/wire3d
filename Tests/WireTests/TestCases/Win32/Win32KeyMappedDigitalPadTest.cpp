#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWin32KeyMappedDigitalPad.h>
#include <WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32KeyMappedDigitalPadTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedDigitalPad mWin32KeyMappedDigitalPad;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32KeyMappedDigitalPadTest()
		: mWin32KeyMappedDigitalPad(&mInputDevice)
	{
	}

	virtual ~Win32KeyMappedDigitalPadTest()
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

TEST_F(Win32KeyMappedDigitalPadTest, should_map_down_key_to_down_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_DOWN);
	ASSERT_TRUE(mWin32KeyMappedDigitalPad.GetDown());
}

TEST_F(Win32KeyMappedDigitalPadTest, should_map_left_key_to_left_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_LEFT);
	ASSERT_TRUE(mWin32KeyMappedDigitalPad.GetLeft());
}

TEST_F(Win32KeyMappedDigitalPadTest, should_map_right_key_to_right_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_RIGHT);
	ASSERT_TRUE(mWin32KeyMappedDigitalPad.GetRight());
}

TEST_F(Win32KeyMappedDigitalPadTest, should_map_up_key_to_up_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(VK_UP);
	ASSERT_TRUE(mWin32KeyMappedDigitalPad.GetUp());
}

}
