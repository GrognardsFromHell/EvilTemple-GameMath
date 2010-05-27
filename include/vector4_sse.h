
// This is for IDEs only
#include "gamemath_internal.h"
#include "vector4.h"

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

GAMEMATH_INLINE Vector4 &Vector4::normalize()
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

GAMEMATH_INLINE Vector4 &Vector4::normalizeEstimated()
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

GAMEMATH_INLINE Vector4 Vector4::normalized() const
{
	__m128 length = _dot_product(mSse, mSse);

	// Then take the square root of the lowest vector component
	length = _mm_sqrt_ss(length);

	// Splat the length into each vector component
	length = _mm_shuffle_ps(length, length, _MM_SHUFFLE(0, 0, 0, 0));

	// The last normalization step is dividing each component of this vector, by its length
	Vector4 result;
	result.mSse = _mm_div_ps(mSse, length);

	return result;
}

GAMEMATH_INLINE float Vector4::dot(const Vector4 &vector) const
{
	const __m128 dotProduct = _dot_product(mSse, vector.mSse);

	return _get_lower_register(dotProduct);
}

GAMEMATH_INLINE Vector4 Vector4::cross(const Vector4 &vector) const
{
	Vector4 result;

	// Desired result: a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1, a4*a4-b4*b4

	// First step: a2*b3, a3*b1, a1*b2, a4*b4
	__m128 left = _mm_shuffle_ps(mSse, mSse, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 right = _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(3, 1, 0, 2));
		
	result.mSse = _mm_mul_ps(left, right);

	// Second step: a3*b2, a1*b3, a2*b1, a4*b4
	left = _mm_shuffle_ps(mSse, mSse, _MM_SHUFFLE(3, 1, 0, 2));
	right = _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(3, 0, 2, 1));

	result.mSse = _mm_sub_ps(result.mSse, _mm_mul_ps(left, right));

	return result;
}

GAMEMATH_INLINE Vector4 &Vector4::operator +=(const Vector4 &vector)
{
	mSse = _mm_add_ps(mSse, vector.mSse);
	return *this;
}

GAMEMATH_INLINE Vector4 &Vector4::operator -(const Vector4 &vector)
{
	mSse = _mm_sub_ps(mSse, vector.mSse);
	return *this;
}

GAMEMATH_INLINE Vector4 operator +(const Vector4 &a, const Vector4 &b)
{
	Vector4 result;
	result.mSse = _mm_add_ps(a.mSse, b.mSse);
	return result;
}

GAMEMATH_INLINE Vector4 operator -(const Vector4 &a, const Vector4 &b)
{
	Vector4 result;
	result.mSse = _mm_sub_ps(a.mSse, b.mSse);
	return result;
}

GAMEMATH_INLINE Vector4 Vector4::operator -() const
{
	Vector4 result;
	result.mSse = _mm_xor_ps(mSse, *reinterpret_cast<const __m128*>(&SignMaskXYZ));
	return result;
}

GAMEMATH_INLINE Vector4 operator *(const float factor, const Vector4 &vector)
{
	__m128 factorVector = _mm_set_ps(1, factor, factor, factor);
	Vector4 result;
	result.mSse = _mm_mul_ps(vector.mSse, factorVector);
	return result;
}

GAMEMATH_INLINE Vector4 operator *(const Vector4 &vector, const float factor)
{
	__m128 factorVector = _mm_set_ps(1, factor, factor, factor);
	Vector4 result;
	result.mSse = _mm_mul_ps(vector.mSse, factorVector);
	return result;
}

GAMEMATH_INLINE Vector4 &Vector4::operator *=(const float factor)
{
	__m128 factorVector = _mm_set_ps(1, factor, factor, factor);
	mSse = _mm_mul_ps(mSse, factorVector);
	return *this;
}

GAMEMATH_INLINE Vector4 Vector4::absolute() const
{
    Vector4 result;
    __m128 invSignMask = _mm_load_ps(reinterpret_cast<const float*>(InvertedSignmask));
    result.mSse = _mm_and_ps(invSignMask, mSse);
    return result;
}

GAMEMATH_INLINE bool Vector4::isInfinite() const
{
    Vector4 tmp = absolute();

    __m128 posInf = _mm_load_ps(reinterpret_cast<const float*>(PositiveInfinity));

    __m128 res = _mm_cmpeq_ps(posInf, tmp.mSse);

    return (_mm_movemask_ps(res) != 0);
}

GAMEMATH_INLINE bool Vector4::operator ==(const Vector4 &other) const
{
    return _mm_movemask_ps(_mm_cmpeq_ps(mSse, other.mSse)) == 0xF;
}

GAMEMATH_NAMESPACE_END
