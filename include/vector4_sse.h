
// This is for IDEs only
#include "gamemath.h"

#if !defined(VECTOR4_H)
#error "Do not include this file directly, only include vector4.h"
#endif

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE Vector4::Vector4()
{
}

GAMEMATH_INLINE Vector4::Vector4(const float x, const float y, const float z, const float w)
	: mSse(_mm_set_ps(w, z, y, x))
{
}

/**
 * Computes the dot product of two vectors a and b and stores the result in r0
 * of an SSE register, and returns the register.
 */
GAMEMATH_INLINE __m128 _dot_product(const __m128 a, const __m128 b)
{
	// Compute x1*x2,y1*y2,z1*z2,w1*w2
	__m128 dotProduct = _mm_mul_ps(a, b);

	// Fold the resulting vector, so that we get squaredLength=x+z,y+w,_,_
	__m128 tmp = _mm_shuffle_ps(dotProduct, dotProduct, _MM_SHUFFLE(0,0,3,2));
	dotProduct = _mm_add_ps(dotProduct, tmp);

	// Fold again, to get (x+z)+(y+w)
	tmp = _mm_shuffle_ps(dotProduct, dotProduct, _MM_SHUFFLE(0,0,0,1));
	return _mm_add_ps(dotProduct, tmp);
}

/**
 * Retrieve r0 of a SSE register
 */
GAMEMATH_INLINE float _get_lower_register(const __m128 &vector)
{
	// Store the result into a 16-byte aligned array and retrieve the lowest element
	GAMEMATH_ALIGN float result[4];
	_mm_store_ps(result, vector);
	return result[0];
}

GAMEMATH_INLINE float Vector4::length() const
{
	// Compute the square root (only of the lowest fp value)
	__m128 length = _mm_sqrt_ss(_dot_product(mSse, mSse));
	
	return _get_lower_register(length);
}

GAMEMATH_INLINE float Vector4::lengthSquared() const
{
	return dot(*this);
}

GAMEMATH_INLINE void Vector4::normalize()
{
	__m128 length = _dot_product(mSse, mSse);

	// Splat the length into each vector component
	length = _mm_shuffle_ps(length, length, _MM_SHUFFLE(0, 0, 0, 0));

	// Then take the square root of each vector component
	length = _mm_sqrt_ps(length);

	// The last normalization step is dividing each component of this vector, by its length
	mSse = _mm_div_ps(mSse, length);
}

GAMEMATH_INLINE Vector4 Vector4::normalized() const
{
	return *this;
}

GAMEMATH_INLINE float Vector4::dot(const Vector4 &vector) const
{
	const __m128 dotProduct = _dot_product(mSse, vector.mSse);

	return _get_lower_register(dotProduct);
}

GAMEMATH_NAMESPACE_END
