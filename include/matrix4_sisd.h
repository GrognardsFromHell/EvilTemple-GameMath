
// This is for IDEs only
#include "gamemath.h"

#if !defined(MATRIX4_H)
#error "Do not include this file directly, only include matrix4.h"
#endif

#include <cstring>

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE void Matrix4::setToIdentity()
{
	setToZero();
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
}

GAMEMATH_INLINE void Matrix4::setToZero()
{
	std::memset(m, 0, sizeof(m)); // all zeros is equivalent to IEEE zero
}

GAMEMATH_INLINE Matrix4 Matrix4::scaling(const Vector4 &vector)
{
	return scaling(vector.x(), vector.y(), vector.z());
}

GAMEMATH_INLINE Matrix4 Matrix4::scaling(const float sx, const float sy, const float sz)
{
	Matrix4 result;
	result.setToZero();
	result.m[0][0] = sx;
	result.m[1][1] = sy;
	result.m[2][2] = sz;
	result.m[3][3] = 1;
	return result;
}

GAMEMATH_INLINE Vector4 Matrix4::operator *(const Vector4 &vector) const
{
	float x = m[0][0] * vector.x() + m[0][1] * vector.x() + m[0][2] * vector.x() + m[0][3] * vector.x();
	float y = m[1][0] * vector.y() + m[1][1] * vector.y() + m[1][2] * vector.y() + m[1][3] * vector.y();
	float z = m[2][0] * vector.z() + m[2][1] * vector.z() + m[2][2] * vector.z() + m[2][3] * vector.z();
	float w = m[3][0] * vector.w() + m[3][1] * vector.w() + m[3][2] * vector.w() + m[3][3] * vector.w();

	return Vector4(x, y, z, w);
}

GAMEMATH_NAMESPACE_END
