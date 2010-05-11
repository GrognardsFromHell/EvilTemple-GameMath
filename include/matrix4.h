#ifndef MATRIX4_H
#define MATRIX4_H

#include <cstdio>

#include "gamemath.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_ALIGN class Matrix4 {
friend GAMEMATH_INLINE Matrix4 operator *(const Matrix4 &a, const Matrix4 &b);
public:
	/**
	 * Sets this matrix to the identity matrix.
	 */
	void setToIdentity();
	
	/**
	 * Sets every element of this matrix to zero.
	 */
	void setToZero();

	/**
	 * Returns the internal data of this matrix as a column-major array of 16 floating point values.
	 *
	 * This can be used to upload the matrix efficiently to OpenGL.
	 *
	 * @return A pointer to the constant internal data of this matrix.
	 */
	const float *data() const;

	/**
	 * Returns the internal data of this matrix as a column-major array of 16 floating point values.
	 *
	 * This can be used to upload the matrix efficiently to OpenGL, or read the matrix directly from a file,
	 * if the endian-ness matches.
	 *
	 * @return A pointer to the constant internal data of this matrix.
	 */
	float *data();
	
	/**
	 * Returns a single element of this matrix.
	 */
	float operator()(int row,int col) const;

	/**
	 * Returns a reference to a single element of this matrix.
	 */
	float &operator()(int row, int col);

	/**
	 * Multiplies this matrix with a scaling matrix and returns a reference to this matrix.
	 *
	 * @param vector A vector containing the scaling factors for each component.
	 *
	 * @return A reference to this matrix.
	 */
	Matrix4 &scale(const Vector4 &vector);
	
	/**
	 * Creates a 3D scaling matrix from a vector.
	 * @param vector The x, y, and z components of this vector are used as elements (0,0), (1,1) and (2,2) of the
	 * resulting matrix.
	 */
	static Matrix4 scaling(const Vector4 &vector);

	/**
	 * Creates a 3D scaling matrix from individual scaling factors.
	 */
	static Matrix4 scaling(const float sx, const float sy, const float sz);

	/**
	 * Creates a transformation matrix that will perform scaling, rotation and translation (in that order).
	 * 
	 * @param scale The x, y, and z components of this vector will be used as the scaling factors.
	 * @param rotation This rotation will be performed by the resulting matrix.
	 * @param translation The x, y, and z components of this vector will be used as the translation in the resulting
	 *					matrix.
	 */
	static Matrix4 transformation(const Vector4 &scale, const Quaternion &rotation, const Vector4 &translation);

	/**
	 * Constructs a translation matrix from the given vector.
	 *
	 * @param translation The translation vector. The w component is ignored.
	 */
	static Matrix4 translation(const Vector4 &translation);

	/**
	 * Creates a viewing matrix, that is equivalent to the matrix created by gluLookAt.
	 *
	 * @param eye The position of the camera in world space. The w coordinate must be 0.
	 * @param center The position in world space on which the camera is centered. The w coordinate must be 0.
	 * @param up The up vector of the camera. This should already be a normalized normal (w=0).
	 */
	static Matrix4 lookAt(const Vector4 &eye, const Vector4 &center, const Vector4 &up);

	/**
	 * Multiplies this scaling matrix with a scaling matrix for the x, y, and z components.

	 * @return A reference to this matrix.
	 */
	Matrix4 &scale(const float sx, const float sy, const float sz);

	/**
	 * Multiplies this matrix with a vector and returns the resulting vector.
	 *
	 * No coordinate homogenization is performed after the multiplication.
	 */
	Vector4 operator *(const Vector4 &vector) const;

	/**
	 * Transforms a position vector by this matrix. Practically, the vector's fourth component is
	 * assumed to be 1, leading to this matrices fourth column being added to the resulting vector
	 * without requiring a multiplication.
	 */
	Vector4 mapPosition(const Vector4 &vector) const;

	/**
	 * Transforms a normal vector by this matrix. Practically, the vector's fourth component is
	 * assumed to be 0, so the fourth column of this matrix doesn't need to be multiplied with the vector.
	 */
	Vector4 mapNormal(const Vector4 &vector) const;

	/**
	 * This methods behaves exactly like glOrtho and will return an ortographic projection matrix for the
	 * given values.
	 */
	static Matrix4 ortho(float left, float right, float bottom, float top, float nearVal, float farVal);

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

GAMEMATH_INLINE const float *Matrix4::data() const
{
	return &m[0][0];
}

GAMEMATH_INLINE float *Matrix4::data()
{
	return &m[0][0];
}

GAMEMATH_INLINE float Matrix4::operator()(int row, int col) const
{
	return m[col][row];
}

GAMEMATH_INLINE float &Matrix4::operator()(int row, int col)
{
	return m[col][row];
}

GAMEMATH_INLINE Matrix4 Matrix4::lookAt(const Vector4 &eye, const Vector4 &center, const Vector4 &up)
{
	Vector4 F = center - eye;
	F.normalize();

	Vector4 s = F.cross(up);
	Vector4 u = s.cross(F);
	F = -F;

	Matrix4 result;
	result.m[0][0] = s.x();
	result.m[1][0] = s.y();
	result.m[2][0] = s.z();
	result.m[3][0] = 0;

	result.m[0][1] = u.x();
	result.m[1][1] = u.y();
	result.m[2][1] = u.z();
	result.m[3][1] = 0;

	result.m[0][2] = F.x();
	result.m[1][2] = F.y();
	result.m[2][2] = F.z();
	result.m[3][2] = 0;

	result.m[0][3] = 0;
	result.m[1][3] = 0;
	result.m[2][3] = 0;
	result.m[3][3] = 1;
	
	return result * Matrix4::translation(- eye);
}

GAMEMATH_INLINE Matrix4 Matrix4::ortho(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	Matrix4 result;
	result.setToZero();
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = - 2.0f / (farVal - nearVal);

	result.m[3][0] = - (right + left) / (right - left);
	result.m[3][1] = - (top + bottom) / (top - bottom);
	result.m[3][2] = - (farVal + nearVal) / (farVal - nearVal);
	result.m[3][3] = 1;

	return result;
}

GAMEMATH_NAMESPACE_END

#if !defined(GAMEMATH_NO_INTRINSICS)
#include "matrix4_sse.h"
#else
#include "matrix4_sisd.h"
#endif

#endif // MATRIX4_H
