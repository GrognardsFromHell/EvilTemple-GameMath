
#include "gamemath_internal.h"
#include "box3d.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE void Box3d::setToInfinity()
{
    *reinterpret_cast<__m128*>(&mMinimum) = _mm_load_ps(reinterpret_cast<const float*>(NegativeInfinity));
    *reinterpret_cast<__m128*>(&mMaximum) = _mm_load_ps(reinterpret_cast<const float*>(PositiveInfinity));
}

GAMEMATH_NAMESPACE_END
