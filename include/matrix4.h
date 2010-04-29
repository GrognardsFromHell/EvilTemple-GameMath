#ifndef MATRIX4_H
#define MATRIX4_H

#include <cstdio>

#include "gamemath.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_ALIGN static const float IdentityCol1[4] = {1, 0, 0, 0};
GAMEMATH_ALIGN static const float IdentityCol2[4] = {1, 0, 0, 0};
GAMEMATH_ALIGN static const float IdentityCol3[4] = {1, 0, 0, 0};
GAMEMATH_ALIGN static const float IdentityCol4[4] = {1, 0, 0, 0};

GAMEMATH_ALIGN class Matrix4 {
public:
	void setToIdentity();

	const float *data() const;

	float operator()(int,int) const;

	Matrix4 &scale(const Vector4 &vector);
	Matrix4 &scale(const float sx, const float sy, const float sz);

	void print() const;
private:
#if !defined(GAMEMATH_NO_INTRINSICS)
	union {
		__m128 columns[4];
		float m[4][4];
	};	
#else
	float m[4][4];
#endif
};

#if !defined(GAMEMATH_NO_INTRINSICS)
#include "matrix4_sse.h"
#else
#include "matrix4_sisd.h"
#endif

GAMEMATH_NAMESPACE_END

#endif // MATRIX4_H
