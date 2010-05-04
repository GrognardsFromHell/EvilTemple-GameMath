#ifndef VECTOR4_H
#define VECTOR4_H

#include "gamemath.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_ALIGN class Vector4
{
friend class Matrix4;
friend GAMEMATH_INLINE Vector4 operator +(const Vector4 &a, const Vector4 &b);
friend GAMEMATH_INLINE Vector4 operator -(const Vector4 &a, const Vector4 &b);
friend GAMEMATH_INLINE Vector4 operator *(const float, const Vector4 &vector);
friend GAMEMATH_INLINE Vector4 operator *(const Vector4 &vector, const float);
public:
	Vector4();
	Vector4(const float x, const float y, const float z, const float w);

	/**
	 * Returns the x component of this vector.
	 */
	float x() const;

	/**
	 * Returns the y component of this vector.
	 */
	float y() const;

	/**
	 * Returns the z component of this vector.
	 */
	float z() const;

	/**
	 * Returns the w component of this vector. Usually 1 means this is a position vector, while 0
	 * means this is a directional vector.
	 */
	float w() const;

	/**
	 * Computes the squared length of this vector. This method is significantly
	 * faster than computing the normal length, since the square root can be omitted.
	 */
	float lengthSquared() const;

	/**
	 * Computes this vector's length.
	 */
	float length() const;

	/**
	 * Normalizes this vector by dividing its components by this vectors length.
	 */
	Vector4 &normalize();

	/**
	 * Normalizes using a reciprocal square root, which only has 11-bit precision. Use this if the
	 * result doesn't need to be very precisely normalized.
	 */
	Vector4 &normalizeEstimated();

	/**
	 * Normalizes this vector and returns it in a new object, while leaving this object untouched.
	 */
	Vector4 normalized() const;

	/**
	 * Computes the dot product between this and another vector.
	 */
	float dot(const Vector4 &vector) const;

	/**
	 * Computes the three-dimensional cross product of two vectors, interpreting both vectors as directional vectors
	 * (w=0). If either vector's w is not zero, the result may be wrong.
	 *
	 * @return this x vector
	 */
	Vector4 cross(const Vector4 &vector) const;

	/**
	 * Adds another vector to this vector.
	 */
	Vector4 &operator +=(const Vector4 &vector);

	/**
	 * Multiplies this vector with a scalar factor.
	 * Only the x,y, and z components of the vector are multiplied.
	 */
	Vector4 &operator *=(const float factor);

	/**
	 * Subtracts another vector from this vector.
	 */
	Vector4 &operator -(const Vector4 &vector);

	/**
	 * Returns a negated version of this vector, negating only the x, y, and z components.
	 */
	Vector4 operator -() const;

	const float *data() const;

	float *data();

private:
#if !defined(GAMEMATH_NO_INTRINSICS)
	union {
		struct {
			float mX;
			float mY;
			float mZ;
			float mW;
		};
		__m128 mSse;
	};
#else
	float mX;
	float mY;
	float mZ;
	float mW;
#endif // GAMEMATH_NO_INTRINSICS
};

GAMEMATH_INLINE float Vector4::x() const 
{
	return mX;
}

GAMEMATH_INLINE float Vector4::y() const 
{
	return mY;
}

GAMEMATH_INLINE float Vector4::z() const 
{
	return mZ;
}

GAMEMATH_INLINE float Vector4::w() const 
{
	return mW;
}

GAMEMATH_INLINE const float *Vector4::data() const
{
	return &mX;
}

GAMEMATH_INLINE float *Vector4::data()
{
	return &mX;
}

GAMEMATH_NAMESPACE_END

#if !defined(GAMEMATH_NO_INTRINSICS)
#include "vector4_sse.h"
#else
#include "vector4_sisd.h"
#endif // GAMEMATH_NO_INTRINSICS

#endif // VECTOR4_H
