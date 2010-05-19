#ifndef QUATERNION_H
#define QUATERNION_H

#include "gamemath.h"

GAMEMATH_NAMESPACE_BEGIN

class Quaternion {
friend GAMEMATH_INLINE Quaternion operator +(const Quaternion &a, const Quaternion &b);
friend GAMEMATH_INLINE Quaternion operator -(const Quaternion &a, const Quaternion &b);
friend GAMEMATH_INLINE Quaternion operator *(const float, const Quaternion &vector);
friend GAMEMATH_INLINE Quaternion operator *(const Quaternion &vector, const float);
public:

	/**
	 * Creates an uninitialized quaternion.
	 */
	Quaternion();

	/**
	 * Creates a quaternion from an x, y, z and w component.
	 */
	Quaternion(float x, float y, float z, float w);

	/**
	 * Creates a new quaternion by copying data from another quaternion.
	 */
	Quaternion(const Quaternion &other);

	/**
	 * Copies another quaternion's internal data to this quaternion.
	 */
	Quaternion &operator =(const Quaternion &other);

	/**
	 * Returns an array of floats (x,y,z,w) representing this quaternions internal data.
	 */
	const float *data() const;

	/**
	 * Returns an array of floats (x,y,z,w) representing this quaternions internal data.
	 */
	float *data();

	/**
	 * Returns the x component of this quaternion.
	 */
	float x() const;

	/**
	 * Returns the y component of this quaternion.
	 */
	float y() const;

	/**
	 * Returns the z component of this quaternion.
	 */
	float z() const;

	/**
	 * Returns the w component of this quaternion.
	 */
	float w() const;

	/**
	 * Creates a quaternion from a rotation axis and an angle expressed in radians.
	 */
	static Quaternion fromAxisAndAngle(const float x, const float y, const float z, const float angle);

	/**
	 * Computes the squared length of this quaternion. This method is significantly
	 * faster than computing the normal length, since the square root can be omitted.
	 */
	float lengthSquared() const;

	/**
	 * Computes this quaternion's length.
	 */
	float length() const;

	/**
	 * Normalizes this quaternion by dividing its components by its length.
	 */
	Quaternion &normalize();

	/**
	 * Normalizes using a reciprocal square root, which only has 11-bit precision. Use this if the
	 * result doesn't need to be very precisely normalized.
	 */
	Quaternion &normalizeEstimated();

	/**
	 * Normalizes this vector and returns it in a new object, while leaving this object untouched.
	 */
	Quaternion normalized() const;

	/**
	 * Computes the dot product between this and another quaternion.
	 */
	float dot(const Quaternion  &vector) const;

	/**
	 * Retrieve the angle of rotation from this quaternion by applying cos^-1 to the scalar and multiplying with two.
	 */
	float angle() const;

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

GAMEMATH_INLINE Quaternion::Quaternion()
{
}

GAMEMATH_INLINE float Quaternion::x() const 
{
	return mX;
}

GAMEMATH_INLINE float Quaternion::y() const 
{
	return mY;
}

GAMEMATH_INLINE float Quaternion::z() const 
{
	return mZ;
}

GAMEMATH_INLINE float Quaternion::w() const 
{
	return mW;
}

GAMEMATH_INLINE const float *Quaternion::data() const
{
	return &mX;
}

GAMEMATH_INLINE float *Quaternion::data() {
	return &mX;
}

GAMEMATH_NAMESPACE_END

#if !defined(GAMEMATH_NO_INTRINSICS)
#include "quaternion_sse.h"
#else
#include "quaternion_sisd.h"
#endif

#endif // QUATERNION_H