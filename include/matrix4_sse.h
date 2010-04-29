
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

GAMEMATH_INLINE const float *Matrix4::data() const
{
	return reinterpret_cast<const float*>(this);
}

GAMEMATH_INLINE float Matrix4::operator()(int row, int col) const
{
	return *(data() + col * 4 + row);
}

GAMEMATH_INLINE void Matrix4::setToIdentity()
{
	columns[0] = _mm_load_ps(IdentityCol1);
	columns[1] = _mm_load_ps(IdentityCol2);
	columns[2] = _mm_load_ps(IdentityCol3);
	columns[3] = _mm_load_ps(IdentityCol4);
}

GAMEMATH_INLINE void Matrix4::print() const
{
	const Matrix4 &m = *this;

	for (int i = 0; i < 4; ++i) {
		printf("%f %f %f %f\n", m(i, 0), m(i, 1), m(i, 2), m(i, 3));
	}
}

GAMEMATH_NAMESPACE_END
