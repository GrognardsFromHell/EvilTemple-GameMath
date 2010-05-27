
#ifndef BOX3D_H
#define BOX3D_H

#include "gamemath_internal.h"
#include "matrix4.h"
#include "vector4.h"

GAMEMATH_NAMESPACE_BEGIN

class Box3d {
public:
    Box3d(const Vector4 &minimum, const Vector4 &maximum);
    Box3d();

    const Vector4 &minimum() const;
    const Vector4 &maximum() const;
    void setMinimum(const Vector4 &minimum);
    void setMaximum(const Vector4 &maximum);

    Vector4 center() const;

    /**
      Under the assumption that the given matrix is an affine transform, this produces a new
      axis aligned bounding box.
      */
    Box3d transformAffine(const Matrix4 &matrix) const;

    /**
    mMinimum is equal to mMaximum.
    */
    bool isNull() const;

    void setToInfinity();

    bool isInfinite() const;
    
    void merge(const Box3d &other);

    void merge(const Vector4 &point);

    bool intersects(const Box3d &other) const;

    bool contains(const Vector4 &point) const;
private:
    Vector4 mMinimum;
    Vector4 mMaximum;
};

GAMEMATH_INLINE Box3d::Box3d() 
    : mMinimum(0, 0, 0, 0), mMaximum(0, 0, 0, 0)
{
}

GAMEMATH_INLINE Box3d::Box3d(const Vector4 &minimum, const Vector4 &maximum)
    : mMinimum(minimum), mMaximum(maximum)
{
}

GAMEMATH_INLINE void Box3d::merge(const Box3d &other)
{
    if (other.mMinimum.x() < mMinimum.x())
        mMinimum.setX(other.mMinimum.x());
    if (other.mMinimum.y() < mMinimum.y())
        mMinimum.setY(other.mMinimum.y());
    if (other.mMinimum.z() < mMinimum.z())
        mMinimum.setZ(other.mMinimum.z());

    if (other.mMaximum.x() > mMaximum.x())
        mMaximum.setX(other.mMaximum.x());
    if (other.mMaximum.y() > mMaximum.y())
        mMaximum.setY(other.mMaximum.y());
    if (other.mMaximum.z() > mMaximum.z())
        mMaximum.setZ(other.mMaximum.z());
}

GAMEMATH_INLINE void Box3d::merge(const Vector4 &point)
{
    if (point.x() < mMinimum.x())
        mMinimum.setX(point.x());
    else if (point.x() > mMaximum.x()) 
        mMaximum.setX(point.x());

    if (point.y() < mMinimum.y())
        mMinimum.setY(point.y());
    else if (point.y() > mMaximum.y()) 
        mMaximum.setY(point.y());

    if (point.z() < mMinimum.z())
        mMinimum.setZ(point.z());
    else if (point.z() > mMaximum.z()) 
        mMaximum.setZ(point.z());
}

GAMEMATH_INLINE Box3d operator *(const Matrix4 &matrix, const Box3d &box)
{
    Vector4 tMin = matrix.mapPosition(box.minimum());
    Vector4 tMax = matrix.mapPosition(box.maximum());
    return Box3d(tMin, tMax);
}

GAMEMATH_INLINE const Vector4 &Box3d::minimum() const
{
    return mMinimum;
}

GAMEMATH_INLINE const Vector4 &Box3d::maximum() const
{
    return mMaximum;
}

GAMEMATH_INLINE void Box3d::setMinimum(const Vector4 &minimum)
{
    mMinimum = minimum;
}

GAMEMATH_INLINE void Box3d::setMaximum(const Vector4 &maximum)
{
    mMaximum = maximum;
}

GAMEMATH_INLINE bool Box3d::isNull() const
{
    return mMinimum == mMaximum;
}

GAMEMATH_INLINE bool Box3d::intersects(const Box3d &other) const
{
    // TODO: This can be SSE accelerated
    return !(
        mMinimum.x() > other.mMaximum.x() || 
        mMinimum.y() > other.mMaximum.y() || 
        mMinimum.z() > other.mMaximum.z() || 
        other.mMinimum.x() > mMaximum.x() || 
        other.mMinimum.y() > mMaximum.y() || 
        other.mMinimum.z() > mMaximum.z()
        );
}

GAMEMATH_INLINE bool Box3d::contains(const Vector4 &point) const
{
    // TODO: This can be SSE accelerated
    return !(
        mMinimum.x() > point.x() || 
        mMinimum.y() > point.y() || 
        mMinimum.z() > point.z() || 
        point.x() > mMaximum.x() || 
        point.y() > mMaximum.y() || 
        point.z() > mMaximum.z()
        );
}

GAMEMATH_INLINE bool Box3d::isInfinite() const
{
    return mMinimum.isInfinite() || mMaximum.isInfinite();
}

GAMEMATH_INLINE Vector4 Box3d::center() const
{
    return 0.5f * (mMinimum + mMaximum);
}

GAMEMATH_NAMESPACE_END

#if !defined(GAMEMATH_NO_INTRINSICS)
#include "box3d_sse.h"
#else
#include "box3d_sisd.h"
#endif

#endif // BOX3D_H
