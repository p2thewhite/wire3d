#include "Sample1.h"

using namespace Wire;

// This macro creates our user application.
WIRE_APPLICATION(Sample1);

//----------------------------------------------------------------------------
Bool Sample1::OnInitialize()
{
	// The platform dependent part of the application might need to do some
	// initialization. If it fails, we return false.
	if (!Parent::OnInitialize())
	{
		return false;
	}

	// We create a cube here and reference it using a smart pointer.
	// The smart pointer automatically deletes the object when it goes out
	// of scope and no other references to the object exist. In this case
	// deletion will happen when Sample1 is being destructed.
	mspCube = CreateCube();

	// Setup the position and orientation of the camera to look down
	// the -z axis with y up.
	Vector3F cameraLocation(0.0F, 0.0F, 10.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	// Providing a field of view (FOV) in degrees, aspect ratio,
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
void Sample1::OnIdle()
{
	// Determine how much time has passed since the last call, so we can
	// move our objects independently of the actual frame rate.
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;
	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	mCuller.SetFrustum(mspCamera->GetFrustum());

	// Clear the framebuffer and the z-buffer.
	mpRenderer->ClearBuffers();

	// Tell the Renderer that we are about to start drawing.
	mpRenderer->PreDraw(mspCamera);

	// We set the render state to backface culling and disable alpha blending.
	// NOTE: if you are not using the scenegraph to handle render states for
	// you, it is your responsibility to handle states between draw calls.
	mspCullState->CullFace = CullState::CM_BACK;
	GetRenderer()->SetCullState(mspCullState);
	mspAlphaState->BlendEnabled = false;
	GetRenderer()->SetAlphaState(mspAlphaState);

	// Draw the upper row of cubes.
	const UInt cubeCount = 5;
	const Float stride = 3.5F;
	const Float offset = cubeCount * -0.5F * stride + stride * 0.6F;
	Float z = MathF::Sin(mAngle) * 3.0F;

	for (UInt i = 0; i < cubeCount; i++)
	{
		// Set world position (translate) and orientation (rotate)
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), -mAngle - 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, 1.8F, z));

		// After setting world transformation, we update the world bounding
		// volume of the cube so we can cull it against the viewing frustum.
		// That way we only draw objects that are visible on the screen.
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			mpRenderer->Draw(mspCube);
		}
	}

	// Draw the lower row of cubes using alpha blending.
	// For correct transparency order, we draw the backfaces first and then
	// the transparent frontfaces.

	// To draw the backfaces, cull the frontfaces
	mspCullState->CullFace = CullState::CM_FRONT;
	GetRenderer()->SetCullState(mspCullState);

	z = MathF::Cos(mAngle) * 3.0F;
	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			mpRenderer->Draw(mspCube);
		}
	}

	// Cull the backfaces, i.e. draw the front faces using alpha blending
	mspCullState->CullFace = CullState::CM_BACK;
	GetRenderer()->SetCullState(mspCullState);
	mspAlphaState->BlendEnabled = true;
	GetRenderer()->SetAlphaState(mspAlphaState);

	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			mpRenderer->Draw(mspCube);
		}
	}

	// Tell the Renderer that we are done with drawing
	mpRenderer->PostDraw();

	// Present the framebuffer (aka backbuffer) on the screen
	mpRenderer->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample1::CreateCube()
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
	Texture2D* pTexture = CreateTexture2();
	pTextureEffect->Textures.Append(pTexture);
	pTextureEffect->BlendOps.Append(TextureEffect::BM_REPLACE);
	pCube->AttachEffect(pTextureEffect);

	// NOTE: Geometry takes ownership over Vertex- and IndexBuffers using
	// smart pointers. Thus, you can share these buffers amongst Geometry 
	// objects without having to worry about deletion. Same applies to
	// Effects and Textures.

	return pCube;
}

#include "Sample1.inl"

//----------------------------------------------------------------------------
Texture2D* Sample1::CreateTexture2()
{
	// Define the properties of the image to be used as a texture
	const UInt width = 256;
	const UInt height = 256;
	Image2D::FormatMode format = Image2D::FM_RGB888;
	const UInt bpp = Image2D::GetBytesPerPixel(format);
	UChar* const pDst = WIRE_NEW UChar[width * height * bpp];

	struct Cell
	{
		Vector2F point;
		ColorRGB color;
	};

	TArray<Cell> cells;
	Random random;
	for (UInt i = 0; i < 30; i++)
	{
		Cell cell;
		cell.point.X() = random.GetFloat() * width;
		cell.point.Y() = random.GetFloat() * height;
		cell.color.R() = random.GetFloat();
		cell.color.G() = random.GetFloat();
		cell.color.B() = random.GetFloat();

		cells.Append(cell);
	}

	for (UInt y = 0; y < height; y++)
	{
		for (UInt x = 0; x < width; x++)
		{
			Float minDist = MathF::MAX_REAL;
			Float min2Dist = MathF::MAX_REAL;
			UInt minIndex = 0;

			for (UInt i = 0; i < cells.GetQuantity(); i++)
			{
				Vector2F pos(static_cast<Float>(x), static_cast<Float>(y));
				
				// Handle tiling
				Vector2F vec = cells[i].point - pos;
 				vec.X() = MathF::FAbs(vec.X());
 				vec.Y() = MathF::FAbs(vec.Y());
 				vec.X() = vec.X() > width/2 ? width-vec.X() : vec.X();
 				vec.Y() = vec.Y() > height/2 ? height-vec.Y() : vec.Y();

				Float distance = vec.Length();

				if (minDist > distance)
				{
					min2Dist = minDist;
					minDist = distance;
					minIndex = i;
				}
				else if (min2Dist > distance)
				{
					min2Dist = distance;
				}
			}

			float factor = (min2Dist - minDist) + 20;
			ColorRGB color = cells[minIndex].color * factor;
			pDst[(y*width+x)*bpp] = static_cast<UChar>(color.R());
			pDst[(y*width+x)*bpp+1] = static_cast<UChar>(color.G());
			pDst[(y*width+x)*bpp+2] = static_cast<UChar>(color.B());
		}
	}

	Image2D* pImage = WIRE_NEW Image2D(format, width, height, pDst);
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);
	return pTexture;
}

//----------------------------------------------------------------------------
Texture2D* Sample1::CreateTexture()
{
	// Create a texture from raw RGB data.
	const UInt width = WireLogo.width;
	const UInt height = WireLogo.height;

	// We create a buffer, copy the RGB values and add alpha values to
	// the texture because we want to demonstrate alpha blending.
	UChar* const pDst = WIRE_NEW UChar[width * height * 4];
	const UChar* pSrc = WireLogo.pixel_data;
	for (UInt y = 0; y < height; y++)
	{
		for (UInt x = 0; x < width; x++)
		{
			pDst[(y*width + x)*4] = *pSrc++;	// R
			pDst[(y*width + x)*4+1] = *pSrc++;	// G
			pDst[(y*width + x)*4+2] = *pSrc++;	// B

			// The alpha value of the texel is calculated using the
			// texel's distance to the center of the texture, so that the
			// center is opaque and transparency increases with the texel's
			// distance from the center.
			Float distance = 255.0F - Vector2F(x-width*0.5F, y-height*0.5F).
				Length() * 4 + 150;
			distance = distance < 0 ? 0 : distance;
			UChar alpha = distance > 255 ? 255 : static_cast<UChar>(distance);
			pDst[(y*width + x)*4+3] = alpha;
		}
	}

	// Create an Image. Mipmaps are created automatically unless 'false' is
	// passed as the last argument.
	// NOTE: Image2D takes ownership of the supplied raw buffer.
	// Do NOT delete that buffer yourself. Image2D takes care of that.
	Image2D* pImage = WIRE_NEW Image2D(Image2D::FM_RGBA8888, width, height,
		pDst/*, false*/);

	// Create a Texture using this image. Images can be shared amongst
	// texture objects. So you can use different filter/wrap/anisotropy values
	// without the need to duplicate images.
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);

	// Use anisotropic filtering for texturing. Looks better, but lowers
	// performance.
	pTexture->SetAnisotropyValue(4.0F);

	return pTexture;
}
