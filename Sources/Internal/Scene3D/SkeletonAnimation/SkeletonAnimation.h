#pragma once

#include "Base/BaseTypes.h"
#include "Scene3D/Components/SkeletonComponent.h"

namespace DAVA
{
class AnimationClip;

class SkeletonAnimation
{
public:
    /**
     * @brief Default constructor for the SkeletonAnimation class.
     * Creates an empty skeleton animation instance with no animation data.
     */
    SkeletonAnimation() = default;
    /**
     * @brief Destructor of SkeletonAnimation class.
     * Cleans up and releases all resources associated with the skeleton animation.
     */
    ~SkeletonAnimation();

    /**
     * @brief Adds an animation clip to the skeleton animation
     * 
     * @param animationClip Pointer to the animation clip to be added
     * @param ignoreMask Set of track indices to be ignored during animation playback. Empty by default
     * @param startTime Start time for the animation clip. Default is negative maximum float value
     * @param endTime End time for the animation clip. Default is maximum float value
     * 
     * @details This function adds a new animation clip to the skeleton animation system.
     * The ignore mask allows specific tracks to be excluded from the animation.
     * Time parameters allow to define a specific time range for the animation clip.
     */
    void AddAnimationClip(AnimationClip* animationClip, const UnorderedSet<uint32>& ignoreMask = UnorderedSet<uint32>(), float32 startTime = -std::numeric_limits<float32>::max(), float32 endTime = std::numeric_limits<float32>::max());

    /**
     * @brief Binds the animation to a skeleton component.
     * 
     * This function associates the animation with a specific skeleton component,
     * allowing the animation to be applied to the skeleton's joints and bones.
     * 
     * @param[in] skeleton Pointer to the skeleton component to bind the animation to.
     *                     Must not be nullptr.
     */
    void BindSkeleton(const SkeletonComponent* skeleton);
    /**
     * @brief Binds a root node for the skeleton animation using the provided node identifier.
     * 
     * @param[in] rootNodeID The unique identifier of the node to be set as the root node.
     * 
     * This function establishes the root node for the skeleton animation hierarchy. The root node
     * serves as the base point from which all child transformations are calculated during animation.
     */
    void BindRootNode(const FastName& rootNodeID);

    /**
     * @brief Evaluates and calculates skeleton pose at specified time in the animation
     * 
     * @param[in] animationLocalTime Time within the animation to evaluate pose at (in seconds)
     * @param[out] outPose Pointer to skeleton pose structure where the calculated pose will be stored
     * 
     * @details This function computes the skeleton pose at the given animation time by interpolating
     * between keyframes and updating joint transformations. The result is written to the provided
     * outPose parameter.
     */
    void EvaluatePose(float32 animationLocalTime, SkeletonPose* outPose);
    /**
     * @brief Evaluates and retrieves the root position offset at a specific time in the animation.
     * 
     * @param[in] animationLocalTime The local time within the animation timeline (in seconds)
     * @param[out] offset Pointer to a Vector3 where the calculated root position offset will be stored
     * 
     * @details This method calculates the root position offset of the skeleton at the specified animation time.
     * The result is stored in the provided Vector3 pointer.
     */
    void EvaluateRootPosition(float32 animationLocalTime, Vector3* offset);
    /**
     * @brief Evaluates the root offset for skeletal animation interpolation between two time points.
     * 
     * @param animationLocalTime0 The starting local time of the animation in seconds
     * @param animationLocalTime1 The ending local time of the animation in seconds
     * @param[out] offset Pointer to Vector3 where the calculated root offset will be stored
     */
    void EvaluateRootOffset(float32 animationLocalTime0, float32 animationLocalTime1, Vector3* offset);

    /**
     * @brief Returns the duration of the skeleton animation in seconds.
     * @return Total duration of the animation sequence in seconds.
     */
    float32 GetDuration() const;

protected:
    struct SkeletonAnimationClip
    {
        AnimationClip* animationClip = nullptr;
        UnorderedSet<uint32> jointsIgnoreMask;

        Vector<std::pair<uint32, const AnimationTrack*>> boundTracks; //[jointIndex, track]
        const AnimationTrack* rootNodeTrack = nullptr; //for root-node transform extraction
        uint32 rootNodePositionChannel = std::numeric_limits<uint32>::max();

        float32 duration = 0.f;
        float32 clipStartTimestamp = 0.f;
        float32 animationStartTimestamp = 0.f;
    };

    /**
     * @brief Evaluates and calculates a joint transformation at a specific time point within an animation track
     * 
     * @param time The time point (in seconds) at which to evaluate the joint transform
     * @param track Pointer to the animation track containing keyframe data
     * @return JointTransform The interpolated transformation of the joint at the specified time
     * 
     * @details This static function interpolates between keyframes in the animation track
     * to determine the exact position, rotation and scale of a joint at any given time
     * during the animation sequence.
     */
    static JointTransform EvaluateJointTransform(float32 time, const AnimationTrack* track);
    /**
     * @brief Evaluates the root position of the skeleton at a given animation time
     * @param[in] clip The skeleton animation clip to evaluate
     * @param[in] animationLocalTime The local time within the animation (in seconds)
     * @param[out] outPosition Pointer to a Vector3 where the resulting root position will be stored
     */
    void EvaluateRootPosition(SkeletonAnimationClip* clip, float32 animationLocalTime, Vector3* outPosition);
    /**
     * @brief Finds animation clip that corresponds to given animation time
     * @param[in] animationTime Time value to find proper clip from animation sequence
     * @return Pointer to animation clip at given time, or nullptr if not found
     */
    SkeletonAnimationClip* FindClip(float32 animationTime);
    /**
     * @brief Calculates the local time within a specific animation clip based on the overall animation time.
     * @param[in] clip The skeleton animation clip to calculate time for
     * @param[in] animationLocalTime Current local time of the animation
     * @return The calculated local time within the specified clip
     */
    float32 GetClipLocalTime(SkeletonAnimationClip* clip, float32 animationLocalTime);

    Vector<SkeletonAnimationClip> animationClips;
    uint32 maxJointIndex = 0;
};

} //ns