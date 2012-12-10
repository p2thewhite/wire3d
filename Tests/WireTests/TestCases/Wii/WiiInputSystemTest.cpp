#include "gtest/gtest.h"

namespace Wire
{

class WiiInputSystemTest : public ::testing::Test
{
protected:
	WiiInputSystemTest()
	{
	}

	virtual ~WiiInputSystemTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(WiiInputSystemTest, should_initialize_PAD_and_WPAD_APIs_on_construction)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_shutdown_WPAD_API_on_destruction)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_initialize_virtual_display_size_for_recently_connected_channels)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_get_and_set_WPAD_data_for_a_given_channel)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_add_new_device_when_a_wii_mote_is_connected_to_a_channel)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_add_new_extension_when_a_nunchuk_is_plugged_to_a_connected_wii_mote)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_remove_device_when_a_wii_mote_is_disconnected_from_a_channel)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_remove_an_extension_when_a_nunchuk_is_unplugged_from_a_connected_wii_mote)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_throw_an_assertion_error_when_an_unsupported_wii_mote_expansion_is_plugged_in)
{
	FAIL();
}

TEST_F(WiiInputSystemTest, should_detect_a_channel_connection_error)
{
	FAIL();
}

}
