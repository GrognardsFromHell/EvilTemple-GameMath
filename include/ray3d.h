
#ifndef RAY3D_H
#define RAY3D_H

#include "gamemath_internal.h"
#include "vector4.h"
#include "matrix4.h"

GAMEMATH_NAMESPACE_BEGIN

struct IntersectionResult {
    bool intersects;
    float distance;
};

/**
  Represents a ray in three dimensonal space.
  */
class Ray3d {
public:
    Ray3d();
    Ray3d(const Vector4 &origin, const Vector4 &direction);

    const Vector4 &origin() const;
    const Vector4 &direction() const;
    const Vector4 &invertedDirection() const;

    void setOrigin(const Vector4 &origin);
    void setDirection(const Vector4 &direction);

    bool intersectsSphere(const Vector4 &sphereOrigin, float sphereRadiusSquare) const;
    bool intersects(const Box3d &box, float &distance) const;
    
private:
    Vector4 mOrigin;
    Vector4 mDirection;
    Vector4 mInvDirection;
};

GAMEMATH_INLINE bool Ray3d::intersectsSphere(const Vector4 &sphereOrigin, float sphereRadiusSquare) const
{
    Vector4 sphereToOrigin = sphereOrigin - mOrigin;
    float distanceToSphereCenter = sphereToOrigin.lengthSquared();

    if (distanceToSphereCenter < sphereRadiusSquare) {
        return true; // Origin of the ray is within the sphere -> ofcourse it intersects
    }

    float angle = mDirection.dot(sphereToOrigin);

    if (angle >= 0) {
        return false; // The direction points away from the sphere.
    }

    return angle * angle >= distanceToSphereCenter; // TODO: Not entirely sure why this works
}

/**
  Transforms a ray using a matrix.
 */
GAMEMATH_INLINE Ray3d operator *(const Matrix4 &matrix, const Ray3d &ray)
{
    Ray3d result;

    result.setOrigin(matrix.mapPosition(ray.origin()));
    result.setDirection(matrix.mapNormal(ray.direction()));

    return result;
}

GAMEMATH_INLINE Ray3d::Ray3d()
{
}

GAMEMATH_INLINE const Vector4 &Ray3d::origin() const
{
    return mOrigin;
}

GAMEMATH_INLINE const Vector4 &Ray3d::direction() const
{
    return mDirection;
}

GAMEMATH_INLINE void Ray3d::setOrigin(const Vector4 &origin)
{
    mOrigin = origin;
}

GAMEMATH_NAMESPACE_END
    
#if !defined(GAMEMATH_NO_INTRINSICS)
#include "ray3d_sse.h"
#endif

#endif // RAY3D_H
