#ifndef EULERANGLE_H_
#define EULERANGLE_H_

namespace Wire
{
	
enum EulerAngle
{
	YAW,
	PITCH,
	ROLL,
	INVALID_EULER_ANGLE
};

const char* GetEulerAngleName(EulerAngle eulerAngle);

}

#endif