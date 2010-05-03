
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
	float x = m[0][0] * vector.x() + m[1][0] * vector.y() + m[2][0] * vector.z() + m[3][0] * vector.w();
	float y = m[0][1] * vector.x() + m[1][1] * vector.y() + m[2][1] * vector.z() + m[3][1] * vector.w();
	float z = m[0][2] * vector.x() + m[1][2] * vector.y() + m[2][2] * vector.z() + m[3][2] * vector.w();
	float w = m[0][3] * vector.x() + m[1][3] * vector.y() + m[2][3] * vector.z() + m[3][3] * vector.w();

	return Vector4(x, y, z, w);
}

GAMEMATH_INLINE Vector4 Matrix4::mapPosition(const Vector4 &vector) const
{
	float x = m[0][0] * vector.x() + m[1][0] * vector.y() + m[2][0] * vector.z() + m[3][0];
	float y = m[0][1] * vector.x() + m[1][1] * vector.y() + m[2][1] * vector.z() + m[3][1];
	float z = m[0][2] * vector.x() + m[1][2] * vector.y() + m[2][2] * vector.z() + m[3][2];

	return Vector4(x, y, z, 1);
}

GAMEMATH_INLINE Vector4 Matrix4::mapNormal(const Vector4 &vector) const
{
	float x = m[0][0] * vector.x() + m[1][0] * vector.y() + m[2][0] * vector.z();
	float y = m[0][1] * vector.x() + m[1][1] * vector.y() + m[2][1] * vector.z();
	float z = m[0][2] * vector.x() + m[1][2] * vector.y() + m[2][2] * vector.z();

	return Vector4(x, y, z, 0);
}

GAMEMATH_INLINE Matrix4 Matrix4::transformation(const Vector4 &scale, 
									   const Quaternion &rotation, 
									   const Vector4 &translation)
{
	Matrix4 result;
	result.setToZero();
	
	// Rotation
	result.m[0][0] = scale.x() * (1 - 2 * rotation.y() * rotation.y() - 2 * rotation.z() * rotation.z());
	result.m[0][1] = scale.x() * (2 * rotation.x() * rotation.y() + 2 * rotation.w() * rotation.z());
	result.m[0][2] = scale.x() * (2 * rotation.x() * rotation.z() - 2 * rotation.w() * rotation.y());

	result.m[1][0] = scale.y() * (2 * rotation.x() * rotation.y() - 2 * rotation.w() * rotation.z());
	result.m[1][1] = scale.y() * (1 - 2 * rotation.x() * rotation.x() - 2 * rotation.z() * rotation.z());
	result.m[1][2] = scale.y() * (2 * rotation.y() * rotation.z() + 2 * rotation.w() * rotation.x());

	result.m[2][0] = scale.z() * (2 * rotation.x() * rotation.z() + 2 * rotation.w() * rotation.y());
	result.m[2][1] = scale.z() * (2 * rotation.y() * rotation.z() - 2 * rotation.w() * rotation.x());
	result.m[2][2] = scale.z() * (1 - 2 * rotation.x() * rotation.x() - 2 * rotation.y() * rotation.y());

	// Translation
	result.m[3][0] = translation.x();
	result.m[3][1] = translation.y();
	result.m[3][2] = translation.z();
	result.m[3][3] = 1;

	return result;
}

GAMEMATH_NAMESPACE_END
