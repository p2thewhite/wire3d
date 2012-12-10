#include "gtest/gtest.h"

namespace Wire
{

class WiiMoteSwingTest : public ::testing::Test
{
protected:
	WiiMoteSwingTest()
	{
	}

	virtual ~WiiMoteSwingTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(WiiMoteSwingTest, should_not_get_directions)
{
	FAIL();
}

}
