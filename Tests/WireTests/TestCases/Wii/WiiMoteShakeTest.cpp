#include "gtest/gtest.h"

namespace Wire
{

class WiiMoteShakeTest : public ::testing::Test
{
protected:
	WiiMoteShakeTest()
	{
	}

	virtual ~WiiMoteShakeTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(WiiMoteShakeTest, should_not_get_directions)
{
	FAIL();
}

}
