// Sample11 - Input

#include "Sample11.h"

using namespace std;

#define DEGREES_TO_RADIANS(x) (x / 180.0f) * 3.14f

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

	mspGuiCamera = WIRE_NEW Camera(/* isPerspective */false);
	mCuller.SetCamera(mspGuiCamera);
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
	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());
	mspGuiCamera->SetFrustum(0, screenWidth, 0, screenHeight, 0, 1);

	mspGuiRoot->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspGuiRoot);

	if (mInputDevicesStateChanged)
	{
		//PrintInputDevicesInformations();
		mInputDevicesStateChanged = false;
	}

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspGuiCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();

//	PrintInputDevicesInformations();
}

void Sample11::OnInput()
{
	// TODO: MAGIC NUMBER = maximum number of players!
	UInt playerCount = min<UInt>(GetInputSystem()->GetMainDevicesCount(), 4);

	// if there's no controller yet, exit
	if (playerCount == 0) 
	{
		return;
	}

	// if there's no button capability, exit
	if (!GetInputSystem()->GetMainDevice(0)->HasCapability(Buttons::TYPE, false))
	{
		return;
	}

	// closes the application if the home button is pressed
	if (static_cast<const Buttons*>(GetInputSystem()->GetMainDevice(0)->GetCapability(Buttons::TYPE, false))->GetButton(Buttons::BUTTON_HOME))
	{
		Close();
		return;
	}

	Float screenHeight = static_cast<Float>(GetRenderer()->GetHeight());
	Float screenWidth = static_cast<Float>(GetRenderer()->GetWidth());

	for (UInt playerIndex = 0; playerIndex < playerCount; playerIndex++) 
	{
		const MainInputDevice* pInputDevice = GetInputSystem()->GetMainDevice(playerIndex);

		Float x;
		Float y;

		if (pInputDevice->HasCapability(IR::TYPE, false))
		{
			const IR* pIR = static_cast<const IR*>(pInputDevice->GetCapability(IR::TYPE, false));

			// wire3D uses the OpenGL convention of (0,0) being at the bottom left corner of the screen, 
			// but we want the upper left corner so make a vertical correction here
			x = pIR->GetRight();
			y = screenHeight - pIR->GetUp();
		} 
		else
		{
			// if there's no IR, position the pointer at the center of the screen
			x = screenWidth / 2;
			y = screenHeight / 2;
		}
		
		Cursors::CursorMode mode = Cursors::CM_POINTING;
		const Buttons* pButtons = NULL;

		// if there are buttons in the main device, get them
		if (pInputDevice->HasCapability(Buttons::TYPE, false))
		{
			pButtons = static_cast<const Buttons*>(pInputDevice->GetCapability(Buttons::TYPE, false));
		}

		// if there are buttons and the 'A' button is pressed, change the cursor mode
		if (pButtons != NULL && pButtons->GetButton(Buttons::BUTTON_A))
		{
			mode = Cursors::CM_PRIMARY_BUTTON_PRESSED;
		}

		// if there's an extension, get its buttons
		if (pInputDevice->GetExtensionsCount() > 0)
		{
			if (pInputDevice->GetExtension(0)->HasCapability(Buttons::TYPE))
			{
				pButtons = static_cast<const Buttons*>(pInputDevice->GetExtension(0)->GetCapability(Buttons::TYPE));
			}
		}

		// if there are buttons and the 'Z' button is pressed, change the cursor mode
		if (pButtons != NULL && pButtons->GetButton(Buttons::BUTTON_Z))
		{
			mode = Cursors::CM_SECONDARY_BUTTON_PRESSED;
		}
		
		Float tilt = 0;

		// get the main device tilt (in degrees)
		if (pInputDevice->HasCapability(Tilt::TYPE, false))
		{
			const Tilt* pTilt = static_cast<const Tilt*>(pInputDevice->GetCapability(Tilt::TYPE, false));
			tilt = DEGREES_TO_RADIANS(pTilt->GetRight());
		}

		mpCursors->SetCursor(x, y, mode, playerIndex, tilt);
	}
}

//----------------------------------------------------------------------------
void Sample11::OnDevicesChange()
{
	mInputDevicesStateChanged = true;
}

//----------------------------------------------------------------------------
void Sample11::PrintAndClear(stringstream& message)
{
	System::Print(message.str().c_str());
	message.str("");
	message.clear();
}

//----------------------------------------------------------------------------
void Sample11::PrintInputDevicesInformations()
{
	stringstream message;

	message << "No. of Input Devices: " << mpInputSystem->GetDevicesCount() << endl;
	PrintAndClear(message);

	for (UInt i = 0; i < mpInputSystem->GetDevicesCount(); i++)
	{
		const InputDevice* pInputDevice = mpInputSystem->GetDevice(i);

		message << "Input Device no. " << i << endl;
		PrintAndClear(message);

		message << "- Name: " << pInputDevice->GetType().GetName() << endl;
		PrintAndClear(message);

		System::Print("- Capabilities:");

		const TArray<InputCapabilityPtr>& inputCapabilities = pInputDevice->GetCapabilities();
		for (UInt j = 0; j < inputCapabilities.GetQuantity(); j++)
		{
			message << inputCapabilities[j]->GetType().GetName() << endl;
			PrintAndClear(message);
		}
	}

	message << endl;
	PrintAndClear(message);
}
