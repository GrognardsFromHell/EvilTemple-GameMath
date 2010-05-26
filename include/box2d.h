#ifndef BOX2D_H
#define BOX2D_H

#include <algorithm>

#include "gamemath_internal.h"

GAMEMATH_NAMESPACE_BEGIN

/**
 * Represents a two-dimensional box.
 */
GAMEMATH_ALIGN class Box2d {
public:
	/**
	 * Constructs a two-dimensional box from its bounds.
	 *
	 * Arguments are automatically swapped to ensure left<right and top<bottom.
	 */
	Box2d(float left, float top, float right, float bottom);

	float left() const;
	float top() const;
	float right() const;
	float bottom() const;

	/**
	 * Determines whether this box intersects with another two-dimensional box.
	 *
	 * Two boxes intersect each other when their surface intersects.
	 */
	bool intersects(const Box2d &other) const;

private:
	float mLeft;
	float mTop;
	float mRight;
	float mBottom;
};

GAMEMATH_INLINE Box2d::Box2d(float left, float top, float right, float bottom)
	: mLeft(left), mTop(top), mRight(right), mBottom(bottom)
{
	// Auto-Swap left/right
	if (mRight < mLeft) {
		std::swap(mRight, mLeft);
	}
	if (mBottom < mTop) {
		std::swap(mBottom, mTop);
	}
}

GAMEMATH_INLINE bool Box2d::intersects(const Box2d &other) const
{
	return !(other.mRight <= mLeft
		|| other.mBottom <= mTop
		|| other.mLeft >= mRight
		|| other.mTop >= mBottom);
}

GAMEMATH_INLINE float Box2d::left() const
{
	return mLeft;
}

GAMEMATH_INLINE float Box2d::top() const
{
	return mTop;
}

GAMEMATH_INLINE float Box2d::right() const
{
	return mRight;
}

GAMEMATH_INLINE float Box2d::bottom() const
{
	return mBottom;
}

GAMEMATH_NAMESPACE_END

#endif // BOX2D_H
