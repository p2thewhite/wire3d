#include "gtest/gtest.h"

#include <Windows.h>
#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWin32KeyMappedShake.h>
#include <WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32KeyMappedShakeTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	Win32KeyMappedShake mWin32KeyMappedShake;
	Win32InputDataBuffer* mpWin32InputDataBuffer;

	Win32KeyMappedShakeTest()
		: mWin32KeyMappedShake(&mInputDevice)
	{
	}

	virtual ~Win32KeyMappedShakeTest()
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

TEST_F(Win32KeyMappedShakeTest, should_not_map_any_direction)
{
	mpWin32InputDataBuffer->SetMouseY(100);
	mpWin32InputDataBuffer->SetMouseX(100);
	mpWin32InputDataBuffer->IncrementMouseWheel(100.0f);
	ASSERT_EQ(0, mWin32KeyMappedShake.GetX());
	ASSERT_EQ(0, mWin32KeyMappedShake.GetY());
	ASSERT_EQ(0, mWin32KeyMappedShake.GetZ());
}

}