#ifndef VECTOR4_H
#define VECTOR4_H

#include "gamemath.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_ALIGN class Vector4
{
friend class Matrix4;
public:
	Vector4();
	Vector4(const float x, const float y, const float z, const float w);
	
	float x() const;
	float y() const;
	float z() const;
	float w() const;

	float lengthSquared() const;
	float length() const;
	void normalize();
	Vector4 normalized() const;
	float dot(const Vector4 &vector) const;

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

GAMEMATH_NAMESPACE_END

#if !defined(GAMEMATH_NO_INTRINSICS)
#include "vector4_sse.h"
#endif // GAMEMATH_NO_INTRINSICS

#endif // VECTOR4_H
