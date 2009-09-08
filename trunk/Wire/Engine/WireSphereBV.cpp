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

		mSphere.Center = Vector3F::ZERO;
		mSphere.Radius = 0.0F;

		for (UInt i = 0; i < quantity; i++)
		{
			mSphere.Center += pVBuffer->Position3(i);
		}

		mSphere.Center /= static_cast<Float>(quantity);

		for (UInt i = 0; i < quantity; i++)
		{
			Vector3F diff = pVBuffer->Position3(i) - mSphere.Center;
			Float radiusSqr = diff.SquaredLength();
			if (radiusSqr > mSphere.Radius)
			{
				mSphere.Radius = radiusSqr;
			}
		}

		mSphere.Radius = MathF::Sqrt(mSphere.Radius);
	}
}

//----------------------------------------------------------------------------
void SphereBV::TransformBy(/*const*/ Transformation& rTransform,
	BoundingVolume* pResult)
{
	Sphere3F& rTarget = (static_cast<SphereBV*>(pResult))->mSphere;
	rTarget.Center = rTransform.ApplyForward(mSphere.Center);
	rTarget.Radius = rTransform.GetNorm() * mSphere.Radius;
}

//----------------------------------------------------------------------------
Int SphereBV::WhichSide(const Plane3F& rPlane) const
{
	Float distance = rPlane.DistanceTo(mSphere.Center);

	if (distance <= -mSphere.Radius)
	{
		return -1;
	}

	if (distance >= mSphere.Radius)
	{
		return +1;
	}

	return 0;
}

//----------------------------------------------------------------------------
void SphereBV::CopyFrom(const BoundingVolume* pInput)
{
	mSphere = static_cast<SphereBV*>(const_cast<BoundingVolume*>(pInput))->
		mSphere;
}

//----------------------------------------------------------------------------
void SphereBV::GrowToContain(const BoundingVolume* pInput)
{
	mSphere = MergeSpheres(mSphere, static_cast<SphereBV*>(
		const_cast<BoundingVolume*>(pInput))->mSphere);
}
