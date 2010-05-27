
#include "gamemath_internal.h"
#include "box3d.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE void Box3d::setToInfinity()
{
    mMinimum.mSse = _mm_load_ps(NegativeInfinity);
    mMaximum.mSse = _mm_load_ps(PositiveInfinity);
}

GAMEMATH_INLINE Box3d Box3d::transformAffine(const Matrix4 &matrix) const
{
    // TODO: Accelerate this

    // Build a new box centered at the translated origin
    Vector4 newMin = matrix.column(3);
    Vector4 newMax = newMin;

    // This is based on a gamedev forum post.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            float av = matrix(j, i) * mMinimum.data()[j];
            float bv = matrix(j, i) * mMaximum.data()[j];

            if (av < bv) {
                newMin.data()[i] += av;
                newMax.data()[i] += bv;
            } else {
                newMin.data()[i] += bv;
                newMax.data()[i] += av;
            }
        }
    }
    
	return Box3d(newMin, newMax);
}

GAMEMATH_NAMESPACE_END
