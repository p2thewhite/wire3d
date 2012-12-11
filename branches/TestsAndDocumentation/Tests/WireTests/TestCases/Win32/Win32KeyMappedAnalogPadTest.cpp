#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWin32KeyMappedAnalogPad.h>
#include <WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32KeyMappedAnalogPadTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedAnalogPad mWin32KeyMappedAnalogPad;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32KeyMappedAnalogPadTest()
		: mWin32KeyMappedAnalogPad(&mInputDevice)
	{
	}

	virtual ~Win32KeyMappedAnalogPadTest()
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

TEST_F(Win32KeyMappedAnalogPadTest, should_map_W_key_to_up_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(0x57); // 'w' key
	ASSERT_FLOAT_EQ(1.0f, mWin32KeyMappedAnalogPad.GetUp());
}

TEST_F(Win32KeyMappedAnalogPadTest, should_map_A_key_to_left_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(0x41); // 'a' key
	ASSERT_FLOAT_EQ(1.0f, mWin32KeyMappedAnalogPad.GetLeft());
}

TEST_F(Win32KeyMappedAnalogPadTest, should_map_S_key_to_down_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(0x53); // 's' key
	ASSERT_FLOAT_EQ(1.0f, mWin32KeyMappedAnalogPad.GetDown());
}

TEST_F(Win32KeyMappedAnalogPadTest, should_map_D_key_to_right_direction)
{
	mpWin32InputDataBuffer->SetKeyDown(0x44); // 'd' key
	ASSERT_FLOAT_EQ(1.0f, mWin32KeyMappedAnalogPad.GetRight());
}

}
