
// This is for IDEs only
#include "gamemath.h"

#if !defined(MATRIX4_H)
#error "Do not include this file directly, only include matrix4.h"
#endif

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE Matrix4 &Matrix4::scale(const Vector4 &vector)
{
	columns[0] = _mm_mul_ps(columns[0], _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(0, 0, 0, 0)));
	columns[1] = _mm_mul_ps(columns[1], _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(1, 1, 1, 1)));
	columns[2] = _mm_mul_ps(columns[2], _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(2, 2, 2, 2)));
	columns[3] = _mm_mul_ps(columns[3], _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(3, 3, 3, 3)));
	return *this;
}

GAMEMATH_INLINE Matrix4 &Matrix4::scale(const float sx, const float sy, const float sz)
{
	__m128 vector = _mm_set_ps(sx, sy, sz, 0);
	columns[0] = _mm_mul_ps(columns[0], _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(0, 0, 0, 0)));
	columns[1] = _mm_mul_ps(columns[1], _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(1, 1, 1, 1)));
	columns[2] = _mm_mul_ps(columns[2], _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(2, 2, 2, 2)));
	return *this;
}

GAMEMATH_INLINE Matrix4 Matrix4::scaling(const Vector4 &vector)
{
	Matrix4 result;

	result.columns[0] = _mm_and_ps(vector.mSse, *reinterpret_cast<const __m128*>(&CoordinateMaskX));
	result.columns[1] = _mm_and_ps(vector.mSse, *reinterpret_cast<const __m128*>(&CoordinateMaskY));
	result.columns[2] = _mm_and_ps(vector.mSse, *reinterpret_cast<const __m128*>(&CoordinateMaskZ));
	result.columns[3] = *reinterpret_cast<const __m128*>(IdentityCol4);
		
	return result;
}

GAMEMATH_INLINE Matrix4 Matrix4::scaling(const float sx, const float sy, const float sz)
{
	Vector4 vector(sx, sy, sz, 0);
	return scaling(vector);
}

GAMEMATH_INLINE void Matrix4::setToIdentity()
{
	columns[0] = _mm_load_ps(IdentityCol1);
	columns[1] = _mm_load_ps(IdentityCol2);
	columns[2] = _mm_load_ps(IdentityCol3);
	columns[3] = _mm_load_ps(IdentityCol4);
}

GAMEMATH_INLINE void Matrix4::setToZero()
{
	columns[0] = _mm_setzero_ps();
	columns[1] = _mm_setzero_ps();
	columns[2] = _mm_setzero_ps();
	columns[3] = _mm_setzero_ps();
}

GAMEMATH_INLINE Vector4 Matrix4::operator *(const Vector4 &vector) const
{
	Vector4 result;

	// Splat the X component into all components of a new vector, and multiply it with the first column
	__m128 tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(0, 0, 0, 0));
	result.mSse = _mm_mul_ps(columns[0], tmp);

	// Repeat the above with the y column/component
	tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(1, 1, 1, 1));
	result.mSse = _mm_add_ps(result.mSse, _mm_mul_ps(columns[1], tmp));

	// Repeat the above with the z column/component
	tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(2, 2, 2, 2));
	result.mSse = _mm_add_ps(result.mSse, _mm_mul_ps(columns[2], tmp));

	// Repeat the above with the w column/component
	tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(3, 3, 3, 3));
	result.mSse = _mm_add_ps(result.mSse, _mm_mul_ps(columns[3], tmp));
	
	return result;
}

GAMEMATH_INLINE Vector4 Matrix4::mapPosition(const Vector4 &vector) const
{
	Vector4 result;

	// Splat the X component into all components of a new vector, and multiply it with the first column
	__m128 tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(0, 0, 0, 0));
	result.mSse = _mm_mul_ps(columns[0], tmp);

	// Repeat the above with the y column/component
	tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(1, 1, 1, 1));
	result.mSse = _mm_add_ps(result.mSse, _mm_mul_ps(columns[1], tmp));

	// Repeat the above with the z column/component
	tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(2, 2, 2, 2));
	result.mSse = _mm_add_ps(result.mSse, _mm_mul_ps(columns[2], tmp));

	result.mSse = _mm_add_ps(result.mSse, columns[3]);

	return result;
}

GAMEMATH_INLINE Vector4 Matrix4::mapNormal(const Vector4 &vector) const
{
	Vector4 result;

	// Splat the X component into all components of a new vector, and multiply it with the first column
	__m128 tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(0, 0, 0, 0));
	result.mSse = _mm_mul_ps(columns[0], tmp);

	// Repeat the above with the y column/component
	tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(1, 1, 1, 1));
	result.mSse = _mm_add_ps(result.mSse, _mm_mul_ps(columns[1], tmp));

	// Repeat the above with the z column/component
	tmp =  _mm_shuffle_ps(vector.mSse, vector.mSse, _MM_SHUFFLE(2, 2, 2, 2));
	result.mSse = _mm_add_ps(result.mSse, _mm_mul_ps(columns[2], tmp));
	
	return result;
}

GAMEMATH_INLINE void Matrix4::print() const
{
	const Matrix4 &m = *this;

	for (int i = 0; i < 4; ++i) {
		printf("%f %f %f %f\n", m(i, 0), m(i, 1), m(i, 2), m(i, 3));
	}
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

GAMEMATH_INLINE Matrix4 Matrix4::rotation(const Quaternion &rotation)
{
	Matrix4 result;
	result.setToZero();

	// Rotation
	result.m[0][0] = (1 - 2 * rotation.y() * rotation.y() - 2 * rotation.z() * rotation.z());
	result.m[0][1] = (2 * rotation.x() * rotation.y() + 2 * rotation.w() * rotation.z());
	result.m[0][2] = (2 * rotation.x() * rotation.z() - 2 * rotation.w() * rotation.y());

	result.m[1][0] = (2 * rotation.x() * rotation.y() - 2 * rotation.w() * rotation.z());
	result.m[1][1] = (1 - 2 * rotation.x() * rotation.x() - 2 * rotation.z() * rotation.z());
	result.m[1][2] = (2 * rotation.y() * rotation.z() + 2 * rotation.w() * rotation.x());

	result.m[2][0] = (2 * rotation.x() * rotation.z() + 2 * rotation.w() * rotation.y());
	result.m[2][1] = (2 * rotation.y() * rotation.z() - 2 * rotation.w() * rotation.x());
	result.m[2][2] = (1 - 2 * rotation.x() * rotation.x() - 2 * rotation.y() * rotation.y());

	result.m[3][3] = 1;

	return result;
}

GAMEMATH_INLINE Matrix4 operator *(const Matrix4 &m1, const Matrix4 &m2)
{
	__m128 resultColumn;

	Matrix4 result;

	resultColumn = _mm_mul_ps(m1.columns[0], _mm_set1_ps(m2.m[0][0]));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[1], _mm_set1_ps(m2.m[0][1])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[2], _mm_set1_ps(m2.m[0][2])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[3], _mm_set1_ps(m2.m[0][3])));
	result.columns[0] = resultColumn;

	resultColumn = _mm_mul_ps(m1.columns[0], _mm_set1_ps(m2.m[1][0]));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[1], _mm_set1_ps(m2.m[1][1])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[2], _mm_set1_ps(m2.m[1][2])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[3], _mm_set1_ps(m2.m[1][3])));
	result.columns[1] = resultColumn;

	resultColumn = _mm_mul_ps(m1.columns[0], _mm_set1_ps(m2.m[2][0]));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[1], _mm_set1_ps(m2.m[2][1])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[2], _mm_set1_ps(m2.m[2][2])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[3], _mm_set1_ps(m2.m[2][3])));
	result.columns[2] = resultColumn;

	resultColumn = _mm_mul_ps(m1.columns[0], _mm_set1_ps(m2.m[3][0]));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[1], _mm_set1_ps(m2.m[3][1])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[2], _mm_set1_ps(m2.m[3][2])));
	resultColumn = _mm_add_ps(resultColumn, _mm_mul_ps(m1.columns[3], _mm_set1_ps(m2.m[3][3])));
	result.columns[3] = resultColumn;

	return result;
}

GAMEMATH_INLINE Matrix4 Matrix4::translation(const Vector4 &translation)
{
	Matrix4 result;
	result.columns[0] = _mm_load_ps(IdentityCol1);
	result.columns[1] = _mm_load_ps(IdentityCol2);
	result.columns[2] = _mm_load_ps(IdentityCol3);
	result.columns[3] = _mm_add_ps(_mm_load_ps(IdentityCol4), translation.mSse);
	return result;
}

GAMEMATH_INLINE Matrix4 Matrix4::translation(float x, float y, float z, float w)
{
	Matrix4 result;
	result.columns[0] = _mm_load_ps(IdentityCol1);
	result.columns[1] = _mm_load_ps(IdentityCol2);
	result.columns[2] = _mm_load_ps(IdentityCol3);
	result.columns[3] = _mm_add_ps(_mm_load_ps(IdentityCol4), _mm_set_ps(w, z, y, x));
	return result;
}

GAMEMATH_NAMESPACE_END
