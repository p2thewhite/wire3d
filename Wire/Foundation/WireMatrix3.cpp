#include "WireMatrix3.h"

namespace Wire
{
template<> const Matrix3<Float> Matrix3<Float>::ZERO(
	0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F);
template<> const Matrix3<Float> Matrix3<Float>::IDENTITY(
	1.0F, 0.0F, 0.0F,
	0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 1.0F);

template<> const Matrix3<Double> Matrix3<Double>::ZERO(
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0);
template<> const Matrix3<Double> Matrix3<Double>::IDENTITY(
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0);
}
