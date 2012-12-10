#include "gtest/gtest.h"

namespace Wire
{

class NunchukTiltTest : public ::testing::Test
{
protected:
	NunchukTiltTest()
	{
	}

	virtual ~NunchukTiltTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(NunchukTiltTest, should_not_get_any_direction)
{
	FAIL();
}

}
