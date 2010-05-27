
#include "gamemath_internal.h"
#include "box3d.h"

#include <limits>

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE Box3d Box3d::transformAffine(const Matrix4 &matrix) const
{
    // Build a new box centered at the translated origin
    Box3d newBox(matrix.column(3), matrix.column(3));

    // This is based on a gamedev forum post.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            float av = matrix(j, i) * mMinimum.data()[j];
            float bv = matrix(j, i) * mMaximum.data()[j];

            if (av < bv) {
                newBox.mMinimum.data()[i] += av;
                newBox.mMaximum.data()[i] += bv;
            } else {
                newBox.mMinimum.data()[i] += bv;
                newBox.mMaximum.data()[i] += av;
            }
        }
    }
    
	return newBox;
}

GAMEMATH_INLINE void Box3d::setToInfinity()
{
    const float inf = std::numeric_limits<float>::infinity();

    mMinimum.setX(- inf);
    mMinimum.setY(- inf );
    mMinimum.setZ(- inf );

    mMaximum.setX(inf);
    mMaximum.setY(inf);
    mMaximum.setZ(inf);
}

GAMEMATH_NAMESPACE_END
