// Sample11 - Input

#include "Sample11.h"

#include "Importer.h"
#include "Cursors.h"
#include "WireInputSystem.h"
#include "WireInputCapability.h"
//#include "WireTHashSet.h"
#include <set>

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

	// working vars for demonstration only, make the cursor position pop
	// once one of them reaches the screen border
	static Int xPos = 0;
	static Int yPos = 0;
	static Float zRoll = 0;
	xPos++;
	yPos++;
	zRoll += 0.02F;
	xPos = xPos > screenWidth ? 0 : xPos;
	yPos = yPos > screenHeight ? 0 : yPos;
	zRoll = zRoll > MathF::TWO_PI ? 0 : zRoll;

	// Wire3D uses the OpenGL convention of (0,0) being at the bottom left
	// corner of the screen.
	SetCursor(xPos, yPos, CM_POINTING, 0 /* player 0 (generic hand */, zRoll);
	SetCursor(xPos / 2, yPos / 2, CM_SECONDARY_BUTTON_PRESSED, /* player no */ 4);

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
void Sample11::SetCursor(Int x, Int y, CursorMode mode, UInt playerNo, Float zRollInRadian)
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

	Float xf = static_cast<Float>(x);
	Float yf = static_cast<Float>(y);
	Vector3F pos(xf, yf, 0);
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

	message << "No. of Input Devices: " << mpInputSystem->GetInputDevicesCount();
	PrintAndClear(message);

	for (UInt i = 0; i < mpInputSystem->GetInputDevicesCount(); i++)
	{
		const VirtualInputDevice* pVirtualInputDevice = mpInputSystem->GetInputDevice(i);

		message << "Input Device " << i;
		PrintAndClear(message);

		message << "- Name " << pVirtualInputDevice->GetName();
		PrintAndClear(message);

		System::Print("- Capabilities: ");

		/*const THashSet<InputCapability>& capabilities = pVirtualInputDevice->GetCapabilities();
		THashSet<InputCapability>::Iterator iterator(&capabilities);
		InputCapability* pCapability;
		while ((pCapability = iterator.GetNext()) != NULL)
		{
			System::Print(GetInputCapabilityName(*pCapability));
		}*/
		const set<InputCapability>& capabilities = pVirtualInputDevice->GetCapabilities();
		set<InputCapability>::iterator iterator = capabilities.begin();
		while (iterator != capabilities.end())
		{
			System::Print(GetInputCapabilityName(*iterator));
			iterator++;
		}
	}
}

//----------------------------------------------------------------------------
void Sample11::PrintKeyStates()
{
	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_A))
	{
		System::Print("Button 'A' Pressed");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_B))
	{
		System::Print("Button 'B' Pressed");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_1))
	{
		System::Print("Button '1' Pressed");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_2))
	{
		System::Print("Button '2' Pressed");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_MINUS))
	{
		System::Print("Button 'MINUS' Pressed");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_PLUS))
	{
		System::Print("Button 'PLUS' Pressed");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_Z))
	{
		System::Print("Button 'Z' Pressed");
	}

	if (mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_C))
	{
		System::Print("Button 'C' Pressed");
	}
}
