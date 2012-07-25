// Sample11 - Input
// This sample demonstrates how to handle inputs from Wii/PC devices.

#include "Sample11.h"

using namespace std;

WIRE_APPLICATION(Sample11);

//----------------------------------------------------------------------------
Sample11::Sample11()
	:
	mInputDevicesStateChanged(false),
	mpCursors(NULL)
{
}

//----------------------------------------------------------------------------
Bool Sample11::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	GetInputSystem()->AddListener(this);

	mpCursors = WIRE_NEW Cursors;
	mspGuiRoot = WIRE_NEW Node;
	mspGuiRoot->AttachChild(mpCursors->GetRoot());
	mspText = StandardMesh::CreateText();
	mspGuiRoot->AttachChild(mspText);

	mspGuiCamera = WIRE_NEW Camera(/* isPerspective */false);
	mCuller.SetCamera(mspGuiCamera);

	UpdateInputDevicesInformationText();
	return true;
}

//----------------------------------------------------------------------------
void Sample11::OnTerminate()
{
	if (mpCursors)
	{
		WIRE_DELETE mpCursors;
		mpCursors = NULL;
	}
}

//----------------------------------------------------------------------------
void Sample11::OnIdle()
{
	Double time = System::GetTime();

	// set the frustum for the text camera (screenWidth and screenHeight
	// could have been changed by the user resizing the window)
	mspGuiCamera->SetFrustum(0, GetWidthF(), 0, GetHeightF(), 0, 1);

	mspGuiRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspGuiRoot);

	if (mInputDevicesStateChanged)
	{
		UpdateInputDevicesInformationText();
		mInputDevicesStateChanged = false;
	}

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspGuiCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void Sample11::OnInput()
{
	UInt playerCount = GetInputSystem()->GetMainDevicesCount();
	playerCount = playerCount > 4 ? 4 : playerCount; // maximum of 4 players

	// if there's no controller yet, return
	if (playerCount == 0) 
	{
		return;
	}

	const Buttons* pButtons = DynamicCast<Buttons>(GetInputSystem()->
		GetMainDevice(0)->GetCapability(Buttons::TYPE, false));

	// if there's no button capability, return
	if (!pButtons)
	{
		return;
	}

	// close the application if the home button is pressed
	if (pButtons->GetButton(Buttons::BUTTON_HOME))
	{
		Close();
		return;
	}

	for (UInt playerIndex = 0; playerIndex < playerCount; playerIndex++) 
	{
		const MainInputDevice* pInputDevice = GetInputSystem()->GetMainDevice(playerIndex);
		// if there's no IR, position the pointer at the center of the screen
		Float x = GetWidthF() * 0.5F;
		Float y = GetHeightF() * 0.5F;

		const IR* pIR = DynamicCast<const IR>(pInputDevice->GetCapability(IR::TYPE, false));
		// Wire3D uses the OpenGL convention of (0,0) being at the bottom left corner of the screen, 
		// but we want the upper left corner so make a vertical correction here
		if (pIR)
		{
			x = pIR->GetRight();
			y = GetHeightF() - pIR->GetUp();
		}
		
		Cursors::CursorMode mode = Cursors::CM_POINTING;
		const Buttons* pButtons = NULL;

		// if there are buttons in the main device, get them
		pButtons = DynamicCast<const Buttons>(pInputDevice->GetCapability(Buttons::TYPE, false));

		// if there are buttons and the 'A' button is pressed, change the cursor mode
		if (pButtons && pButtons->GetButton(Buttons::BUTTON_A))
		{
			mode = Cursors::CM_PRIMARY_BUTTON_PRESSED;
		}

		// if 'B' button is pressed, change the cursor mode
		if (pButtons && pButtons->GetButton(Buttons::BUTTON_B))
		{
			mode = Cursors::CM_SECONDARY_BUTTON_PRESSED;
		}

		// if there's an extension, get its buttons
		if (pInputDevice->GetExtensionsCount() > 0)
		{
			pButtons = DynamicCast<const Buttons>(pInputDevice->
				GetExtension(0)->GetCapability(Buttons::TYPE));
			if (pButtons)
			{
				// query nunchuck to initiate recognition just for info text
				pButtons->GetButton(Buttons::BUTTON_Z);
			}
		}

		Float tilt = 0;
		// get the main device tilt (in degrees)
		const Tilt* pTilt = DynamicCast<const Tilt>(pInputDevice->GetCapability(Tilt::TYPE, false));
		if (pTilt)
		{
			tilt = MathF::DEG_TO_RAD * (pTilt->GetRight());
		}

		mpCursors->SetCursor(x, y, mode, playerIndex+1, tilt);
	}
}

//----------------------------------------------------------------------------
void Sample11::OnDevicesChange()
{
	mInputDevicesStateChanged = true;
}

//----------------------------------------------------------------------------
void Sample11::UpdateInputDevicesInformationText()
{
	// set to screen width (might change any time in window mode)
	mspText->SetLineWidth(GetWidthF());
	mspText->Clear();
	// Text uses OpenGL convention of (0,0) being left bottom of window
	mspText->SetPen(0, GetHeightF()-mspText->GetFontHeight()-32.0F);

	const UInt TextArraySize = 4000;
	Char text[TextArraySize];
	System::Sprintf(text, TextArraySize, "Number of Input Devices: %d\n",
		mpInputSystem->GetDevicesCount());
	mspText->Append(text);

	for (UInt i = 0; i < mpInputSystem->GetDevicesCount(); i++)
	{
		const InputDevice* pInputDevice = mpInputSystem->GetDevice(i);

		System::Sprintf(text, TextArraySize, "\nInput Device Number: %d "
			"(%s)\n- Capabilities:\n", i, pInputDevice->GetType().GetName());
		mspText->Append(text);

		const TArray<InputCapabilityPtr>& inputCapabilities = pInputDevice->
			GetCapabilities();
		for (UInt j = 0; j < inputCapabilities.GetQuantity(); j++)
		{
			System::Sprintf(text, TextArraySize, "%s\n",
				inputCapabilities[j]->GetType().GetName());
			mspText->Append(text);
		}
	}

	mspText->Update(GetRenderer());
}
