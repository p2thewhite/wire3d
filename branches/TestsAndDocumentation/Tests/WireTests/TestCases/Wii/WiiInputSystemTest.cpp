#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <WireMemory.h>
#include <WireWiiInputSystem.h>
#include <WireWiiMote.h>
#include <WireWiiInputDataBuffer.h>
#include <WireWPADWrapper.h>
#include "../../Mocks/WireApplicationMock.h"
#include "../../Mocks/Wii/WireWPADWrapperMock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::Eq;
using ::testing::SetArgPointee;
using ::testing::Invoke;

namespace Wire
{

class WiiInputSystemTest : public ::testing::Test
{
protected:

	ApplicationMock mApplicationMock;
	WPADWrapperMock mWPADWrapperMock;

	WiiInputSystemTest()
	{
	}

	virtual ~WiiInputSystemTest()
	{
	}

	virtual void SetUp()
	{
		Application::SetApplication(&mApplicationMock);
		WiiInputSystem::SetWPADWrapper(&mWPADWrapperMock);

		EXPECT_CALL(mApplicationMock, GetWidth()).WillRepeatedly(Return(ApplicationMock::WIDTH));
		EXPECT_CALL(mApplicationMock, GetHeight()).WillRepeatedly(Return(ApplicationMock::HEIGHT));
	}

	virtual void TearDown()
	{
	}

};

void CopyWPADWrapperData(UInt channel, WPADWrapperData* pWPADWrapperData)
{
	pWPADWrapperData->btns_h = 1;
	pWPADWrapperData->btns_l = 2;
	pWPADWrapperData->btns_d = 3;
	pWPADWrapperData->btns_u = 4;
}

TEST_F(WiiInputSystemTest, should_initialize_PAD_and_WPAD_APIs_on_construction)
{
	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown());

	// --

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_shutdown_WPAD_API_on_destruction)
{
	EXPECT_CALL(mWPADWrapperMock, Init());
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_initialize_virtual_display_size_for_recently_connected_channels)
{
	UInt connectedChannel = 0;

	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(connectedChannel), _)).WillOnce(Return(WPAD_WRAPPER_ERR_NONE));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(1), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(2), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(3), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, SetDataFormat(Eq(connectedChannel), Eq(WPAD_WRAPPER_FMT_BTNS_ACC_IR))).Times(1);

	WPADWrapperExpansion expansion;
	expansion.type = WPAD_WRAPPER_EXP_NONE;
	EXPECT_CALL(mWPADWrapperMock, GetExpansion(Eq(connectedChannel), _)).WillOnce(SetArgPointee<1>(expansion));

	EXPECT_CALL(mWPADWrapperMock, SetVRes(Eq(connectedChannel), Eq(ApplicationMock::WIDTH), Eq(ApplicationMock::HEIGHT))).Times(1);

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	pWiiInputSystem->DiscoverDevices();

	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_get_and_set_WPAD_data_for_a_given_channel)
{
	UInt connectedChannel = 0;

	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(connectedChannel), _)).WillOnce(Return(WPAD_WRAPPER_ERR_NONE));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(1), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(2), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(3), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, SetDataFormat(Eq(connectedChannel), Eq(WPAD_WRAPPER_FMT_BTNS_ACC_IR))).Times(1);

	WPADWrapperExpansion expansion;
	expansion.type = WPAD_WRAPPER_EXP_NONE;
	EXPECT_CALL(mWPADWrapperMock, GetExpansion(Eq(connectedChannel), _)).WillOnce(SetArgPointee<1>(expansion));

	EXPECT_CALL(mWPADWrapperMock, SetVRes(Eq(connectedChannel), Eq(ApplicationMock::WIDTH), Eq(ApplicationMock::HEIGHT))).Times(1);
	EXPECT_CALL(mWPADWrapperMock, GetData(Eq(connectedChannel), _)).WillOnce(Invoke(CopyWPADWrapperData));

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	pWiiInputSystem->DiscoverDevices();
	pWiiInputSystem->Capture();

	const WiiMote* pWiiMote = static_cast<const WiiMote*>(pWiiInputSystem->GetMainDevice(0));
	const WiiInputDataBuffer* pWiiInputDataBuffer = static_cast<const WiiInputDataBuffer*>(pWiiMote->GetInputDataBuffer());
	const WPADWrapperData* pData = pWiiInputDataBuffer->GetConstData();

	EXPECT_EQ(1, pData->btns_h);
	EXPECT_EQ(2, pData->btns_l);
	EXPECT_EQ(3, pData->btns_d);
	EXPECT_EQ(4, pData->btns_u);

	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_add_new_device_when_a_wii_mote_is_connected_to_a_channel)
{
	UInt connectedChannel = 0;

	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(connectedChannel), _)).WillOnce(Return(WPAD_WRAPPER_ERR_NONE));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(1), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(2), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(3), _)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, SetDataFormat(Eq(connectedChannel), Eq(WPAD_WRAPPER_FMT_BTNS_ACC_IR))).Times(1);

	WPADWrapperExpansion expansion;
	expansion.type = WPAD_WRAPPER_EXP_NONE;
	EXPECT_CALL(mWPADWrapperMock, GetExpansion(Eq(connectedChannel), _)).WillOnce(SetArgPointee<1>(expansion));

	EXPECT_CALL(mWPADWrapperMock, SetVRes(Eq(connectedChannel), Eq(ApplicationMock::WIDTH), Eq(ApplicationMock::HEIGHT))).Times(1);

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	pWiiInputSystem->DiscoverDevices();

	EXPECT_EQ(1, pWiiInputSystem->GetDevicesCount());

	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_add_new_extension_when_a_nunchuk_is_plugged_to_a_connected_wii_mote)
{
	UInt connectedChannel = 0;

	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(connectedChannel), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR_NONE));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(1), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(2), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(3), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, SetDataFormat(Eq(connectedChannel), Eq(WPAD_WRAPPER_FMT_BTNS_ACC_IR))).Times(1);

	WPADWrapperExpansion expansion1, expansion2;
	expansion1.type = WPAD_WRAPPER_EXP_NONE;
	expansion2.type = WPAD_WRAPPER_EXP_NUNCHUK;
	EXPECT_CALL(mWPADWrapperMock, GetExpansion(Eq(connectedChannel), _)).Times(2).WillOnce(SetArgPointee<1>(expansion1)).WillOnce(SetArgPointee<1>(expansion2));

	EXPECT_CALL(mWPADWrapperMock, SetVRes(Eq(connectedChannel), Eq(ApplicationMock::WIDTH), Eq(ApplicationMock::HEIGHT))).Times(2);

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	pWiiInputSystem->DiscoverDevices();
	pWiiInputSystem->DiscoverDevices();

	EXPECT_EQ(2, pWiiInputSystem->GetDevicesCount());
	EXPECT_EQ(1, pWiiInputSystem->GetMainDevicesCount());
	EXPECT_EQ(1, pWiiInputSystem->GetDeviceExtensionsCount());

	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_remove_device_when_a_wii_mote_is_disconnected_from_a_channel)
{
	UInt connectedChannel = 0;

	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(connectedChannel), _)).Times(2).WillOnce(Return(WPAD_WRAPPER_ERR_NONE)).WillOnce(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(1), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(2), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(3), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, SetDataFormat(Eq(connectedChannel), Eq(WPAD_WRAPPER_FMT_BTNS_ACC_IR))).Times(1);

	WPADWrapperExpansion expansion;
	expansion.type = WPAD_WRAPPER_EXP_NONE;
	EXPECT_CALL(mWPADWrapperMock, GetExpansion(Eq(connectedChannel), _)).WillOnce(SetArgPointee<1>(expansion));

	EXPECT_CALL(mWPADWrapperMock, SetVRes(Eq(connectedChannel), Eq(ApplicationMock::WIDTH), Eq(ApplicationMock::HEIGHT))).Times(1);

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	pWiiInputSystem->DiscoverDevices();

	EXPECT_EQ(1, pWiiInputSystem->GetDevicesCount());

	pWiiInputSystem->DiscoverDevices();

	EXPECT_EQ(0, pWiiInputSystem->GetDevicesCount());

	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_remove_an_extension_when_a_nunchuk_is_unplugged_from_a_connected_wii_mote)
{
	UInt connectedChannel = 0;

	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(connectedChannel), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR_NONE));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(1), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(2), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(3), _)).Times(2).WillRepeatedly(Return(WPAD_WRAPPER_ERR));
	EXPECT_CALL(mWPADWrapperMock, SetDataFormat(Eq(connectedChannel), Eq(WPAD_WRAPPER_FMT_BTNS_ACC_IR))).Times(1);

	WPADWrapperExpansion expansion1, expansion2;
	expansion1.type = WPAD_WRAPPER_EXP_NUNCHUK;
	expansion2.type = WPAD_WRAPPER_EXP_NONE;
	EXPECT_CALL(mWPADWrapperMock, GetExpansion(Eq(connectedChannel), _)).Times(2).WillOnce(SetArgPointee<1>(expansion1)).WillOnce(SetArgPointee<1>(expansion2));

	EXPECT_CALL(mWPADWrapperMock, SetVRes(Eq(connectedChannel), Eq(ApplicationMock::WIDTH), Eq(ApplicationMock::HEIGHT))).Times(2);

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	pWiiInputSystem->DiscoverDevices();

	EXPECT_EQ(2, pWiiInputSystem->GetDevicesCount());
	EXPECT_EQ(1, pWiiInputSystem->GetMainDevicesCount());
	EXPECT_EQ(1, pWiiInputSystem->GetDeviceExtensionsCount());

	pWiiInputSystem->DiscoverDevices();

	EXPECT_EQ(1, pWiiInputSystem->GetDevicesCount());
	EXPECT_EQ(1, pWiiInputSystem->GetMainDevicesCount());
	EXPECT_EQ(0, pWiiInputSystem->GetDeviceExtensionsCount());

	WIRE_DELETE pWiiInputSystem;
}

TEST_F(WiiInputSystemTest, should_detect_a_channel_connection_error)
{
	EXPECT_CALL(mWPADWrapperMock, Init()).Times(1);
	EXPECT_CALL(mWPADWrapperMock, Shutdown()).Times(1);

	EXPECT_CALL(mWPADWrapperMock, Probe(Eq(0), _)).WillOnce(Return(WPAD_WRAPPER_ERR));

	// ---

	WiiInputSystem* pWiiInputSystem = WIRE_NEW WiiInputSystem();
	Bool isCorrectlyConnected = pWiiInputSystem->IsChannelCorrectlyConnected(0);

	EXPECT_FALSE(isCorrectlyConnected);

	WIRE_DELETE pWiiInputSystem;
}

}
