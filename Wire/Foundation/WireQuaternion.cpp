#include "WireQuaternion.h"

namespace Wire
{
template<> const Quaternion<Float> Quaternion<Float>::IDENTITY(
	1.0F, 0.0F, 0.0F, 0.0F);
template<> const Quaternion<Float> Quaternion<Float>::ZERO(
	0.0F, 0.0F, 0.0F, 0.0f);

template<> const Quaternion<Double> Quaternion<Double>::IDENTITY(
	1.0, 0.0, 0.0 ,0.0);
template<> const Quaternion<Double> Quaternion<Double>::ZERO(
	0.0, 0.0, 0.0, 0.0);
}
