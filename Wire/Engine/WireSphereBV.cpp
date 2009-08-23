#include "WireSphereBV.h"

using namespace Wire;

//----------------------------------------------------------------------------
// The factory function of BoundingVolume creates SphereBV objects. You may
// delete this and allow another bounding volume class to be the default.
//----------------------------------------------------------------------------
BoundingVolume* BoundingVolume::Create()
{
	return WIRE_NEW SphereBV;
}

//----------------------------------------------------------------------------
SphereBV::SphereBV()
	:
	mSphere(Vector3F::ZERO, 0.0F)
{
}

//----------------------------------------------------------------------------
SphereBV::SphereBV(const Sphere3F& rSphere)
	:
	mSphere(rSphere)
{
}

//----------------------------------------------------------------------------
SphereBV::~SphereBV()
{
}


//----------------------------------------------------------------------------
void SphereBV::ComputeFromData(const VertexBuffer* pVBuffer)
{
	if (pVBuffer)
	{
		UInt quantity = pVBuffer->GetVertexQuantity();

		mSphere.mCenter = Vector3F::ZERO;
		mSphere.mRadius = 0.0F;

		for (UInt i = 0; i < quantity; i++)
		{
			mSphere.mCenter += pVBuffer->Position3(i);
		}

		mSphere.mCenter /= static_cast<Float>(quantity);

		for (UInt i = 0; i < quantity; i++)
		{
			Vector3F diff = pVBuffer->Position3(i) - mSphere.mCenter;
			Float radiusSqr = diff.SquaredLength();
			if (radiusSqr > mSphere.mRadius)
			{
				mSphere.mRadius = radiusSqr;
			}
		}

		mSphere.mRadius = MathF::Sqrt(mSphere.mRadius);
	}
}

//----------------------------------------------------------------------------
void SphereBV::TransformBy(/*const*/ Transformation& rTransform,
	BoundingVolume* pResult)
{
	Sphere3F& rTarget = ((SphereBV*)pResult)->mSphere;
	rTarget.mCenter = rTransform.ApplyForward(mSphere.mCenter);
	rTarget.mRadius = rTransform.GetNorm() * mSphere.mRadius;
}
