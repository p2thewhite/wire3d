#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWin32KeyMappedSwing.h>
#include <WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32KeyMappedSwingTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedSwing mWin32KeyMappedSwing;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32KeyMappedSwingTest()
		: mWin32KeyMappedSwing(&mInputDevice)
	{
	}

	virtual ~Win32KeyMappedSwingTest()
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

TEST_F(Win32KeyMappedSwingTest, should_not_map_any_direction)
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
