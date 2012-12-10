#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/WireInputDeviceStub.h"
#include <Win32/WireWin32KeyMappedSwing.h>
#include <Win32/WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32MappedSwingTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedSwing mWin32KeyMappedSwing;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32MappedSwingTest()
		: mWin32KeyMappedSwing(&mInputDevice)
	{
	}

	virtual ~Win32MappedSwingTest()
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

TEST_F(Win32MappedSwingTest, should_not_map_any_direction)
{
	mpWin32InputDataBuffer->SetMouseY(100);
	mpWin32InputDataBuffer->SetMouseX(100);
	mpWin32InputDataBuffer->IncrementMouseWheel(100.0f);
	ASSERT_EQ(0, mWin32KeyMappedSwing.GetLeft());
	ASSERT_EQ(0, mWin32KeyMappedSwing.GetRight());
	ASSERT_EQ(0, mWin32KeyMappedSwing.GetUp());
	ASSERT_EQ(0, mWin32KeyMappedSwing.GetDown());
	ASSERT_EQ(0, mWin32KeyMappedSwing.GetBackward());
	ASSERT_EQ(0, mWin32KeyMappedSwing.GetForward());
}

}
