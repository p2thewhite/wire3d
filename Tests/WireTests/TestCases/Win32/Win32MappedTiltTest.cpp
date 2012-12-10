#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/WireInputDeviceStub.h"
#include <Win32/WireWin32KeyMappedTilt.h>
#include <Win32/WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32MappedTiltTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedTilt mWin32KeyMappedTilt;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32MappedTiltTest()
		: mWin32KeyMappedTilt(&mInputDevice)
	{
	}

	virtual ~Win32MappedTiltTest()
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

TEST_F(Win32MappedTiltTest, should_map_negative_mouse_wheel_to_left_direction)
{
	mpWin32InputDataBuffer->IncrementMouseWheel(-100.0f);
	ASSERT_EQ(100.0f, mWin32KeyMappedTilt.GetLeft());
}

TEST_F(Win32MappedTiltTest, should_map_positive_mouse_wheel_to_right_direction)
{
	mpWin32InputDataBuffer->IncrementMouseWheel(100.0f);
	ASSERT_EQ(100.0f, mWin32KeyMappedTilt.GetRight());
}

}
