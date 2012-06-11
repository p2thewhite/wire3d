// Sample11 - Input

#include "Sample11.h"

#include "Importer.h"
#include "Cursors.h"
#include "WireInputSystem.h"
#include "WireVirtualInputDevice.h"
#include "WireInputCapability.h"
//#include "WireTHashSet.h"
#include <set>
#include <algorithm>

using namespace Wire;
using namespace std;

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

	GetInputSystem()->GetDevicesDiscoveryStrategy()->AddInputDeviceDiscoveryListener(this);

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

	// TODO: MAGIC NUMBER = Maximum number of players!
	UInt playerCount = min<UInt>(GetInputSystem()->GetInputDevicesCount(), 4);
	for (UInt playerNo = 0; playerNo < playerCount; playerNo++) 
	{
		const VirtualInputDevice* pVirtualInputDevice = GetInputSystem()->GetInputDevice(playerNo);

		// Wire3D uses the OpenGL convention of (0,0) being at the bottom left corner of the screen, that's why
		// we make a vertical correction here!
		Float x = pVirtualInputDevice->GetIRAxis(IR_X);
		Float y = screenHeight - pVirtualInputDevice->GetIRAxis(IR_Y);

		CursorMode mode;
		if (pVirtualInputDevice->GetButton(BUTTON_A)) {
			mode = CM_PRIMARY_BUTTON_PRESSED;
		} else if (pVirtualInputDevice->GetButton(BUTTON_B)) {
			mode = CM_SECONDARY_BUTTON_PRESSED;
		} else {
			mode = CM_POINTING;
		}

		SetCursor(x, y, mode, playerNo, 0);
	}

	mspCursors->UpdateGS(time);
	mCuller.ComputeVisibleSet(mspCursors);

	if (mInputDevicesStateChanged)
	{
		PrintInputDevicesInformation();
		mInputDevicesStateChanged = false;
	}

	PrintKeyStates();

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspGuiCamera);
	GetRenderer()->DrawScene(mCuller.GetVisibleSets());
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
void Sample11::Update()
{
	mInputDevicesStateChanged = true;
}

//----------------------------------------------------------------------------
void Sample11::SetCursor(Float x, Float y, CursorMode mode, UInt playerNo, Float zRollInRadian)
{
	if (playerNo >= mspCursors->GetQuantity())
	{
		WIRE_ASSERT(false /* player number is out of bounds */);
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
void Sample11::PrintInputDevicesInformation()
{
	stringstream message;

	message << "No. of Input Devices: " << mpInputSystem->GetInputDevicesCount() << "\n";
	PrintAndClear(message);

	for (UInt i = 0; i < mpInputSystem->GetInputDevicesCount(); i++)
	{
		const VirtualInputDevice* pVirtualInputDevice = mpInputSystem->GetInputDevice(i);

		message << "Input Device " << i << "\n";
		PrintAndClear(message);

		message << "- Name " << pVirtualInputDevice->GetName() << "\n";
		PrintAndClear(message);

		System::Print("- Capabilities:\n");

		/*const THashSet<InputCapability>& capabilities = pVirtualInputDevice->GetCapabilities();
		THashSet<InputCapability>::Iterator iterator(&capabilities);
		InputCapability* pCapability;
		while ((pCapability = iterator.GetNext()) != NULL)
		{
			System::Print(GetInputCapabilityName(*pCapability));
		}*/
		const set<InputCapability>& capabilities = pVirtualInputDevice->GetCapabilities();
		set<InputCapability>::const_iterator iterator = capabilities.begin();
		while (iterator != capabilities.end())
		{
			message << GetInputCapabilityName(*iterator) << "\n";
			PrintAndClear(message);
			iterator++;
		}
	}
}

//----------------------------------------------------------------------------
void Sample11::PrintKeyStates()
{
	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_A))
	{
		System::Print("Button 'A' Pressed\n");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_B))
	{
		System::Print("Button 'B' Pressed\n");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_1))
	{
		System::Print("Button '1' Pressed\n");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_2))
	{
		System::Print("Button '2' Pressed\n");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_MINUS))
	{
		System::Print("Button 'MINUS' Pressed\n");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_PLUS))
	{
		System::Print("Button 'PLUS' Pressed\n");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_Z))
	{
		System::Print("Button 'Z' Pressed\n");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_C))
	{
		System::Print("Button 'C' Pressed\n");
	}
}
