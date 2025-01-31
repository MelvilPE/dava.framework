#pragma once

#include "Base/BaseMath.h"
#include "Base/BaseTypes.h"

namespace DAVA
{
class JointTransform
{
public:
    /**
     * @brief Default constructor for JointTransform class.
     * 
     * Creates a JointTransform instance with default values for all members.
     */
    JointTransform() = default;
    /**
     * @brief Constructs JointTransform object from a transformation matrix
     * @param transform Matrix4 representing the joint transformation in 3D space
     * @details Creates a joint transform instance initialized with the provided 4x4 transformation matrix.
     *          The matrix contains rotation, translation, and scale information for the joint.
     */
    explicit JointTransform(const Matrix4& transform);

    /**
     * @brief Checks if this joint transform is equal to another joint transform.
     * @param other The joint transform to compare with
     * @return true if transforms are equal, false otherwise
     */
    bool operator==(const JointTransform& other) const;
    /**
     * @brief Checks if two joint transforms are not equal
     * @param other The joint transform to compare with
     * @return true if transforms are not equal, false otherwise
     */
    bool operator!=(const JointTransform& other) const;

    /**
     * @brief Resets all transform components to their default values
     *
     * Sets the translation vector to (0,0,0), rotation quaternion to identity,
     * and scale vector to (1,1,1)
     */
    void Reset();

    /**
     * @brief Sets the position of the joint transform
     * @param[in] position New position vector of the joint transform
     */
    void SetPosition(const Vector3& position);
    /**
     * @brief Sets the orientation value for the joint transform
     * @param[in] orientation The new orientation quaternion to be set for the joint
     */
    void SetOrientation(const Quaternion& orientation);
    /**
     * Sets the uniform scale factor for the joint transform.
     * @param scale The scaling factor to be applied uniformly in all dimensions (x, y, z).
     */
    void SetScale(float32 scale);

    /**
     * @brief Returns the position of the joint transform
     * @return const reference to Vector3 containing the position coordinates
     */
    const Vector3& GetPosition() const;
    /**
     * @brief Gets the joint transform's orientation
     * @return Const reference to the quaternion representing orientation
     */
    const Quaternion& GetOrientation() const;
    /**
     * Gets the joint scale value.
     * @return Scale of the joint as a floating point value.
     */
    float32 GetScale() const;
    /**
     * @brief Gets the transformation matrix for this joint.
     * @return Matrix4 containing the combined rotation, scale and translation transformations for this joint.
     */
    Matrix4 GetMatrix() const;

    /**
     * @brief Checks if the joint transform data is empty
     * @return true if transform data contains no data, false otherwise
     */
    bool IsEmpty() const;
    /**
     * @brief Checks if the joint transform has position data.
     * @return true if position data is available, false otherwise.
     */
    bool HasPosition() const;
    /**
     * @brief Checks if there is orientation data for this joint transform
     * @return true if joint transform contains orientation data, false otherwise
     */
    bool HasOrientation() const;
    /**
     * Checks whether this joint transform contains scale information.
     * @return true if the joint transform includes scale data, false otherwise
     */
    bool HasScale() const;

    /**
     * Constructs joint transformation from a matrix
     * @param transform The matrix representing the joint transformation in world space
     */
    void Construct(const Matrix4& transform);

    /**
     * @brief Appends another transform to this transform, resulting in a combined transformation.
     * 
     * This function combines two joint transformations by applying this transform first,
     * followed by the provided transform. The result represents both transformations
     * applied in sequence.
     * 
     * @param transform The JointTransform to append to the current transform
     * @return JointTransform A new transform that represents the combined transformations
     */
    JointTransform AppendTransform(const JointTransform& transform) const;
    /**
     * @brief Calculates and returns the inverse transformation of the current joint transform.
     * 
     * @details The inverse transformation reverses the effects of the current transform.
     * If T is the current transform, then T * T.GetInverse() equals the identity transform.
     * 
     * @return JointTransform The inverse of the current joint transform.
     */
    JointTransform GetInverse() const;

    /**
     * @brief Transforms a point in 3D space using the joint's transformation matrix
     * @param point The point to transform in local space coordinates
     * @return Vector3 The transformed point in global space coordinates
     */
    Vector3 ApplyToPoint(const Vector3& point) const;
    /**
    * @brief Applies skeletal transform to axis-aligned bounding box
    * @param[in] bbox Axis-aligned bounding box to transform
    * @return Transformed axis-aligned bounding box that encloses the original bbox after applying this joint transform
    */
    AABBox3 ApplyToAABBox(const AABBox3& bbox) const;

    /**
     * @brief Creates an interpolated JointTransform between two transforms
     * @param t0 The starting transform
     * @param t1 The ending transform
     * @param factor The interpolation factor (0.0 for t0, 1.0 for t1)
     * @return A JointTransform interpolated between t0 and t1 based on the factor
     */
    static JointTransform Lerp(const JointTransform& t0, const JointTransform& t1, float32 factor);
    /**
     * @brief Creates a joint transform by taking t0 value and overriding it with t1 where t1 has valid values
     * @param[in] t0 Base joint transform that will be overridden
     * @return Resulting JointTransform with t0 values overridden by t1 where applicable
     */
    static JointTransform Override(const JointTransform& t0, const JointTransform& t1);

private:
    enum eTransformFlag
    {
        FLAG_POSITION = 1 << 0,
        FLAG_ORIENTATION = 1 << 1,
        FLAG_SCALE = 1 << 2,
    };

    Quaternion orientation;
    Vector3 position;
    float32 scale = 1.f;
    uint8 flags = 0;
};

/**
 * @brief Resets the joint transform to its default state
 * 
 * Sets all transformation properties to their default values:
 * - flags to 0
 * - position to zero vector (0,0,0)
 * - orientation to identity quaternion
 * - scale to uniform 1.0
 */
inline void JointTransform::Reset()
{
    flags = 0;
    position = Vector3();
    orientation = Quaternion();
    scale = 1.f;
}

/**
 * @brief Sets the position of the joint transform.
 * @param[in] _position New position vector to be set.
 * @details Updates the position and sets the position flag to indicate the value has been modified.
 */
inline void JointTransform::SetPosition(const Vector3& _position)
{
    position = _position;
    flags |= FLAG_POSITION;
}

/**
 * @brief Sets the orientation of the joint transform
 * @param[in] _orientation New orientation quaternion to be set
 * @details Updates the transform's orientation and marks the orientation flag as modified
 */
inline void JointTransform::SetOrientation(const Quaternion& _orientation)
{
    orientation = _orientation;
    flags |= FLAG_ORIENTATION;
}

/**
 * @brief Sets the scale value for the joint transformation.
 * @param[in] _scale The scale value to be set.
 * @details This function sets the scale factor and marks the scale flag as modified in the transformation flags.
 */
inline void JointTransform::SetScale(float32 _scale)
{
    scale = _scale;
    flags |= FLAG_SCALE;
}

/**
 * @brief Returns the position of the joint transform.
 * @return A const reference to the Vector3 representing the position.
 */
inline const Vector3& JointTransform::GetPosition() const
{
    return position;
}

/**
 * @brief Returns the orientation quaternion of the joint transform.
 * @return Constant reference to the quaternion representing joint orientation.
 */
inline const Quaternion& JointTransform::GetOrientation() const
{
    return orientation;
}

/**
 * @brief Returns the scale factor of the joint transform
 * @return The scale factor as a 32-bit float value
 */
inline float32 JointTransform::GetScale() const
{
    return scale;
}

/**
 * @brief Checks if the joint transform has no active transformations.
 * @details A joint transform is considered empty when no transformation flags are set.
 * @return true if no transformations are active (flags == 0), false otherwise.
 */
inline bool JointTransform::IsEmpty() const
{
    return (flags == 0);
}

/**
 * @brief Checks if this transform has position data.
 * 
 * @return true if transform contains position data
 * @return false if transform does not contain position data
 */
inline bool JointTransform::HasPosition() const
{
    return (flags & FLAG_POSITION) == FLAG_POSITION;
}

/**
 * @brief Checks if the joint transform has orientation data.
 * @return true if the transform contains orientation data, false otherwise.
 */
inline bool JointTransform::HasOrientation() const
{
    return (flags & FLAG_ORIENTATION) == FLAG_ORIENTATION;
}

/**
 * @brief Checks if this joint transform has a scale component.
 * @return true if the transform contains scale data, false otherwise.
 */
inline bool JointTransform::HasScale() const
{
    return (flags & FLAG_SCALE) == FLAG_SCALE;
}

/**
 * @brief Applies the joint transformation to a point in 3D space.
 * 
 * This function transforms a point by applying:
 * 1. Scale
 * 2. Rotation (using the orientation quaternion)
 * 3. Translation (position offset)
 * 
 * @param inVec The input point vector to transform
 * @return Vector3 The transformed point in 3D space
 */
inline Vector3 JointTransform::ApplyToPoint(const Vector3& inVec) const
{
    return position + orientation.ApplyToVectorFast(inVec) * scale;
}

/**
* Compares this JointTransform with another for equality.
* Two JointTransforms are considered equal if they have identical flags, orientation, position and scale.
* 
* @param other The JointTransform to compare with
* @return true if the transforms are identical, false otherwise
*/
inline bool JointTransform::operator==(const JointTransform& other) const
{
    return (flags == other.flags) && (orientation == other.orientation) && (position == other.position) && (scale == other.scale);
}

/**
* @brief Inequality operator that compares this JointTransform with another.
* @param other The JointTransform to compare with.
* @return true if the transforms are not equal, false otherwise.
*/
inline bool JointTransform::operator!=(const JointTransform& other) const
{
    return !(*this == other);
}

/**
 * @brief Constructs joint transformation data from a 4x4 transformation matrix
 * 
 * Decomposes the input transformation matrix into position, orientation and scale components.
 * Sets all transformation flags (position, orientation and scale) to indicate valid data.
 * Note: Only uniform scale is supported (scale.x is used for all axes).
 * 
 * @param transform Input 4x4 transformation matrix to decompose
 */
inline void JointTransform::Construct(const Matrix4& transform)
{
    Vector3 scale3;
    transform.Decomposition(position, scale3, orientation);
    scale = scale3.x;

    flags = FLAG_POSITION | FLAG_ORIENTATION | FLAG_SCALE;
}

} //ns