#include "WireVector3.h"

namespace Wire
{
template<> const Vector3<Float> Vector3<Float>::ZERO(0.0f, 0.0f, 0.0f);
template<> const Vector3<Float> Vector3<Float>::UNIT_X(1.0f, 0.0f, 0.0f);
template<> const Vector3<Float> Vector3<Float>::UNIT_Y(0.0f, 1.0f, 0.0f);
template<> const Vector3<Float> Vector3<Float>::UNIT_Z(0.0f, 0.0f, 1.0f);
template<> const Vector3<Float> Vector3<Float>::ONE(1.0f, 1.0f, 1.0f);

template<> const Vector3<Double> Vector3<Double>::ZERO(0.0, 0.0, 0.0);
template<> const Vector3<Double> Vector3<Double>::UNIT_X(1.0, 0.0, 0.0);
template<> const Vector3<Double> Vector3<Double>::UNIT_Y(0.0, 1.0, 0.0);
template<> const Vector3<Double> Vector3<Double>::UNIT_Z(0.0, 0.0, 1.0);
template<> const Vector3<Double> Vector3<Double>::ONE(1.0, 1.0, 1.0);
}