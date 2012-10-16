// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void PdrVertexAttributes::Enable(Renderer* pRenderer)
{
	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();

	for (UInt i = 0; i < mDeclaration.GetQuantity(); i++)
	{
		GXSetVtxDesc(mDeclaration[i].Attr, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, mDeclaration[i].Attr, mDeclaration[i].
			CompCnt, mDeclaration[i].CompType, 0);
	}
}

//----------------------------------------------------------------------------
inline UInt PdrVertexAttributes::GetVertexSize() const
{
	return mVertexSize;
}
