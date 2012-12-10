#include "gtest/gtest.h"

#include <Win32/WireWin32EmulatedWiiMote.h>
#include <Win32/WireWin32EmulatedNunchuk.h>
#include <WireAnalogPad.h>
#include <WireButtons.h>
#include <WireShake.h>
#include <WireSwing.h>
#include <WireTilt.h>

namespace Wire
{

class Win32EmulatedNunchukTest : public ::testing::Test
{
protected:
	Win32EmulatedWiiMote mWin32EmulatedWiiMote;
	Win32EmulatedNunchuk mWin32EmulatedNunchuk;

	Win32EmulatedNunchukTest()
		: mWin32EmulatedNunchuk(&mWin32EmulatedWiiMote)
	{
	}

	virtual ~Win32EmulatedNunchukTest()
	{
	}

	virtual void SetUp()
	{
		mWin32EmulatedNunchuk.SetUp();
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Win32EmulatedNunchukTest, should_set_up_correct_capabilities)
{
	ASSERT_TRUE(mWin32EmulatedNunchuk.HasCapability(AnalogPad::TYPE));
	ASSERT_TRUE(mWin32EmulatedNunchuk.HasCapability(Buttons::TYPE));
	ASSERT_TRUE(mWin32EmulatedNunchuk.HasCapability(Shake::TYPE));
	ASSERT_TRUE(mWin32EmulatedNunchuk.HasCapability(Swing::TYPE));
	ASSERT_TRUE(mWin32EmulatedNunchuk.HasCapability(Tilt::TYPE));
}

}
