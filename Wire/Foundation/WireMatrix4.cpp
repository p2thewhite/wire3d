#include "WireMatrix4.h"

namespace Wire
{
template<> const Matrix4<Float> Matrix4<Float>::ZERO(
	0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F);
template<> const Matrix4<Float> Matrix4<Float>::IDENTITY(
	1.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 1.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 1.0F);

template<> const Matrix4<Double> Matrix4<Double>::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0);
template<> const Matrix4<Double> Matrix4<Double>::IDENTITY(
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0);
}
