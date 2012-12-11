#include "gtest/gtest.h"

#include "../../Stubs/WireInputDeviceStub.h"
#include <WireWiiMoteIR.h>
#include <WireWiiInputDataBuffer.h>
#include <WireMath.h>

namespace Wire
{

class WiiMoteIRTest : public ::testing::Test
{
protected:
	InputDeviceStub mInputDevice;
	WiiMoteIR mWiiMoteIR;
	WiiInputDataBuffer* mpWiiInputDataBuffer;

	WiiMoteIRTest()
		: mWiiMoteIR(&mInputDevice)
	{
	}

	virtual ~WiiMoteIRTest()
	{
	}

	virtual void SetUp()
	{
		mpWiiInputDataBuffer = new WiiInputDataBuffer();
		mInputDevice.SetInputDataBuffer(mpWiiInputDataBuffer);
	}

	virtual void TearDown()
	{
		delete mpWiiInputDataBuffer;
	}

};

TEST_F(WiiMoteIRTest, should_get_up_direction)
{
	mpWiiInputDataBuffer->GetData()->ir.valid = 1;
	mpWiiInputDataBuffer->GetData()->ir.y = 100.0f;
	ASSERT_FLOAT_EQ(100.0f, mWiiMoteIR.GetUp());
}

TEST_F(WiiMoteIRTest, should_get_down_direction)
{
	mpWiiInputDataBuffer->GetData()->ir.valid = 1;
	mpWiiInputDataBuffer->GetData()->ir.y = -100.0f;
	ASSERT_FLOAT_EQ(100.0f, mWiiMoteIR.GetDown());
}

TEST_F(WiiMoteIRTest, should_get_left_direction)
{
	mpWiiInputDataBuffer->GetData()->ir.valid = 1;
	mpWiiInputDataBuffer->GetData()->ir.x = -100.0f;
	ASSERT_FLOAT_EQ(100.0f, mWiiMoteIR.GetLeft());
}

TEST_F(WiiMoteIRTest, should_get_right_direction)
{
	mpWiiInputDataBuffer->GetData()->ir.valid = 1;
	mpWiiInputDataBuffer->GetData()->ir.x = 100.0f;
	ASSERT_FLOAT_EQ(100.0f, mWiiMoteIR.GetRight());
}

TEST_F(WiiMoteIRTest, should_get_forward_direction)
{
	mpWiiInputDataBuffer->GetData()->ir.valid = 1;
	mpWiiInputDataBuffer->GetData()->ir.z = 100.0f;
	ASSERT_FLOAT_EQ(100.0f, mWiiMoteIR.GetForward());
}

TEST_F(WiiMoteIRTest, should_get_backward_direction)
{
	mpWiiInputDataBuffer->GetData()->ir.valid = 1;
	mpWiiInputDataBuffer->GetData()->ir.z = -100.0f;
	ASSERT_FLOAT_EQ(100.0f, mWiiMoteIR.GetBackward());
}

TEST_F(WiiMoteIRTest, should_ignore_invalid_IR_trackings)
{
	mpWiiInputDataBuffer->GetData()->ir.valid = 0;
	ASSERT_FLOAT_EQ(MathF::MAX_REAL, mWiiMoteIR.GetUp());
	ASSERT_FLOAT_EQ(-MathF::MAX_REAL, mWiiMoteIR.GetDown());
	ASSERT_FLOAT_EQ(-MathF::MAX_REAL, mWiiMoteIR.GetLeft());
	ASSERT_FLOAT_EQ(MathF::MAX_REAL, mWiiMoteIR.GetRight());
	ASSERT_FLOAT_EQ(MathF::MAX_REAL, mWiiMoteIR.GetForward());
	ASSERT_FLOAT_EQ(-MathF::MAX_REAL, mWiiMoteIR.GetBackward());
}

}
