#include "gtest/gtest.h"

#include <WireWin32EmulatedWiiMote.h>
#include <WireDigitalPad.h>
#include <WireButtons.h>
#include <WireIR.h>
#include <WireShake.h>
#include <WireSwing.h>
#include <WireTilt.h>

namespace Wire
{

class Win32EmulatedWiiMoteTest : public ::testing::Test
{
protected:
	Win32EmulatedWiiMote mWin32EmulatedWiiMote;

	Win32EmulatedWiiMoteTest()
	{
	}

	virtual ~Win32EmulatedWiiMoteTest()
	{
	}

	virtual void SetUp()
	{
		mWin32EmulatedWiiMote.SetUp();
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32EmulatedWiiMoteTest, should_set_up_correct_capabilities)
{
	ASSERT_TRUE(mWin32EmulatedWiiMote.HasCapability(DigitalPad::TYPE, false));
	ASSERT_TRUE(mWin32EmulatedWiiMote.HasCapability(IR::TYPE, false));
	ASSERT_TRUE(mWin32EmulatedWiiMote.HasCapability(Buttons::TYPE, false));
	ASSERT_TRUE(mWin32EmulatedWiiMote.HasCapability(Shake::TYPE, false));
	ASSERT_TRUE(mWin32EmulatedWiiMote.HasCapability(Swing::TYPE, false));
	ASSERT_TRUE(mWin32EmulatedWiiMote.HasCapability(Tilt::TYPE, false));
}

}
