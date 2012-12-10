#include "gtest/gtest.h"

namespace Wire
{

class Win32MappedSwingTest : public ::testing::Test
{
protected:
	Win32MappedSwingTest()
	{
	}

	virtual ~Win32MappedSwingTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32MappedSwingTest, should_not_map_any_direction)
{
	FAIL();
}

}
