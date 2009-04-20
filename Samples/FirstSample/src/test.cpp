#include "DemoInit.h"

DemoInit DEMO;

using namespace Wire;

void CameraInit(Matrix34f& rView);
void DrawInit();
void DrawPyramid(float rtri, float scaleFactor, Matrix34f& view);
void DrawCube(float rquad, float scaleFactor, Matrix34f& view);

//---------------------------------------------------------------------------------
int main( int argc, char **argv )
//---------------------------------------------------------------------------------
{
	Matrix34f view;

	float rtri = 0.0f , rquad = 0.0f;

	DEMO.Init();
 	CameraInit(view);
	DrawInit();

	float angle = 0.0f;

	WPAD_ScanPads();

	while(!(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME))
	{
		float scaleFactor = Mathf::Sin(angle);
		angle += M_PI / 180.0f;
		angle = Mathf::FMod(angle, M_PI);

		DEMO.BeforeRender();

		DrawPyramid(rtri, scaleFactor, view);
		DrawCube(rquad, scaleFactor, view);

		DEMO.DoneRender();

		rtri+=0.5f;				// Increase The Rotation Variable For The Triangle ( NEW )
		rquad-=0.15f;			// Decrease The Rotation Variable For The Quad     ( NEW )

		WPAD_ScanPads();
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

	GXSetCullMode(GX_CULL_NONE);

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
	GXSetCullMode(GX_CULL_BACK);

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

//-------------------------------------------------------------------------
void CameraInit(Matrix34f& rView)
{
	// setup our camera at the origin
	// looking down the -z axis with y up
	Vector3f cam(0.0f, 0.0f, 0.0f);
	Vector3f up(0.0f, 1.0f, 0.0f);
	Vector3f look(0.0f, 0.0f, -1.0f);

	// die casterei hier bleibt natürlich nicht so.
	MTXLookAt(rView,
		reinterpret_cast<Vector*>(static_cast<float*>(cam)), 
		reinterpret_cast<Vector*>(static_cast<float*>(up)), 
		reinterpret_cast<Vector*>(static_cast<float*>(look)));

	// setup our projection matrix
	// this creates a perspective matrix with a view angle of 90,
	// and aspect ratio based on the display resolution
	f32 w = DEMO.GetRenderMode()->viWidth;
	f32 h = DEMO.GetRenderMode()->viHeight;
	Matrix4f perspective;
	MTXPerspective(perspective, 45, (f32)w/h, 0.1F, 300.0F);
	GXSetProjection(perspective, GX_PERSPECTIVE);
}

//-------------------------------------------------------------------------
void DrawInit()
{
	// clears the bg to color and clears the z buffer
	GXColor background = {0, 0, 0, 0xff};
	GXSetCopyClear(background, GX_MAX_Z24);
}
