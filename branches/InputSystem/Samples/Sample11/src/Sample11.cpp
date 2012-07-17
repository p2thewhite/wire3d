// Sample11 - Input

#include "Sample11.h"

#include "Importer.h"
#include "Cursors.h"
#include <vector>
#include <algorithm>

using namespace Wire;
using namespace std;

#define DEGREES_TO_RADIANS(x) (x / 180.0f) * 3.14f

WIRE_APPLICATION (Sample11);

//----------------------------------------------------------------------------
Sample11::Sample11() :
		mInputDevicesStateChanged(false)
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

	InitCursors();

	mspGuiCamera = WIRE_NEW Camera(/* isPerspective */false);
	mCuller.SetCamera(mspGuiCamera);
	return true;
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

	mspCursors->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspCursors);

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
		else {
			// if there's no IR, position the pointer at the center of the screen
			x = screenWidth / 2;
			y = screenHeight / 2;
		}
		
		CursorMode mode = CM_POINTING;

		if (pInputDevice->HasCapability(Buttons::TYPE, false))
		{
			const Buttons* pButtons = static_cast<const Buttons*>(pInputDevice->GetCapability(Buttons::TYPE, false));
			
			// check the buttons and change the cursor mode properly
			if (pButtons->GetButton(Buttons::BUTTON_A)) {
				mode = CM_PRIMARY_BUTTON_PRESSED;
			}
		}

		if (pInputDevice->GetExtensionsCount() > 0)
		{
			if (pInputDevice->GetExtension(0)->HasCapability(Buttons::TYPE))
			{
				const Buttons* pButtons = static_cast<const Buttons*>(pInputDevice->GetExtension(0)->GetCapability(Buttons::TYPE));

				// check the buttons and change the cursor mode properly
				if (pButtons->GetButton(Buttons::BUTTON_Z)) {
					mode = CM_SECONDARY_BUTTON_PRESSED;
				}
			}
		}
		
		Float tilt;

		if (pInputDevice->HasCapability(Tilt::TYPE, false))
		{
			const Tilt* pTilt = static_cast<const Tilt*>(pInputDevice->GetCapability(Tilt::TYPE, false));
			tilt = DEGREES_TO_RADIANS(pTilt->GetRight());
		}
		else
		{
			tilt = 0;
		}

		SetCursor(x, y, mode, playerIndex, tilt);
	}
}

//----------------------------------------------------------------------------
void Sample11::OnDevicesChange()
{
	mInputDevicesStateChanged = true;
}

//----------------------------------------------------------------------------
void Sample11::SetCursor(Float x, Float y, CursorMode mode, UInt playerNo, Float zRollInRadian)
{
	if (playerNo >= mspCursors->GetQuantity())
	{
		WIRE_ASSERT(false /* playerIndex number is out of bounds */);
		return;
	}

	Node* pCursors = DynamicCast < Node > (mspCursors->GetChild(playerNo));
	WIRE_ASSERT(pCursors);
	WIRE_ASSERT(pCursors->GetQuantity() == 3);
	for (UInt i = 0; i < pCursors->GetQuantity(); i++)
	{
		pCursors->GetChild(i)->Culling = Spatial::CULL_ALWAYS;
	}

	switch (mode)
	{
	case CM_OFF:
		return;

	case CM_POINTING:
		pCursors->GetChild(0)->Culling = Spatial::CULL_NEVER;
		break;

	case CM_PRIMARY_BUTTON_PRESSED:
		pCursors->GetChild(1)->Culling = Spatial::CULL_NEVER;
		break;

	case CM_SECONDARY_BUTTON_PRESSED:
		pCursors->GetChild(2)->Culling = Spatial::CULL_NEVER;
		break;

	default:
		WIRE_ASSERT(false);
	}

	Vector3F pos(x, y, 0);
	pCursors->Local.SetTranslate(pos);
	Matrix3F roll(Vector3F::UNIT_Z, zRollInRadian);
	pCursors->Local.SetRotate(roll);
}

//----------------------------------------------------------------------------
void Sample11::InitCursors()
{
	mspCursors = WIRE_NEW Node;
	mspCursorsAlpha = WIRE_NEW StateAlpha();
	mspCursorsAlpha->BlendEnabled = true;
	mspCursors->AttachState(mspCursorsAlpha);

	Image2D* pImage = Importer::DecodePNG(Cursors::PNG, Cursors::SIZE, false);
	Texture2D * pTexture = WIRE_NEW	Texture2D(pImage);
	mspMaterial = WIRE_NEW	Material();
	mspMaterial->AddTexture(pTexture, Material::BM_REPLACE);

	Node * pPlayer0 = WIRE_NEW Node;
	mspCursors->AttachChild(pPlayer0);
	pPlayer0->AttachChild(CreateCursor(0.25F, 0.75F));
	pPlayer0->AttachChild(CreateCursor(0, 0.75F));
	pPlayer0->AttachChild(CreateCursor(0.5F, 0.75F));

	Node * pPlayer1 = WIRE_NEW Node;
	mspCursors->AttachChild(pPlayer1);
	pPlayer1->AttachChild(CreateCursor(0, 0.25F));
	pPlayer1->AttachChild(CreateCursor(0, 0));
	pPlayer1->AttachChild(CreateCursor(0, 0.5F));

	Node * pPlayer2 = WIRE_NEW Node;
	mspCursors->AttachChild(pPlayer2);
	pPlayer2->AttachChild(CreateCursor(0.25F, 0.25F));
	pPlayer2->AttachChild(CreateCursor(0.25F, 0));
	pPlayer2->AttachChild(CreateCursor(0.25F, 0.5F));

	Node * pPlayer3 = WIRE_NEW Node;
	mspCursors->AttachChild(pPlayer3);
	pPlayer3->AttachChild(CreateCursor(0.5F, 0.25F));
	pPlayer3->AttachChild(CreateCursor(0.5F, 0));
	pPlayer3->AttachChild(CreateCursor(0.5F, 0.5F));

	Node * pPlayer4 = WIRE_NEW Node;
	mspCursors->AttachChild(pPlayer4);
	pPlayer4->AttachChild(CreateCursor(0.75F, 0.25F));
	pPlayer4->AttachChild(CreateCursor(0.75F, 0));
	pPlayer4->AttachChild(CreateCursor(0.75F, 0.5F));

	mspCursors->UpdateRS();
}

//----------------------------------------------------------------------------
Geometry* Sample11::CreateCursor(Float uOffset, Float vOffset)
{
	Geometry* pCursor = StandardMesh::CreateQuad(0, 1, false, 32.0f);
	const Vector2F uvs[] =
	{ Vector2F(0 + uOffset, 0 + vOffset), Vector2F(0.25f + uOffset, 0 + vOffset), Vector2F(0.25f + uOffset, 0.25f + vOffset), Vector2F(0 + uOffset, 0.25f + vOffset) };

	VertexBuffer* pVBuffer = pCursor->GetMesh()->GetVertexBuffer();
	for (UInt i = 0; i < pVBuffer->GetQuantity(); i++)
	{
		// recenter to fingertip
		pVBuffer->Position3(i) -= Vector3F(-16.0F, 16.0F, 0);
		pVBuffer->TCoord2(i) = uvs[i];
	}

	pCursor->Culling = Spatial::CULL_ALWAYS;
	pCursor->SetMaterial(mspMaterial);

	return pCursor;
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

		const vector<const InputCapability*>& inputCapabilities = pInputDevice->GetCapabilities();
		vector<const InputCapability*>::const_iterator j = inputCapabilities.begin();
		while (j != inputCapabilities.end())
		{
			message << (*j)->GetType().GetName() << endl;
			PrintAndClear(message);
			j++;
		}
	}
}

