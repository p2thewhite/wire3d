#include "gtest/gtest.h"

namespace Wire
{

class Win32MappedIRTest : public ::testing::Test
{
protected:
	Win32MappedIRTest()
	{
	}

	virtual ~Win32MappedIRTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32MappedIRTest, should_map_mouse_Y_axis_to_up_down_axis)
{
	FAIL();
}

TEST_F(Win32MappedIRTest, should_map_mouse_X_axis_to_left_right_axis)
{
	FAIL();
}

TEST_F(Win32MappedIRTest, should_not_map_forward_backward_axis)
{
	FAIL();
}

}
