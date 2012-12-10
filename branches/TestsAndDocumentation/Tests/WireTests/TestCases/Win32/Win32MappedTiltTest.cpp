#include "gtest/gtest.h"

namespace Wire
{

class Win32MappedTiltTest : public ::testing::Test
{
protected:
	Win32MappedTiltTest()
	{
	}

	virtual ~Win32MappedTiltTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32MappedTiltTest, should_map_negative_mouse_wheel_to_left_direction)
{
	FAIL();
}

TEST_F(Win32MappedTiltTest, should_map_positive_mouse_wheel_to_right_direction)
{
	FAIL();
}

}
