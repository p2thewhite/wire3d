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
