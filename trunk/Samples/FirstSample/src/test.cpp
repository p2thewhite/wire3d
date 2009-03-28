/*---------------------------------------------------------------------------------

	nehe lesson 5 port to GX by WinterMute

---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#ifndef WIRE_WII
#define WIRE_WII
#endif
//#undef WIRE_WII

#include "../../../Wire/Foundation/WireFoundation.h"
#include "../../../Wire/Engine/WireEngine.h"

#define DEFAULT_FIFO_SIZE	(256*1024)
 
static void *frameBuffer[2] = { NULL, NULL};
GXRModeObj *rmode;

using namespace Wire;

void DrawPyramid(float rtri, float scaleFactor, Matrix34f& view);
void DrawCube(float rquad, float scaleFactor, Matrix34f& view);

//---------------------------------------------------------------------------------
int main( int argc, char **argv )
//---------------------------------------------------------------------------------
{
	f32 yscale;

	u32 xfbHeight;

	Matrix34f view;
	Matrix4f perspective;

	float rtri = 0.0f , rquad = 0.0f;

	u32	fb = 0; 	// initial framebuffer index
	GXColor background = {0, 0, 0, 0xff};


	// init the vi.
	VIDEO_Init();
	WPAD_Init();
 
	rmode = VIDEO_GetPreferredMode(NULL);
	
	// allocate 2 framebuffers for double buffering
	frameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	frameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	// setup the fifo and then init the flipper
	void *gp_fifo = NULL;
	gp_fifo = memalign(32,DEFAULT_FIFO_SIZE);
	memset(gp_fifo,0,DEFAULT_FIFO_SIZE);
 
	GXInit(gp_fifo,DEFAULT_FIFO_SIZE);
 
	// clears the bg to color and clears the z buffer
	GXSetCopyClear(background, GX_MAX_Z24);
 
	// other gx setup
	GXSetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GXGetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GXSetDispCopyYScale(yscale);
	GXSetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	GXSetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GXSetDispCopyDst(rmode->fbWidth,xfbHeight);
	GXSetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GXSetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));
 
	GXSetCullMode(GX_CULL_NONE);
	GXCopyDisp(frameBuffer[fb],GX_TRUE);
	GXSetDispCopyGamma(GX_GM_1_0);
 
	// setup our camera at the origin
	// looking down the -z axis with y up
	Vector3f cam(0.0f, 0.0f, 0.0f);
	Vector3f up(0.0f, 1.0f, 0.0f);
	Vector3f look(0.0f, 0.0f, -1.0f);

	// die casterei hier bleibt natürlich nicht so.
	MTXLookAt(view,
		reinterpret_cast<Vector*>(static_cast<float*>(cam)), 
		reinterpret_cast<Vector*>(static_cast<float*>(up)), 
		reinterpret_cast<Vector*>(static_cast<float*>(look)));
 
	// setup our projection matrix
	// this creates a perspective matrix with a view angle of 90,
	// and aspect ratio based on the display resolution
    f32 w = rmode->viWidth;
    f32 h = rmode->viHeight;
	MTXPerspective(perspective, 45, (f32)w/h, 0.1F, 300.0F);
	GXSetProjection(perspective, GX_PERSPECTIVE);

	float angle = 0.0f;

	while(1) {

		float scaleFactor = Mathf::Sin(angle);
		angle += M_PI / 180.0f;
		angle = Mathf::FMod(angle, M_PI);

		WPAD_ScanPads();

		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

		// do this before drawing
		GXSetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);


		DrawPyramid(rtri, scaleFactor, view);
		DrawCube(rquad, scaleFactor, view);

		// do this stuff after drawing
		GXDrawDone();
		
		fb ^= 1;		// flip framebuffer
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GXSetColorUpdate(GX_TRUE);
		GXCopyDisp(frameBuffer[fb],GX_TRUE);

		VIDEO_SetNextFramebuffer(frameBuffer[fb]);
 
		VIDEO_Flush();
 
		VIDEO_WaitVSync();

		rtri+=0.5f;				// Increase The Rotation Variable For The Triangle ( NEW )
		rquad-=0.15f;			// Decrease The Rotation Variable For The Quad     ( NEW )
	}

	return 0;
}

//-------------------------------------------------------------------------
void DrawPyramid(float rtri, float scaleFactor, Matrix34f& view)
{
	Matrix34f model, modelview;
	Transformation modelTransform;

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	// setup the vertex attribute table
	// describes the data
	// args: vat location 0-7, type of data, data format, size, scale
	// so for ex. in the first call we are sending position data with
	// 3 values X,Y,Z of size F32. scale sets the number of fractional
	// bits for non float data.
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);

	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	Vector3f triAxis = Vector3f(0, 1, 0);
	model.FromAxisAngle(triAxis, DegToRad(rtri));
	modelTransform.SetRotate(model);
	modelTransform.SetTranslate(Vector3f(-1.5f,0.0f,-6.0f));
	modelTransform.SetUniformScale(scaleFactor + 0.5f);
	modelTransform.GetTransformation(model);
	modelview = view * model;
	// load the modelview matrix into matrix memory
	GXLoadPosMtxImm(modelview, GX_PNMTX0);

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);		// Draw A Pyramid

	GXPosition3f32( 0.0f, 1.0f, 0.0f);		// Top of Triangle (front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GXPosition3f32(-1.0f,-1.0f, 1.0f);	// Left of Triangle (front)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
	GXPosition3f32( 1.0f,-1.0f, 1.0f);	// Right of Triangle (front)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue

	GXPosition3f32( 0.0f, 1.0f, 0.0f);		// Top of Triangle (Right)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GXPosition3f32( 1.0f,-1.0f, 1.0f);	// Left of Triangle (Right)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GXPosition3f32( 1.0f,-1.0f,-1.0f);	// Right of Triangle (Right)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green

	GXPosition3f32( 0.0f, 1.0f, 0.0f);		// Top of Triangle (Back)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GXPosition3f32(-1.0f,-1.0f,-1.0f);	// Left of Triangle (Back)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GXPosition3f32( 1.0f,-1.0f,-1.0f);	// Right of Triangle (Back)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green

	GXPosition3f32( 0.0f, 1.0f, 0.0f);		// Top of Triangle (Left)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GXPosition3f32(-1.0f,-1.0f,-1.0f);	// Left of Triangle (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GXPosition3f32(-1.0f,-1.0f, 1.0f);	// Right of Triangle (Left)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green

	GXEnd();
}

//-------------------------------------------------------------------------
void DrawCube(float rquad, float scaleFactor, Matrix34f& view)
{
	Matrix34f model, modelview;
	Transformation modelTransform;

	// setup the vertex descriptor
// 	GXClearVtxDesc();
// 	GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
// 	GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
// 
// 	// Position has 3 elements (x,y,z), each of type s16,
// 	// no fractional bits (integers)
// 	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
// 
// 	// Color 0 has 4 components (r, g, b, a), each component is 8b.
// 	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
// 
// 	// stride = 3 elements (x,y,z) each of type s16
// 	GXSetArray(GX_VA_POS, Verts_s16, 3*sizeof(s16));
// 	// stride = 4 elements (r,g,b,a) each of type u8
// 	GXSetArray(GX_VA_CLR0, Colors_rgba8, 4*sizeof(u8));
// 
// 	// Initialize lighting, texgen, and tev parameters
// 	GXSetNumChans(1); // default, color = vertex color
// 	GXSetNumTexGens(0); // no texture in this demo
// 	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
// 	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	// setup the vertex attribute table
	// describes the data
	// args: vat location 0-7, type of data, data format, size, scale
	// so for ex. in the first call we are sending position data with
	// 3 values X,Y,Z of size F32. scale sets the number of fractional
	// bits for non float data.
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);

	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	Vector3f cubeAxis(1, 1, 1);
	model.FromAxisAngle(cubeAxis, DegToRad(rquad));
	modelTransform.SetRotate(model);
	modelTransform.SetTranslate(Vector3f(1.5f,0.0f,-7.0f));
	modelTransform.SetScale(Vector3f(scaleFactor + 0.5f, 1.0f, 1.0f));
	modelTransform.GetTransformation(model);
	modelview = view * model;
	// load the modelview matrix into matrix memory
	GXLoadPosMtxImm(modelview, GX_PNMTX0);

	GXBegin(GX_QUADS, GX_VTXFMT0, 24);			// Draw a Cube

	GXPosition3f32( 1.0f, 1.0f,-1.0f);	// Top Left of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
	GXPosition3f32(-1.0f, 1.0f,-1.0f);	// Top Right of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
	GXPosition3f32(-1.0f, 1.0f, 1.0f);	// Bottom Right of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
	GXPosition3f32( 1.0f, 1.0f, 1.0f);		// Bottom Left of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green

	GXPosition3f32( 1.0f,-1.0f, 1.0f);	// Top Left of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange
	GXPosition3f32(-1.0f,-1.0f, 1.0f);	// Top Right of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange
	GXPosition3f32(-1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange
	GXPosition3f32( 1.0f,-1.0f,-1.0f);	// Bottom Left of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange

	GXPosition3f32( 1.0f, 1.0f, 1.0f);		// Top Right Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GXPosition3f32(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GXPosition3f32(-1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GXPosition3f32( 1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red

	GXPosition3f32( 1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow
	GXPosition3f32(-1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow
	GXPosition3f32(-1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow
	GXPosition3f32( 1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow

	GXPosition3f32(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GXPosition3f32(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GXPosition3f32(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GXPosition3f32(-1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue

	GXPosition3f32( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Right)
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet
	GXPosition3f32( 1.0f, 1.0f, 1.0f);		// Top Left Of The Quad (Right)
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet
	GXPosition3f32( 1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet
	GXPosition3f32( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Right)		
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet

	GXEnd();									// Done Drawing The Quad 
}
