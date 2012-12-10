#include "gtest/gtest.h"

namespace Wire
{

class WiiMoteTest : public ::testing::Test
{
protected:
	WiiMoteTest()
	{
	}

	virtual ~WiiMoteTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(WiiMoteTest, should_set_up_correct_capabilities)
{
	FAIL();
}

}
