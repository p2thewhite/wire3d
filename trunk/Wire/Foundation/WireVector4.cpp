#include "WireVector4.h"

namespace Wire
{
template<> const Vector4<Float> Vector4<Float>::ZERO(0.0F, 0.0F, 0.0F, 0.0F);
template<> const Vector4<Float> Vector4<Float>::UNIT_X(1.0F, 0.0F, 0.0F, 0.0F);
template<> const Vector4<Float> Vector4<Float>::UNIT_Y(0.0F, 1.0F, 0.0F, 0.0F);
template<> const Vector4<Float> Vector4<Float>::UNIT_Z(0.0F, 0.0F, 1.0F, 0.0F);
template<> const Vector4<Float> Vector4<Float>::UNIT_W(0.0F, 0.0F, 0.0F, 1.0F);
template<> const Vector4<Float> Vector4<Float>::ONE(1.0F, 1.0F, 1.0F, 1.0F);

template<> const Vector4<Double> Vector4<Double>::ZERO(0.0, 0.0, 0.0, 0.0);
template<> const Vector4<Double> Vector4<Double>::UNIT_X(1.0, 0.0, 0.0, 0.0);
template<> const Vector4<Double> Vector4<Double>::UNIT_Y(0.0, 1.0, 0.0, 0.0);
template<> const Vector4<Double> Vector4<Double>::UNIT_Z(0.0, 0.0, 1.0, 0.0);
template<> const Vector4<Double> Vector4<Double>::UNIT_W(0.0, 0.0, 1.0, 1.0);
template<> const Vector4<Double> Vector4<Double>::ONE(1.0, 1.0, 1.0, 1.0);
}
