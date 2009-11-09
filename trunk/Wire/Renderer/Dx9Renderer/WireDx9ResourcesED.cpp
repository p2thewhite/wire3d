#include "WireDx9Renderer.h"

#include "WireDx9Resources.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Dx9Renderer::OnEnableIBuffer(ResourceIdentifier* pID)
{
	// Bind the current index buffer.
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	msResult = mpDevice->SetIndices(pResource->ID);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnEnableVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);
	msResult = mpDevice->SetStreamSource(0, pResource->ID, 0,
		pResource->VertexSize);
	WIRE_ASSERT(SUCCEEDED(msResult));
	msResult = mpDevice->SetVertexDeclaration(pResource->Declaration);
	WIRE_ASSERT(SUCCEEDED(msResult));
}
