// This sample demonstrates how to create a transparent, textured cube and
// render it using different render states and a light.

#include "Sample1.h"

using namespace Wire;

// This macro creates our user application.
WIRE_APPLICATION(Sample1);

//----------------------------------------------------------------------------
Bool Sample1::OnInitialize()
{
	// This function is called by the framework before the rendering loop
	// starts. Put all you initializations here.

	// The platform dependent part of the application might need to do some
	// initialization. If it fails, we return false and the application exits.
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

	// By providing a field of view (FOV) in degrees, aspect ratio,
	// near and far plane, we define a viewing frustum used for culling.
	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);
	mCuller.SetCamera(mspCamera);

	// We render some of the cubes using transparency and front-face culling,
	// so we create the required render state objects here.
	mspCullState = WIRE_NEW CullState;
	mspAlphaState = WIRE_NEW AlphaState;

	// We render the top row of cubes with lighting on, so we create a light
	// and a material state for the cube, which defines how the light (color)
	// will be reflected.
	mspLight = WIRE_NEW Light;
	mspMaterialState = WIRE_NEW MaterialState;

	// Initialize working variables used in the render loop (i.e. OnIdle()).
	mAngle = 0.0F;
	mLastTime = System::GetTime();

	return true;
}

//----------------------------------------------------------------------------
void Sample1::OnIdle()
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

	// The light's color is white. We set the material of the cube so it
	// reduces the red and blue component a bit, resulting in a tint of green.
 	mspMaterialState->Ambient = ColorRGBA(0.9F, 1.0F, 0.9F, 1.0F);
 	GetRenderer()->SetState(mspMaterialState);

	// modulate the texture with the light
	mspTextureEffect->BlendOps[0] = TextureEffect::BM_MODULATE;

	GetRenderer()->EnableLighting();
	GetRenderer()->SetLight(mspLight);

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
			GetRenderer()->Draw(mspCube);
		}
	}

	// Draw the lower row of cubes using alpha blending.
	// For correct transparency order, we draw the backfaces first and then
	// the transparent frontfaces.

	// To draw the backfaces, cull the frontfaces
	mspCullState->CullFace = CullState::CM_FRONT;
	GetRenderer()->SetState(mspCullState);

	GetRenderer()->DisableLighting();

	// There is no more light, so the texture blending needs to be reset,
	// otherwise it will modulate undefined values.
	mspTextureEffect->BlendOps[0] = TextureEffect::BM_REPLACE;
	
	z = MathF::Cos(mAngle) * 3.0F;
	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			GetRenderer()->Draw(mspCube);
		}
	}

	// Cull the backfaces, i.e. draw the front faces using alpha blending
	mspCullState->CullFace = CullState::CM_BACK;
	GetRenderer()->SetState(mspCullState);
	mspAlphaState->BlendEnabled = true;
	GetRenderer()->SetState(mspAlphaState);

	for (UInt i = 0; i < cubeCount; i++)
	{
		Matrix34F model(Vector3F(0.75F, 0.25F, 0.5F), mAngle + 0.2F * i);
		mspCube->World.SetRotate(model);
		mspCube->World.SetTranslate(Vector3F(offset + stride * i, -1.8F, z));
		mspCube->UpdateWorldBound();
		if (mCuller.IsVisible(mspCube))
		{
			GetRenderer()->Draw(mspCube);
		}
	}

	// Tell the Renderer that we are done with drawing
	GetRenderer()->PostDraw();

	// Present the framebuffer (aka backbuffer) on the screen
	GetRenderer()->DisplayBackBuffer();
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
	// It consists of 3d positions, 2d texture coordinates and 3d normal
	// vectors. Normals are required for lighting.
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetTCoordChannels(2);	// channels: U, V
	attributes.SetNormalChannels(3);

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

		// We don't provide normals here, we generate them later.
//		pCubeVerts->Normal3(i) = normals[i];
	}

	// Fill the IndexBuffer with data.
	UInt indexQuantity = sizeof(indices) / sizeof(UInt);
	IndexBuffer* pIndices = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndices)[i] = indices[i];
	}

	// Geometric objects consist of a Vertex- and IndexBuffer.
	Geometry* pCube = WIRE_NEW Geometry(pCubeVerts, pIndices);

	// Generate normal vectors from the triangles of the geometry.
	pCube->GenerateNormals();

	// The cube shall be textured. Therefore we create and attach a texture
	// effect, where we add textures and define their blending modes.
	mspTextureEffect = WIRE_NEW TextureEffect;
	Texture2D* pTexture = CreateTexture();
	mspTextureEffect->Textures.Append(pTexture);
	mspTextureEffect->BlendOps.Append(TextureEffect::BM_MODULATE);
	pCube->AttachEffect(mspTextureEffect);

	// NOTE: Geometry takes ownership over Vertex- and IndexBuffers using
	// smart pointers. Thus, you can share these buffers amongst Geometry 
	// objects without having to worry about deletion. Same applies to
	// Effects, Textures and Images.

	return pCube;
}

#include "Sample1.inl"

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
	// without the need to duplicate images in memory.
	Texture2D* pTexture = WIRE_NEW Texture2D(pImage);

	// Use anisotropic filtering for texturing. Looks better, but lowers
	// performance.
	pTexture->SetAnisotropyValue(4.0F);

	return pTexture;
}
