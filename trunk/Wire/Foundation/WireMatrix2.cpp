#include "WireMatrix2.h"

namespace Wire
{
template<> const Matrix2<Float> Matrix2<Float>::ZERO(
	0.0F, 0.0F,
	0.0F, 0.0F);
template<> const Matrix2<Float> Matrix2<Float>::IDENTITY(
	1.0F, 0.0F,
	0.0F, 1.0F);

template<> const Matrix2<Double> Matrix2<Double>::ZERO(
	0.0, 0.0,
	0.0, 0.0);
template<> const Matrix2<Double> Matrix2<Double>::IDENTITY(
	1.0, 0.0,
	0.0, 1.0);
}
