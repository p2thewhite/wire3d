#include "Sample4.h"
#include "LensflareNode.h"

using namespace Wire;

WIRE_APPLICATION(Sample4);

//----------------------------------------------------------------------------
Sample4::Sample4()
{
}

//----------------------------------------------------------------------------
Bool Sample4::OnInitialize()
{
	// This function is called by the framework before the rendering loop
	// starts. Put all you initializations here.

	// The platform dependent part of the application might need to do some
	// initialization. If it fails, we return false.
	if (!Parent::OnInitialize())
	{
		return false;
	}

	// We create a cube here and reference it using a smart pointer.
	// The smart pointer automatically deletes the object when it goes out
	// of scope and no other references to the object exist. In this case
	// deletion will happen when Sample4 is being destructed.
	mspCube = CreateCube();

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, 0.0F, 3.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);
	mCuller.SetCamera(mspCamera);

	// We render some of the cubes using transparency and frontface culling,
	// so we create the required render state objects here.
	mspCullState = WIRE_NEW CullState;
	mspAlphaState = WIRE_NEW AlphaState;

	// Initialize working variables used in the render loop (i.e. OnIdle()).
	mAngle = 0.0F;
	mLastTime = System::GetTime();

	return true;
}

//----------------------------------------------------------------------------
void Sample4::OnIdle()
{
	// This function is called by the framework's render loop until the
	// application exits. Perform all your rendering here.

	// Determine how much time has passed since the last call, so we can
	// move our objects independently of the actual frame rate.
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	// If the camera's viewing frustum changed, we need to update the culler
	// (we know we don't change it, so it's commented out here)
	//	mCuller.SetFrustum(mspCamera->GetFrustum());

	// Clear the framebuffer and the z-buffer.
	GetRenderer()->ClearBuffers();

	// Tell the Renderer that we want to start drawing.
	GetRenderer()->PreDraw(mspCamera);

	// We set the render state to backface culling and disable alpha blending.
	// NOTE: if you are not using the scenegraph to handle render states for
	// you, it is your responsibility to handle states between draw calls.
	mspCullState->CullFace = CullState::CM_BACK;
	GetRenderer()->SetState(mspCullState);

	mspAlphaState->BlendEnabled = false;
	GetRenderer()->SetState(mspAlphaState);

	// Set world position (translate) and orientation (rotate)
	Matrix34F model(Matrix34F::IDENTITY);
	mspCube->World.SetRotate(model);
	mspCube->World.SetTranslate(Vector3F::ZERO);

	// After setting world transformation, we update the world bounding
	// volume of the cube so we can cull it against the viewing frustum.
	// That way we only draw objects that are visible on the screen.
	mspCube->UpdateWorldBound();
	if (mCuller.IsVisible(mspCube))
	{
		GetRenderer()->Draw(mspCube);
	}

	// Tell the Renderer that we are done with drawing
	GetRenderer()->PostDraw();

	// Present the framebuffer (aka backbuffer) on the screen
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample4::CreateCube()
{
	// Create a cube with unique texture (UV) coordinates for each side.
	// This means we have to duplicate vertices, since every vertex can only
	// have one UV coordinate per texture. Thus resulting in 24 (4 vertices
	// times 6 sides of the cube) vertices.
	const Float extent = 1.0F;
	const Vector3F vertices[] =
	{
		// side 1
		Vector3F(-extent,  extent,  extent),
		Vector3F( extent,  extent,  extent),
		Vector3F( extent, -extent,  extent),
		Vector3F(-extent, -extent,  extent),
		// side 2
		Vector3F( extent,  extent, -extent),
		Vector3F( extent,  extent,  extent),
		Vector3F(-extent,  extent,  extent),
		Vector3F(-extent,  extent, -extent),
		// side 3
		Vector3F( extent, -extent,  extent),
		Vector3F( extent,  extent,  extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent, -extent, -extent),
		// side 4
		Vector3F(-extent,  extent, -extent),
		Vector3F( extent,  extent, -extent),
		Vector3F( extent, -extent, -extent),
		Vector3F(-extent, -extent, -extent),
		// side 5
		Vector3F( extent, -extent, -extent),
		Vector3F( extent, -extent,  extent),
		Vector3F(-extent, -extent,  extent),
		Vector3F(-extent, -extent, -extent),
		// side 6
		Vector3F(-extent, -extent,  extent),
		Vector3F(-extent,  extent,  extent),
		Vector3F(-extent,  extent, -extent),
		Vector3F(-extent, -extent, -extent)
	};

	// Texture coordinates
	const Float extentUv = 1.0F;
	const Vector2F uvs[] =
	{
		// side 1
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 2
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 3
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 4
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 5
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv),
		// side 6
		Vector2F(0.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 0.0F * extentUv),
		Vector2F(1.0F * extentUv, 1.0F * extentUv),
		Vector2F(0.0F * extentUv, 1.0F * extentUv)
	};

	// Indices provide connectivity information and define the triangle mesh.
	// Every side of the cube consists of 2 triangles.
	const UInt indices[] =
	{
		// side 1
		0, 2, 1,
		0, 3, 2,
		// side 2
		4, 6, 5,
		4, 7, 6,
		// side 3
		8, 10, 9,
		8, 11, 10,
		// side 4
		12, 13, 14,
		12, 14, 15,
		// side 5
		16, 17, 18,
		16, 18, 19,
		// side 6
		20, 21, 22,
		20, 22, 23
	};

	// Before creating the VertexBuffer we need to define its format.
	// It consists of 3d positions and 2d texture coordinates
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetTCoordChannels(2);	// channels: U, V

	// Now with the attributes being defined, we can create a VertexBuffer
	// and fill it with data.
	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);
	WIRE_ASSERT(vertexQuantity == (sizeof(uvs) / sizeof(Vector2F)));
	VertexBuffer* pCubeVerts = WIRE_NEW VertexBuffer(attributes,
		vertexQuantity);

	for (UInt i = 0; i < pCubeVerts->GetVertexQuantity(); i++)
	{
		pCubeVerts->Position3(i) = vertices[i];
		pCubeVerts->TCoord2(i) = uvs[i];
	}

	// Same for the IndexBuffer
	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

	// Geometric objects consist of a Vertex- and IndexBuffer.
	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);

	// The cube shall be textured. Therefore we create and attach a texture
	// effect, where we add a texture and define its blending mode.
	TextureEffect* pTextureEffect = WIRE_NEW TextureEffect;
	Texture2D* pTexture = CreateTexture();
	pTextureEffect->Textures.Append(pTexture);
	pTextureEffect->BlendOps.Append(TextureEffect::BM_REPLACE);
	pCube->AttachEffect(pTextureEffect);

	// NOTE: Geometry takes ownership over Vertex- and IndexBuffers using
	// smart pointers. Thus, you can share these buffers amongst Geometry 
	// objects without having to worry about deletion. Same applies to
	// Effects, Textures and Images.

	return pCube;
}

//----------------------------------------------------------------------------
Texture2D* Sample4::CreateTexture()
{
	const Image2D::FormatMode format = Image2D::FM_RGBA8888;
	const UInt bpp = Image2D::GetBytesPerPixel(format);
	const UInt width = 256;
	const UInt height = 256;

	const Float r = MathF::Min(static_cast<Float>(width), static_cast<Float>(
		height)) * 0.5F;

	UChar* const pDst = WIRE_NEW UChar[width * height * bpp];
	for (UInt y = 0; y < height; y++)
	{
		for (UInt x = 0; x < width; x++)
		{
			Float dx = r-x;
			Float dy = r-y;
			Float r1 = MathF::Sqrt(dx*dx + dy*dy) / r;

// replace the if-statement with c *= 1-smoothstep(1-.01, 1+.01, r);

// 			Float c = 1-r1;
// 			c = c*c;
// 			c *= 1 - SmoothStep(1-0.01F, 1+0.01F, r1);

// 			Float c = r1*r1;
// 			c = c*c;
// 			c = c*c*c;
// 			c *= 1 - SmoothStep(1-0.01F, 1+0.01F, r1);

// 			Float c = r1;
// 			c *= 1 - SmoothStep(1-0.01F, 1+0.01F, r1);

			Float c = 1-MathF::FAbs(r1 - 0.9F) / 0.1F;
			if (c < 0)
			{
				c = 0;
			}
			else
			{
				c = c*c;
				c = c*c;
			}

			UChar alpha = static_cast<UChar>(c * 255.0F);

			pDst[(y*width + x)*4] = alpha;	// R
			pDst[(y*width + x)*4+1] = alpha;	// G
			pDst[(y*width + x)*4+2] = alpha;	// B
			pDst[(y*width + x)*4+3] = alpha;
		}
	}




// 	Float* const pTemp = WIRE_NEW Float[width*height];
// 	for (UInt i = 0; i < width*height; i++)
// 	{
// 		pTemp[i] = 0.0F;
// 	}
// 
// 	Random rand;
// 	for (UInt count = 0; count < 200; count++)
// 	{
// 		// pick a random direction
// 		Float angle = rand.GetFloat() * MathF::TWO_PI;
// 		Float dx = MathF::Cos(angle);
// 		Float dy = MathF::Sin(angle);
// 
// 		// push particle along this path
// 		Float fx = static_cast<Float>(width) * 0.5F;
// 		Float fy = static_cast<Float>(height) * 0.5F;
// 
// 		for (UInt step = 0; step < r; step++)
// 		{
// 			DrawParticle(pTemp, fx, fy, width);
// 			fx += dx;
// 			fy += dy;
// 		}
// 	}
// 
// 	// normalize
// 	Float max = 0;
// 	for (UInt i = 0; i < width*height; i++)
// 	{
// 		if (pTemp[i] > max)
// 		{
// 			max = pTemp[i];
// 		}
// 	}
// 
// 	max = 1/max;
// 	for (UInt i = 0; i < width*height; i++)
// 	{
// 		Float f = pTemp[i] * max * 512.0F;
// 		UChar val = static_cast<UChar>(MathF::Clamp(0, f, 255.0F));
// 		pDst[i*bpp] = val;
// 		pDst[i*bpp+1] = val;
// 		pDst[i*bpp+2] = val;
// 		pDst[i*bpp+3] = val;
// 	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);

	return pTexture;
}

//----------------------------------------------------------------------------
void Sample4::DrawParticle(Float* const pDst, Float fx, Float fy, UInt width)
{
	const Int partRadius = 10;
	
	for (Int y = -partRadius; y < partRadius; y++)
	{
		for (Int x = -partRadius; x < partRadius; x++)
		{
			Float r2 = static_cast<Float>(x*x + y*y);
			Float c = 1 - r2 / static_cast<Float>(partRadius*partRadius);
			c = c*c;
			c = c*c;
			pDst[(y+static_cast<Int>(fy))*width+ x+static_cast<Int>(fx)] += c;
		}
	}
}

//----------------------------------------------------------------------------
Float Sample4::SmoothStep(Float a, Float b, Float x)
{
	if (x < a)
	{
		return 0;
	}

	if (x >= b)
	{
		return 1;
	}

	x = (x-a) / (b-a);
	return (x*x) * (3-2*x);
}
