#pragma once

#include "Base/BaseTypes.h"
#include "Scene3D/SkeletonAnimation/JointTransform.h"

namespace DAVA
{
class SkeletonPose
{
public:
    /**
     * @brief Constructor that creates a skeleton pose with specified number of joints
     * @param[in] jointCount Initial number of joints in the skeleton pose. Default is 0
     */
    SkeletonPose(uint32 jointCount = 0);

    /**
     * @brief Sets the number of joints in the skeleton pose.
     * @param jointCount The total number of joints to be set in the skeleton.
     * @note This method presumably allocates or resizes internal storage for joints.
     */
    void SetJointCount(uint32 jointCount);
    /**
     * @return Number of joints in the skeleton pose.
     */
    uint32 GetJointsCount() const;

    /**
     * @brief Resets the skeleton pose to its initial state.
     * 
     * Resets all joint transformations to their default/bind pose values and clears any
     * applied animations or modifications. After calling this method, the skeleton will
     * be in its original reference pose.
     */
    void Reset();

    /**
     * @brief Sets the transformation for a specific joint in the skeleton.
     * @param jointIndex The index of the joint to set the transformation for.
     * @param transform The new transformation to apply to the joint.
     * 
     * This function updates the transformation (position, rotation, and scale) of a single joint
     * in the skeleton structure. The joint is identified by its index in the skeleton hierarchy.
     */
    void SetTransform(uint32 jointIndex, const JointTransform& transform);

    /**
     * @brief Sets the position of a specific joint in the skeleton pose
     * @param jointIndex Index of the joint to set the position for
     * @param position The new position vector to set for the joint
     */
    void SetPosition(uint32 jointIndex, const Vector3& position);
    /**
     * @brief Sets the orientation/rotation of a specific joint in the skeleton pose.
     * @param jointIndex Index of the joint whose orientation is to be modified.
     * @param orientation New orientation value represented as a quaternion.
     */
    void SetOrientation(uint32 jointIndex, const Quaternion& orientation);
    /**
     * @brief Sets the scale value for the specified joint in the skeleton pose
     * @param jointIndex Index of the joint to set scale for
     * @param scale Scale value to be applied to the joint
     */
    void SetScale(uint32 jointIndex, float32 scale);

    /**
     * @brief Returns the transformation matrix for a specific joint in the skeleton.
     * @param jointIndex Index of the joint in the skeleton hierarchy.
     * @return Reference to the joint transform containing rotation, translation and scale.
     * @throw None
     */
    const JointTransform& GetJointTransform(uint32 jointIndex) const;

    /**
    * Adds bone transformations from another skeleton pose to this one.
    * @param[in] other The skeleton pose whose transformations will be added to the current pose
    */
    void Add(const SkeletonPose& other);
    /**
     * @brief Calculates the difference between this pose and another skeleton pose
     * 
     * @param[in] other The skeleton pose to compare with the current pose
     * 
     * This function computes the differences between joint transformations
     * of the current pose and the provided skeleton pose.
     */
    void Diff(const SkeletonPose& other);
    /**
     * @brief Overrides current skeleton pose with data from another pose
     * @param[in] other Source skeleton pose to copy data from
     * 
     * Copies all bone transformations and joint states from the provided pose,
     * completely replacing current pose data. Both poses must have compatible
     * skeleton structures for correct operation.
     */
    void Override(const SkeletonPose& other);
    /**
     * @brief Linearly interpolates between this pose and another skeleton pose
     * @param other The target skeleton pose to interpolate towards
     * @param factor Interpolation factor in range [0,1]. 0 means this pose, 1 means other pose
     * 
     * Performs linear interpolation between joint transforms of this pose and the other pose.
     * The resulting pose will be a blend between the two poses based on the factor value.
     */
    void Lerp(const SkeletonPose& other, float32 factor);

private:
    Vector<JointTransform> jointTransforms;
};

/**
 * @brief Sets the number of joints in the skeleton pose
 * 
 * Resizes the joint transforms array to accommodate the specified number of joints.
 * Any new joints added will be initialized with default joint transform values.
 * 
 * @param jointCount The number of joints to set for this skeleton pose
 */
inline void SkeletonPose::SetJointCount(uint32 jointCount)
{
    jointTransforms.resize(jointCount, JointTransform());
}

/**
 * @brief Returns the total number of joints in the skeleton pose
 * @return The number of joints as a uint32 value
 */
inline uint32 SkeletonPose::GetJointsCount() const
{
    return uint32(jointTransforms.size());
}

/**
 * @brief Resets all joint transforms in the skeleton pose to their default state
 * 
 * Iterates through all joint transforms in the pose and calls Reset() on each one,
 * effectively returning the entire skeleton pose to its initial configuration.
 */
inline void SkeletonPose::Reset()
{
    for (JointTransform& t : jointTransforms)
        t.Reset();
}

/**
 * @brief Sets the transform for a specific joint in the skeleton pose
 * 
 * If the given joint index exceeds the current joint count, the joint array
 * will be automatically resized to accommodate the new joint.
 * 
 * @param[in] jointIndex Index of the joint to set the transform for
 * @param[in] transform New transform to be applied to the joint
 */
inline void SkeletonPose::SetTransform(uint32 jointIndex, const JointTransform& transform)
{
    if (GetJointsCount() <= jointIndex)
        SetJointCount(jointIndex + 1);

    jointTransforms[jointIndex] = transform;
}

/**
 * @brief Sets the position for a specific joint in the skeleton pose
 * 
 * If the requested joint index is larger than the current joint count,
 * the pose will be automatically resized to accommodate the new joint.
 * 
 * @param[in] jointIndex Index of the joint to set position for
 * @param[in] position New position vector to set for the joint
 */
inline void SkeletonPose::SetPosition(uint32 jointIndex, const Vector3& position)
{
    if (GetJointsCount() <= jointIndex)
        SetJointCount(jointIndex + 1);

    jointTransforms[jointIndex].SetPosition(position);
}

/**
 * @brief Sets the orientation of a specific joint in the skeleton pose
 * 
 * If the requested joint index is beyond the current joint count, the pose will be
 * automatically expanded to accommodate the new joint index.
 * 
 * @param[in] jointIndex Index of the joint to modify
 * @param[in] orientation New orientation quaternion for the joint
 */
inline void SkeletonPose::SetOrientation(uint32 jointIndex, const Quaternion& orientation)
{
    if (GetJointsCount() <= jointIndex)
        SetJointCount(jointIndex + 1);

    jointTransforms[jointIndex].SetOrientation(orientation);
}

/**
 * @param jointIndex Index of the joint to set the scale for
 * @brief Sets uniform scale value for a specific joint
 */
inline void SkeletonPose::SetScale(uint32 jointIndex, float32 scale)
{
    if (GetJointsCount() <= jointIndex)
        SetJointCount(jointIndex + 1);

    jointTransforms[jointIndex].SetScale(scale);
}

/**
 * @brief Gets the transform for a specific joint in the skeleton pose.
 * @details Returns a static empty transform if the joint index is out of bounds.
 * 
 * @param jointIndex Index of the joint to retrieve the transform for
 * @return const JointTransform& Transform of the specified joint or empty transform if index is invalid
 */
inline const JointTransform& SkeletonPose::GetJointTransform(uint32 jointIndex) const
{
    static JointTransform emptyTransform;

    if (jointIndex < GetJointsCount())
        return jointTransforms[jointIndex];
    else
        return emptyTransform;
}

} //ns