#pragma once
#ifndef WIREOGCDEFINES_H
#define WIREOGCDEFINES_H
#include <gctypes.h>
#include "ogc/gu.h"

#define GX_TEXCOORD_NULL GX_TEXCOORDNULL

#define GXBegin GX_Begin
#define GXClearVtxDesc GX_ClearVtxDesc
#define GXColor1x8 GX_Color1x8
#define GXCopyDisp GX_CopyDisp
#define GXDrawDone GX_DrawDone
#define GXEnd GX_End
#define GXGetYScaleFactor GX_GetYScaleFactor
#define GXInit GX_Init
#define GXInitLightColor GX_InitLightColor
#define GXInitLightPos GX_InitLightPos
#define GXInvalidateTexAll GX_InvalidateTexAll
#define GXInvalidateVtxCache GX_InvVtxCache
#define GXLoadPosMtxImm GX_LoadPosMtxImm
#define GXPosition1x8 GX_Position1x8
#define GXPosition3f32 GX_Position3f32
#define GXRenderModeObj GXRModeObj
#define GXSetArray GX_SetArray
#define GXSetColorUpdate GX_SetColorUpdate
#define GXSetCopyClear GX_SetCopyClear
#define GXSetCopyFilter GX_SetCopyFilter
#define GXSetCullMode GX_SetCullMode
#define GXSetDispCopyDst GX_SetDispCopyDst
#define GXSetDispCopyGamma GX_SetDispCopyGamma
#define GXSetDispCopySrc GX_SetDispCopySrc
#define GXSetDispCopyYScale GX_SetDispCopyYScale
#define GXSetFieldMode GX_SetFieldMode
#define GXSetNumChans GX_SetNumChans
#define GXSetNumTevStages GX_SetNumTevStages
#define GXSetNumTexGens GX_SetNumTexGens
#define GXSetPixelFmt GX_SetPixelFmt
#define GXSetProjection GX_LoadProjectionMtx
#define GXSetScissor GX_SetScissor
#define GXSetViewport GX_SetViewport
#define GXSetViewportJitter GX_SetViewportJitter
#define GXSetVtxAttrFmt GX_SetVtxAttrFmt
#define GXSetVtxDesc GX_SetVtxDesc
#define GXSetTevOp GX_SetTevOp
#define GXSetTevOrder GX_SetTevOrder
#define GXSetZMode GX_SetZMode

#define MTXConcat guMtxConcat
#define MTXFrustum guFrustum
#define MTXIdentity guMtxIdentity
#define MTXLookAt guLookAt
#define MTXMultVec guVecMultiply
#define MTXPerspective guPerspective
#define MTXRotAxisRad guMtxRotAxisRad
#define MTXTrans guMtxTrans
#define VECAdd guVecAdd
#define VECScale guVecScale
#define VECSubtract guVecSub
#define Vec Vector

#define PADInit WPAD_Init

#define VIConfigure VIDEO_Configure
#define VIFlush VIDEO_Flush
#define VIGetNextField VIDEO_GetNextField
#define VIInit VIDEO_Init
#define VISetBlack VIDEO_SetBlack
#define VISetNextFrameBuffer VIDEO_SetNextFramebuffer
#define VIWaitForRetrace VIDEO_WaitVSync

#endif /* WIREOGCDEFINES_H */
