
#include "gamemath_internal.h"
#include "ray3d.h"
#include "box3d.h"

GAMEMATH_NAMESPACE_BEGIN

GAMEMATH_INLINE Ray3d::Ray3d(const Vector4 &origin, const Vector4 &direction)
    : mOrigin(origin), mDirection(direction)
{
    mInvDirection.mSse = _mm_rcp_ps(direction.mSse);
}

GAMEMATH_INLINE void Ray3d::setDirection(const Vector4 &direction)
{
    mDirection = direction;
    mInvDirection.mSse = _mm_rcp_ps(direction.mSse);
}

/*
  Based on work @ http://www.flipcode.com/archives/SSE_RayBox_Intersection_Test.shtml
  && http://www.uni-koblenz.de/~cg/publikationen/cp_raytrace.pdf
*/
GAMEMATH_INLINE bool Ray3d::intersects(const Box3d &box) const
{
    const __m128 temp1 = _mm_mul_ps(_mm_sub_ps(box.minimum(), mOrigin), mInvDirection);
    const __m128 temp2 = _mm_mul_ps(_mm_sub_ps(box.maximum(), mOrigin), mInvDirection);

    __m128 minVec = _mm_min_ps(temp1, temp2);
    __m128 maxVec = _mm_max_ps(temp1, temp2);

    // This sse code tries to find the smallest component of a vector without branching

    // Get the y component into the lower reg
    const __m128 minVecY = _mm_shuffle_ps(minVec, minVec, _MM_SHUFFLE(3, 2, 0, 1));
    minVec = _mm_max_ss(minVec, minVecY); // r0 is max(minX,minY)

    const __m128 maxVecY = _mm_shuffle_ps(maxVec, maxVec, _MM_SHUFFLE(3, 2, 0, 1));
    maxVec = _mm_min_ss(maxVec, maxVecY); // r0 is min(maxX,maxY)

    const __m128 minVecZ = _mm_movehl_ps(minVec, minVec); // r0 is now r3 (which is minZ)
    minVec = _mm_max_ss(minVec, minVecZ); // r0 is max(max(minX,minY),minZ)

    const __m128 maxVecZ = _mm_movehl_ps(maxVec, maxVec); // r0 is now r3 (which is maxZ)
    maxVec = _mm_min_ss(maxVec, maxVecZ); // r0 is min(min(maxX,maxY),maxZ)

    if (_mm_comigt_ss(minVec, maxVec) || _mm_comilt_ss(maxVec, _mm_setzero_ps()))
        return false;

    return true;
}

GAMEMATH_NAMESPACE_END
