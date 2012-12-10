#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <Win32/WireWin32KeyMappedIR.h>
#include <Win32/WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32MappedIRTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedIR mWin32KeyMappedIR;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32MappedIRTest()
		: mWin32KeyMappedIR(&mInputDevice)
	{
	}

	virtual ~Win32MappedIRTest()
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

TEST_F(Win32MappedIRTest, should_map_mouse_Y_axis_to_up_down_axis)
{
	mpWin32InputDataBuffer->SetMouseY(100);
	ASSERT_FLOAT_EQ(-100.0f, mWin32KeyMappedIR.GetDown());
	ASSERT_FLOAT_EQ(100.0f, mWin32KeyMappedIR.GetUp());
}

TEST_F(Win32MappedIRTest, should_map_mouse_X_axis_to_left_right_axis)
{
	mpWin32InputDataBuffer->SetMouseX(100);
	ASSERT_FLOAT_EQ(-100.0f, mWin32KeyMappedIR.GetLeft());
	ASSERT_FLOAT_EQ(100.0f, mWin32KeyMappedIR.GetRight());
}

TEST_F(Win32MappedIRTest, should_not_map_forward_backward_axis)
{
	mpWin32InputDataBuffer->SetMouseY(100);
	mpWin32InputDataBuffer->SetMouseX(100);
	mpWin32InputDataBuffer->IncrementMouseWheel(100.0f);
	ASSERT_FLOAT_EQ(0, mWin32KeyMappedIR.GetBackward());
	ASSERT_FLOAT_EQ(0, mWin32KeyMappedIR.GetForward());
}

}
