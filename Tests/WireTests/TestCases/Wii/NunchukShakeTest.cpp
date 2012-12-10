#include "gtest/gtest.h"

namespace Wire
{

class NunchukShakeTest : public ::testing::Test
{
protected:
	NunchukShakeTest()
	{
	}

	virtual ~NunchukShakeTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(NunchukShakeTest, should_not_get_any_direction)
{
	FAIL();
}

}
