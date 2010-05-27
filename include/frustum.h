
#include "gamemath_internal.h"
#include "vector4.h"
#include "matrix4.h"
#include "box3d.h"

GAMEMATH_NAMESPACE_BEGIN

/**
  Describes a frustum with 6 planes. It can be used for view-frustum culling by 
  extracting a frustum from the view-projection matrix and calling isVisible to
  test whether an object should be drawn.
  */
class Frustum {
public:
    /**
      The planes maintained by this frustum.
      */
    enum ClippingPlane {
        Plane_Left = 0,
        Plane_Right,
        Plane_Top,
        Plane_Bottom,
        Plane_Near,
        Plane_Far
    };

    /**
      Extracts a frustum from a given transformation matrix.
      The space in which the planes are given depends on the matrix.
      If matrix is the model view projection matrix, this frustum
      will be in world coordinates.
      */
    void extract(const Matrix4 &matrix);

    /**
       Checks whether a given bounding box is visible within this frustum.
       Please note that the box and the frustum must be in the same space
       for this function to succeed.
      */
    bool isVisible(const Box3d &boundingBox) const;

private:
    Vector4 mPlanes[6];

};

GAMEMATH_INLINE Vector4 normalizePlane(const Vector4 &plane)
{
    Vector4 result = plane;
    float l = 1 / sqrt(result.x() * result.x() + result.y() * result.y() + result.z() * result.z());
    result.setX(result.x() * l);
    result.setY(result.y() * l);
    result.setZ(result.z() * l);
    result.setW(result.w() * l);
    return result;
}

GAMEMATH_INLINE void Frustum::extract(const Matrix4 &transformMatrix)
{
    const Matrix4 matrix = transformMatrix.transposed();

    /*
    This is inspired by: "Fast Extraction of Viewing Frustum Planes from the World-
    View-Projection Matrix" by Gil Gribb and Klaus Hartmann.
    */
    mPlanes[Plane_Left] = normalizePlane(matrix.column(3) + matrix.column(0));
    mPlanes[Plane_Right] = normalizePlane(matrix.column(3) - matrix.column(0));
    mPlanes[Plane_Top] = normalizePlane(matrix.column(3) + matrix.column(1));
    mPlanes[Plane_Bottom] = normalizePlane(matrix.column(3) - matrix.column(1));
    mPlanes[Plane_Near] = normalizePlane(matrix.column(3) + matrix.column(2));
    mPlanes[Plane_Far] = normalizePlane(matrix.column(3) - matrix.column(2));
}

GAMEMATH_INLINE bool Frustum::isVisible(const Box3d &boundingBox) const
{
    const Vector4 minPoint = boundingBox.minimum();
    const Vector4 maxPoint = boundingBox.maximum();

    // this is implemented according to Realtime Rendering 3rd edition, p756

    Vector4 c = 0.5f * (maxPoint + minPoint);
    c.setW(1);
    Vector4 h = 0.5f * (maxPoint - minPoint);
    h.setW(0);
    
    for (int i = Plane_Left; i <= Plane_Far; ++i) {
        const Vector4 &plane = mPlanes[i];

        float e = Vector4(plane.x(), plane.y(), plane.z(), 0).absolute().dot(h);
        float s = plane.dot(c);
        
        if (s + e < 0)
            return false;
    }

    return true;
}

GAMEMATH_NAMESPACE_END
