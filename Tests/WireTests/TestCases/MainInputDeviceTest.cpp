#include "gtest/gtest.h"

#include "../Stubs/WireMainInputDeviceStub.h"
#include "../Stubs/WireInputDeviceExtensionStub.h"
#include "../Stubs/WireInputCapabilityStub.h"

namespace Wire
{

class MainInputDeviceTest : public ::testing::Test
{
protected:
	MainInputDeviceStub mMainInputDevice;
	InputDeviceExtensionPtr mInputDeviceExtension;

	MainInputDeviceTest()
	{
	}

	virtual ~MainInputDeviceTest()
	{
	}

	virtual void SetUp()
	{
		mInputDeviceExtension = new InputDeviceExtensionStub(&mMainInputDevice);
		mInputDeviceExtension->SetUp();
		mMainInputDevice.AddExtension(mInputDeviceExtension);
	}

	virtual void TearDown()
	{
	}

};

TEST_F(MainInputDeviceTest, should_consider_extensions_capabilities_as_his_own_capabilities)
{
	ASSERT_TRUE(mMainInputDevice.HasCapability(InputCapabilityStub::TYPE, true));
}

TEST_F(MainInputDeviceTest, should_get_extension_by_type)
{
	const InputCapability* pInputCapability = mMainInputDevice.GetCapability(InputCapabilityStub::TYPE, true);
	ASSERT_TRUE(pInputCapability != NULL);
}

}
