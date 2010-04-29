
// This is for IDEs only
#include "gamemath.h"

#if !defined(VECTOR4_H)
#error "Do not include this file directly, only include vector4.h"
#endif

#include <cmath>

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE Vector4::Vector4()
{
}

GAMEMATH_INLINE Vector4::Vector4(const float x, const float y, const float z, const float w)
	: mX(x), mY(y), mZ(z), mW(w)
{
}

GAMEMATH_INLINE float Vector4::length() const
{
	return std::sqrtf(lengthSquared());
}

GAMEMATH_INLINE float Vector4::lengthSquared() const
{
	return dot(*this);
}

GAMEMATH_INLINE void Vector4::normalize()
{
	const float reciprocalLength = 1 / length();
	
	mX *= reciprocalLength;
	mY *= reciprocalLength;
	mZ *= reciprocalLength;
	mW *= reciprocalLength;
}

GAMEMATH_INLINE Vector4 Vector4::normalized() const
{
	return *this;
}

GAMEMATH_INLINE float Vector4::dot(const Vector4 &vector) const
{
	return mX * vector.mX + mY * vector.mY + mZ * vector.mZ + vector.mW * vector.mW;
}

GAMEMATH_INLINE Vector4 Vector4::cross(const Vector4 &vector) const
{
	Vector4 result;

	result.mX = mY * vector.mZ - mZ * vector.mY;
	result.mY = mZ * vector.mX - mX * vector.mZ;
	result.mZ = mX * vector.mY - mY * vector.mX;
	result.mW = 0;

	return result;
}

GAMEMATH_NAMESPACE_END