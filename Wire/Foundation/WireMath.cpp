#include "WireMath.h"

namespace Wire
{
template<> const Float Math<Float>::PI = static_cast<Float>(4.0*atan(1.0));
template<> const Float Math<Float>::MAX_REAL =  FLT_MAX;
template<> const Float Math<Float>::DEG_TO_RAD = Math<Float>::PI/180.0f;
template<> const Float Math<Float>::RAD_TO_DEG = 180.0f/Math<Float>::PI;

template<> const Double Math<Double>::PI = 4.0*atan(1.0);
template<> const Double Math<Double>::MAX_REAL =  DBL_MAX;
template<> const Double Math<Double>::DEG_TO_RAD = Math<Double>::PI/180.0;
template<> const Double Math<Double>::RAD_TO_DEG = 180.0/Math<Double>::PI;
}