#include "WireMatrix4.h"

namespace Wire
{
template<> const Matrix4<float> Matrix4<float>::ZERO(
	0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F);
template<> const Matrix4<float> Matrix4<float>::IDENTITY(
	1.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 1.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 1.0F);

template<> const Matrix4<double> Matrix4<double>::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0);
template<> const Matrix4<double> Matrix4<double>::IDENTITY(
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0);
}
