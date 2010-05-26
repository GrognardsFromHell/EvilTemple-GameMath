
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
GAMEMATH_INLINE bool Ray3d::intersects(const Box3d &box, float &distance) const
{
    __m128 l1 = _mm_mul_ps(_mm_sub_ps(box.minimum().mSse, mOrigin.mSse), mInvDirection.mSse);
    __m128 l2 = _mm_mul_ps(_mm_sub_ps(box.maximum().mSse, mOrigin.mSse), mInvDirection.mSse);

    __m128 posInf = _mm_load_ps(PositiveInfinity);

	// the order we use for those min/max is vital to filter out
	// NaNs that happens when an inv_dir is +/- inf and
	// (box_min - pos) is 0. inf * 0 = NaN
    __m128 filtered_l1a = _mm_min_ps(l1, posInf);
	__m128 filtered_l2a = _mm_min_ps(l2, posInf);

    __m128 negInf = _mm_load_ps(NegativeInfinity);

	__m128 filtered_l1b = _mm_max_ps(l1, negInf);
	__m128 filtered_l2b = _mm_max_ps(l2, negInf);
        
	// now that we're back on our feet, test those slabs.
	__m128 lmax = _mm_max_ps(filtered_l1a, filtered_l2a);
	__m128 lmin = _mm_min_ps(filtered_l1b, filtered_l2b);

	// unfold back. try to hide the latency of the shufps & co.
    const __m128 lmax0 = _mm_shuffle_ps(lmax, lmax, _MM_SHUFFLE(0, 3, 2, 1));
	const __m128 lmin0 = _mm_shuffle_ps(lmin, lmin, _MM_SHUFFLE(0, 3, 2, 1));
	lmax = _mm_min_ss(lmax, lmax0);
	lmin = _mm_max_ss(lmin, lmin0);

    bool intersects = _mm_comige_ss(lmax, _mm_setzero_ps()) && _mm_comige_ss(lmax, lmin);

    float distanceFromOrigin;

    if (intersects) {
        _mm_store_ss(&distanceFromOrigin, lmin);

        if (distanceFromOrigin < distance) {
            distance = distanceFromOrigin;
        }
    }

    return intersects;
}

GAMEMATH_NAMESPACE_END
