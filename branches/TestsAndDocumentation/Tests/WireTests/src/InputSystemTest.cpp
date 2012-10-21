#include "gtest/gtest.h"

namespace Wire
{

	class InputSystemTest : public ::testing::Test
	{
	protected:
		InputSystemTest ()
		{
		}

		virtual ~InputSystemTest ()
		{
		}

		virtual void SetUp()
		{
		}

		virtual void TearDown()
		{
		}

	};

	TEST_F(InputSystemTest , Capture)
	{
		EXPECT_TRUE(true);
	}

}
