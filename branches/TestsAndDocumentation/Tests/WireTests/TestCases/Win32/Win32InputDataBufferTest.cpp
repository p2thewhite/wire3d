#include "gtest/gtest.h"

#include <Win32/WireWin32InputDataBuffer.h>

namespace Wire
{

class Win32InputDataBufferTest : public ::testing::Test
{
protected:
	Win32InputDataBuffer mWin32InputDataBuffer;

	Win32InputDataBufferTest()
	{
	}

	virtual ~Win32InputDataBufferTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32InputDataBufferTest, should_get_and_set_key)
{
	mWin32InputDataBuffer.SetKeyDown(100);
	ASSERT_TRUE(mWin32InputDataBuffer.GetKey(100));
}

TEST_F(Win32InputDataBufferTest, should_get_and_set_mouse)
{
	mWin32InputDataBuffer.SetMouseX(100);
	mWin32InputDataBuffer.SetMouseY(100);
	ASSERT_EQ(100, mWin32InputDataBuffer.GetMouseX());
	ASSERT_EQ(100, mWin32InputDataBuffer.GetMouseY());
}

TEST_F(Win32InputDataBufferTest, should_get_and_set_mouse_wheel)
{
	mWin32InputDataBuffer.IncrementMouseWheel(0.5f);
	ASSERT_FLOAT_EQ(0.5f, mWin32InputDataBuffer.GetMouseWheel());
}

TEST_F(Win32InputDataBufferTest, should_copy_from_other_input_data_buffer)
{
	Win32InputDataBuffer* pWin32InputDataBuffer;
	pWin32InputDataBuffer = new Win32InputDataBuffer();
	pWin32InputDataBuffer->SetKeyDown(100);
	pWin32InputDataBuffer->IncrementMouseWheel(0.5f);
	pWin32InputDataBuffer->SetMouseX(100);
	pWin32InputDataBuffer->SetMouseY(100);
	mWin32InputDataBuffer.CopyFrom(pWin32InputDataBuffer);
	ASSERT_TRUE(mWin32InputDataBuffer.GetKey(100));
	ASSERT_EQ(100, mWin32InputDataBuffer.GetMouseX());
	ASSERT_EQ(100, mWin32InputDataBuffer.GetMouseY());
	ASSERT_FLOAT_EQ(0.5f, mWin32InputDataBuffer.GetMouseWheel());
}

}
