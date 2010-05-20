
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


GAMEMATH_INLINE Quaternion operator +(const Quaternion &a, const Quaternion &b)
{
	Quaternion result;
	result.mSse = _mm_add_ps(a.mSse, b.mSse);
	return result;
}

GAMEMATH_INLINE Quaternion operator -(const Quaternion &a, const Quaternion &b)
{
	Quaternion result;
	result.mSse = _mm_sub_ps(a.mSse, b.mSse);
	return result;
}

GAMEMATH_INLINE Quaternion operator *(const float factor, const Quaternion &vector)
{
	__m128 factorVector = _mm_set_ps(factor, factor, factor, factor);
	Quaternion result;
	result.mSse = _mm_mul_ps(vector.mSse, factorVector);
	return result;
}

GAMEMATH_INLINE Quaternion operator *(const Quaternion &vector, const float factor)
{
	__m128 factorVector = _mm_set_ps(factor, factor, factor, factor);
	Quaternion result;
	result.mSse = _mm_mul_ps(vector.mSse, factorVector);
	return result;
}


GAMEMATH_INLINE float Quaternion::length() const
{
	// Compute the square root (only of the lowest fp value)
	__m128 length = _mm_sqrt_ss(_dot_product(mSse, mSse));
	
	return _get_lower_register(length);
}

GAMEMATH_INLINE float Quaternion::lengthSquared() const
{
	return dot(*this);
}

GAMEMATH_INLINE Quaternion &Quaternion::normalize()
{
	__m128 length = _dot_product(mSse, mSse);

	// Then take the square root of the lowest vector component
	length = _mm_sqrt_ss(length);

	// Splat the length into each vector component
	length = _mm_shuffle_ps(length, length, _MM_SHUFFLE(0, 0, 0, 0));

	// The last normalization step is dividing each component of this vector, by its length
	mSse = _mm_div_ps(mSse, length);

	// TODO: What to do for nearly length0 vectors

	return *this;
}

GAMEMATH_INLINE Quaternion &Quaternion::normalizeEstimated()
{
	__m128 length = _dot_product(mSse, mSse);

	// Then take the square root of each vector component
	length = _mm_rsqrt_ss(length);

	// Splat the length into each vector component
	length = _mm_shuffle_ps(length, length, _MM_SHUFFLE(0, 0, 0, 0));

	// The last normalization step is dividing each component of this vector, by its length
	mSse = _mm_mul_ps(mSse, length);

	// TODO: What to do for nearly length0 vectors

	return *this;
}

GAMEMATH_INLINE Quaternion Quaternion::normalized() const
{
	__m128 length = _dot_product(mSse, mSse);

	// Then take the square root of the lowest vector component
	length = _mm_sqrt_ss(length);

	// Splat the length into each vector component
	length = _mm_shuffle_ps(length, length, _MM_SHUFFLE(0, 0, 0, 0));

	// The last normalization step is dividing each component of this vector, by its length
	Quaternion result;
	result.mSse = _mm_div_ps(mSse, length);

	return result;
}

GAMEMATH_INLINE float Quaternion::dot(const Quaternion &vector) const
{
	const __m128 dotProduct = _dot_product(mSse, vector.mSse);

	return _get_lower_register(dotProduct);
}

GAMEMATH_NAMESPACE_END
