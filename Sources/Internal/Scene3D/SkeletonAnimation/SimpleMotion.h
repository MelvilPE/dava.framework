#pragma once

#include "Base/BaseTypes.h"
#include "Base/FastName.h"
#include "Reflection/Reflection.h"
#include "Entity/Component.h"

namespace DAVA
{
class AnimationClip;
class SkeletonAnimation;
class SkeletonComponent;
class SkeletonPose;
class SimpleMotion
{
public:
    /**
     * @brief Default constructor for SimpleMotion class.
     * Creates an instance of SimpleMotion with default values for all members.
     */
    SimpleMotion() = default;
    /**
     * @brief Destructor for SimpleMotion class.
     */
    ~SimpleMotion();

    /**
     * @brief Binds the animation to a specific skeleton component
     * @param[in] skeleton Pointer to the skeleton component to bind this motion to
     * @details This function associates the motion with a skeleton, allowing the animation
     *          to manipulate the bones of the specified skeleton component.
     */
    void BindSkeleton(SkeletonComponent* skeleton);
    /**
     * Starts the animation by setting initial values and preparing it for playback.
     * This function must be called before using the animation.
     */
    void Start();
    /**
     * @brief Stops the current animation playback.
     * 
     * Halts the animation at its current position and prevents further playback.
     */
    void Stop();
    /**
     * @brief Updates the state of the simple motion animation.
     * 
     * @param dTime Delta time in seconds since the last update.
     * 
     * Used to advance the animation state by the specified time interval.
     * The animation progress is calculated based on this delta time.
     */
    void Update(float32 dTime);

    /**
     * @brief Evaluates and calculates the skeleton pose based on the current motion parameters.
     * 
     * @param[out] outPose Pointer to the SkeletonPose structure that will be filled with the evaluated pose data.
     *                     The pose contains the final transformation for each bone in the skeleton.
     * 
     * @note The output pose must be properly initialized before calling this function.
     */
    void EvaluatePose(SkeletonPose* outPose);

    /**
     * @brief Checks if the skeleton animation is currently playing
     * @return true if the animation is currently playing, false otherwise
     */
    bool IsPlaying() const;

    /**
     * @brief Sets the animation clip to be used by this motion.
     * @param[in] animationClip Pointer to the animation clip to be set. Can be nullptr to clear current animation.
     */
    void SetAnimation(AnimationClip* animationClip);
    /**
     * @return Animation clip representing the motion data
     * Gets the animation clip associated with this motion
     */
    AnimationClip* GetAnimation() const;

    /**
     * @return Number of times the motion has been repeated since its start
     */
    uint32 GetRepeatsCount() const;
    /**
     * @brief Sets the number of times this motion will be repeated.
     * @param[in] count The number of times to repeat the motion. Use 0 for infinite repetition.
     */
    void SetRepeatsCount(uint32 count);

protected:
    AnimationClip* animationClip = nullptr;
    SkeletonAnimation* skeletonAnimation = nullptr;

    uint32 repeatsCount = 0;
    uint32 repeatsLeft = 0;
    float32 currentAnimationTime = 0.f;

    bool isPlaying = false;
};

/**
 * @brief Checks if the motion is currently playing.
 * 
 * @return true if the motion is playing
 * @return false if the motion is stopped or paused
 */
inline bool SimpleMotion::IsPlaying() const
{
    return isPlaying;
}

/**
 * @brief Returns the animation clip associated with this simple motion.
 * @return Pointer to the AnimationClip object. Can be nullptr if no animation is set.
 */
inline AnimationClip* SimpleMotion::GetAnimation() const
{
    return animationClip;
}

/**
 * @brief Returns the number of times the motion is set to repeat
 * @return Number of animation repetitions
 */
inline uint32 SimpleMotion::GetRepeatsCount() const
{
    return repeatsCount;
}

/**
 * @brief Sets the number of times the motion should repeat
 * @param[in] count The number of repeats. A value of 0 means infinite repeats.
 */
inline void SimpleMotion::SetRepeatsCount(uint32 count)
{
    repeatsCount = count;
}

} //ns