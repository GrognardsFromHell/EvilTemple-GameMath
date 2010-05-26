
#include "gamemath_internal.h"
#include "box3d.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE void Box3d::setToInfinity()
{
    mMinimum.mSse = _mm_load_ps(NegativeInfinity);
    mMaximum.mSse = _mm_load_ps(PositiveInfinity);
}

GAMEMATH_NAMESPACE_END
