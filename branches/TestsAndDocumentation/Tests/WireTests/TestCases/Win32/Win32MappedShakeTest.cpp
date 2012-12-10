#include "gtest/gtest.h"

namespace Wire
{

class Win32MappedShakeTest : public ::testing::Test
{
protected:
	Win32MappedShakeTest()
	{
	}

	virtual ~Win32MappedShakeTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32MappedShakeTest, should_not_map_any_direction)
{
	FAIL();
}

}
