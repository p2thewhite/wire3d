#include "gtest/gtest.h"

namespace Wire
{

class WiiNunchukTest : public ::testing::Test
{
protected:
	WiiNunchukTest()
	{
	}

	virtual ~WiiNunchukTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(WiiNunchukTest, should_set_up_correct_capabilities)
{
	FAIL();
}

}
