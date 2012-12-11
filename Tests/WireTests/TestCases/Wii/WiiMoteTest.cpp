#include "gtest/gtest.h"

#include <WireWiiMote.h>
#include <WireDigitalPad.h>
#include <WireButtons.h>
#include <WireIR.h>
#include <WireShake.h>
#include <WireSwing.h>
#include <WireTilt.h>

namespace Wire
{

class WiiMoteTest : public ::testing::Test
{
protected:
	WiiMote mWiiMote;

	WiiMoteTest()
		: mWiiMote(0)
	{
	}

	virtual ~WiiMoteTest()
	{
	}

	virtual void SetUp()
	{
		mWiiMote.SetUp();
	}

	virtual void TearDown()
	{
	}

};

TEST_F(WiiMoteTest, should_set_up_correct_capabilities)
{
	ASSERT_TRUE(mWiiMote.HasCapability(DigitalPad::TYPE, false));
	ASSERT_TRUE(mWiiMote.HasCapability(IR::TYPE, false));
	ASSERT_TRUE(mWiiMote.HasCapability(Buttons::TYPE, false));
	ASSERT_TRUE(mWiiMote.HasCapability(Shake::TYPE, false));
	ASSERT_TRUE(mWiiMote.HasCapability(Swing::TYPE, false));
	ASSERT_TRUE(mWiiMote.HasCapability(Tilt::TYPE, false));
}

}
