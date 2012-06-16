#include "WireEulerAngle.h"

namespace Wire
{

const char* GetEulerAngleName(EulerAngle eulerAngle)
{
	switch (eulerAngle)
	{
	case YAW:
		return "YAW";
	case PITCH:
		return "PITCH";
	case ROLL:
		return "ROLL";
	default:
		return "INVALID_EULER_ANGLE";
	}
}

}