//----------------------------------------------------------------------------
inline void GXRenderer::EndScene()
{
}

//----------------------------------------------------------------------------
inline void GXRenderer::SetWireframeState(WireframeState* pState)
{
	mspStates[GlobalState::WIREFRAME] = pState;
}
