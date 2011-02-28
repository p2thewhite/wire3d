#include "Sample7.h"

using namespace Wire;

WIRE_APPLICATION(Sample7);

//----------------------------------------------------------------------------
Bool Sample7::OnInitialize()
{
	if (!Parent::OnInitialize())
	{
		return false;
	}

	mspGeometry = CreateGeometry(8, 128);

	// setup our camera at the origin
	// looking down the -z axis with y up
	Vector3F cameraLocation(0.0F, 0.0F, 6.0F);
	Vector3F viewDirection(0.0F, 0.0F, -1.0F);
	Vector3F up(0.0F, 1.0F, 0.0F);
	Vector3F right = viewDirection.Cross(up);
	mspCamera = WIRE_NEW Camera;
	mspCamera->SetFrame(cameraLocation, viewDirection, up, right);

	Float width = static_cast<Float>(GetRenderer()->GetWidth());
	Float height = static_cast<Float>(GetRenderer()->GetHeight());
	mspCamera->SetFrustum(45, width / height , 0.1F, 300.0F);

	mAngle = 0.0F;
	mLastTime = System::GetTime();
	return true;
}

//----------------------------------------------------------------------------
void Sample7::OnIdle()
{
	Double time = System::GetTime();
	Double elapsedTime = time - mLastTime;
	mLastTime = time;

	mAngle += static_cast<Float>(elapsedTime);
	mAngle = MathF::FMod(mAngle, MathF::TWO_PI);

	// rotate and zoom the torus knots
	Matrix34F rotate(Vector3F(1, 1, 0), mAngle);
	mspGeometry->World.SetRotate(rotate);

	GetRenderer()->ClearBuffers();
	GetRenderer()->PreDraw(mspCamera);
	GetRenderer()->Draw(mspGeometry);
	GetRenderer()->PostDraw();
	GetRenderer()->DisplayBackBuffer();
}

//----------------------------------------------------------------------------
Geometry* Sample7::CreateGeometry(UInt shapeCount, UInt segmentCount)
{
	// Create a PQ (4,3) torus knot with a inner radius of 0.2
	Geometry* pTorus = CreatePqTorusKnot(shapeCount, 0.2F, segmentCount, 3, 2);

	StateMaterial* pMaterial = WIRE_NEW StateMaterial;
	pMaterial->Ambient = ColorRGBA(1.0F, 1.0F, 0.5F, 1.0f); 
	pTorus->States[State::MATERIAL] = pMaterial;

	Light* pLight = WIRE_NEW Light;
	pTorus->Lights.Append(pLight);

	return pTorus;
}

//----------------------------------------------------------------------------
Geometry* Sample7::CreatePqTorusKnot(UInt shapeCount, Float shapeRadius,
	UInt segmentCount, UInt p, UInt q)
{
	shapeCount++;
	segmentCount++;

	// create the inner shape (i.e. a circle)
	TArray<Vector3F> shape(shapeCount);
	Vector3F pos(0, shapeRadius, 0);
	Float angleStride = MathF::TWO_PI / (shapeCount-1);
	Float angle = 0;
	for (UInt i = 0; i < (shapeCount-1); i++)
	{
		Matrix34F rot(Vector3F(0, 0, 1), angle);
		shape.SetElement(i, pos * rot);
		angle += angleStride;
	}

	// the last vertex can't share uv-coords, so we duplicate the first vertex
	shape.SetElement(shapeCount-1, shape[0]);

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);  // channels: X, Y, Z
	attributes.SetNormalChannels(3);

	const UInt vertexCount = segmentCount * shapeCount;
	VertexBuffer* pVBuffer = WIRE_NEW VertexBuffer(attributes, vertexCount);

	// create the pq torus knot and position & align the shape along it
	angleStride = MathF::TWO_PI / (segmentCount-1);
	angle = 0;
	for (UInt i = 0; i < segmentCount-1; i++)
	{
		Float r = 0.5F * (2 + MathF::Sin(q * angle));
		Float x = r * MathF::Cos(p * angle);
		Float y = r * MathF::Cos(q * angle);
		Float z = r * MathF::Sin(p * angle);
		Vector3F p0(x, y, z);

		r = 0.5F * (2 + MathF::Sin(q * (angle+0.1F)));
		x = r * MathF::Cos(p * (angle+0.1F));
		y = r * MathF::Cos(q * (angle+0.1F));
		z = r * MathF::Sin(p * (angle+0.1F));
		Vector3F p1(x, y, z);

		angle += angleStride;		

		// approximate Frenet frame
		Vector3F t = p1-p0;
		Vector3F n = p1+p0;
		Vector3F b = t.Cross(n);
		n = b.Cross(t);
		n.Normalize();
		b.Normalize();

		for (UInt j = 0; j < shapeCount; j++)
		{	
			Vector3F newVertex = p0 + shape[j].X() * n + shape[j].Y() * b;
			pVBuffer->Position3(i*shapeCount + j) = newVertex;
		}
	}

	// copy the last segment from the first
	for (UInt j = 0; j < shapeCount; j++)
	{	
		pVBuffer->Position3((segmentCount-1)*shapeCount + j) = pVBuffer->
			Position3(j);
	}

	// here we establish connectivity information defined in an IndexBuffer
	const UInt indexCount = (segmentCount-1)*(shapeCount-1)*6;
	IndexBuffer* pIBuffer = WIRE_NEW IndexBuffer(indexCount);
	for (UInt j = 0; j < segmentCount-1; j++)
	{
		UInt offset = shapeCount*j;
		for (UInt i = 0; i < shapeCount-1; i++)
		{
			UInt index = (shapeCount-1)*j+i;
			UInt index0 = i+offset;
			UInt index1 = index0+shapeCount;
			UInt index2 = index0+1;
			UInt index3 = index0+shapeCount+1;

			(*pIBuffer)[index*6] = index0;
			(*pIBuffer)[index*6+1] = index1;
			(*pIBuffer)[index*6+2] = index2;

			(*pIBuffer)[index*6+3] = index2;
			(*pIBuffer)[index*6+4] = index1;
			(*pIBuffer)[index*6+5] = index3;
		}
	}

	Geometry* pGeo = WIRE_NEW Geometry(pVBuffer, pIBuffer);
	pGeo->GenerateNormals(true);
	return pGeo;
}

//----------------------------------------------------------------------------
struct Cell
{
	Vector2F point;
	ColorRGB color;
};
