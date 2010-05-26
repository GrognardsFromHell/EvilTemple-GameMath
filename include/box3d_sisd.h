
#include "gamemath_internal.h"
#include "box3d.h"

#include <limits>

GAMEMATH_NAMESPACE_BEGIN

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
