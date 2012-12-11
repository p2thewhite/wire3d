#include "gtest/gtest.h"

#include <WireWiiMote.h>
#include <WireWiiNunchuk.h>
#include <WireAnalogPad.h>
#include <WireButtons.h>
#include <WireShake.h>
#include <WireSwing.h>
#include <WireTilt.h>

namespace Wire
{

class WiiNunchukTest : public ::testing::Test
{
protected:
	WiiMote mWiiMote;
	WiiNunchuk mWiiNunchuk;

	WiiNunchukTest()
		: mWiiMote(0), mWiiNunchuk(&mWiiMote)
	{
	}

	virtual ~WiiNunchukTest()
	{
	}

	virtual void SetUp()
	{
		mWiiNunchuk.SetUp();
	}

	virtual void TearDown()
	{
	}

};

TEST_F(WiiNunchukTest, should_set_up_correct_capabilities)
{
	ASSERT_TRUE(mWiiNunchuk.HasCapability(AnalogPad::TYPE));
	ASSERT_TRUE(mWiiNunchuk.HasCapability(Buttons::TYPE));
	ASSERT_TRUE(mWiiNunchuk.HasCapability(Shake::TYPE));
	ASSERT_TRUE(mWiiNunchuk.HasCapability(Swing::TYPE));
	ASSERT_TRUE(mWiiNunchuk.HasCapability(Tilt::TYPE));
}

}
