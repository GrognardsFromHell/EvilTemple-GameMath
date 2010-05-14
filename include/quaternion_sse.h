
#include "gamemath.h"

#if !defined(QUATERNION_H)
#define QUATERNION_H
#endif

#include <cmath>

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE Quaternion::Quaternion(float x, float y, float z, float w)
	: mSse(_mm_set_ps(w, z, y, x))
{
}

GAMEMATH_INLINE Quaternion::Quaternion(const Quaternion &other)
	: mSse(other.mSse)
{
}

GAMEMATH_INLINE Quaternion &Quaternion::operator =(const Quaternion &other)
{
	mSse = other.mSse;
	return *this;
}

GAMEMATH_INLINE Quaternion Quaternion::fromAxisAndAngle(const float x, const float y, const float z, const float angle)
{
	const float sinAngle = std::sin(angle * 0.5f);
	const float cosAngle = std::cos(angle * 0.5f);

	return Quaternion(x * sinAngle, y * sinAngle, z * sinAngle, cosAngle);
}

GAMEMATH_INLINE float Quaternion::angle() const
{
	return (float)(std::acos((double)mW) * 2.0);
}

GAMEMATH_NAMESPACE_END
