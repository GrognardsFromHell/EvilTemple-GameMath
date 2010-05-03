
#include "gamemath.h"

#include <cmath>

#if !defined(QUATERNION_H)
#define QUATERNION_H
#endif

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE Quaternion::Quaternion(float x, float y, float z, float w)
: mX(x), mY(y), mZ(z), mW(w)
{
}

GAMEMATH_INLINE Quaternion::Quaternion(const Quaternion &other)
	: mX(other.mX), mY(other.mY), mZ(other.mZ), mW(other.mW)
{
}

GAMEMATH_INLINE Quaternion &Quaternion::operator =(const Quaternion &other)
{
	mX = other.mX;
	mY = other.mY;
	mZ = other.mZ;
	mW = other.mW;
}

GAMEMATH_INLINE Quaternion Quaternion::fromAxisAndAngle(const float x, const float y, const float z, const float angle)
{
	const float sinAngle = std::sinf(angle * 0.5f);
	const float cosAngle = std::cosf(angle * 0.5f);

	return Quaternion(x * sinAngle, y * sinAngle, z * sinAngle, cosAngle);
}

GAMEMATH_NAMESPACE_END
