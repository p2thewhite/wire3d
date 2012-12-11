#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWin32KeyMappedTilt.h>
#include <WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32KeyMappedTiltTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedTilt mWin32KeyMappedTilt;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32KeyMappedTiltTest()
		: mWin32KeyMappedTilt(&mInputDevice)
	{
	}

	virtual ~Win32KeyMappedTiltTest()
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

TEST_F(Win32KeyMappedTiltTest, should_map_negative_mouse_wheel_to_left_direction)
{
	mpWin32InputDataBuffer->IncrementMouseWheel(-100.0f);
	ASSERT_FLOAT_EQ(100.0f, mWin32KeyMappedTilt.GetLeft());
}

TEST_F(Win32KeyMappedTiltTest, should_map_positive_mouse_wheel_to_right_direction)
{
	mpWin32InputDataBuffer->IncrementMouseWheel(100.0f);
	ASSERT_FLOAT_EQ(100.0f, mWin32KeyMappedTilt.GetRight());
}

}
