#pragma once

#include "Base/AllocatorFactory.h"
#include "Base/BaseMath.h"
#include "Base/BaseTypes.h"
#include "Base/Hash.h"
#include "Base/UnordererMap.h"

#include "MotionTransition.h"

namespace DAVA
{
class BlendTree;
class SkeletonComponent;
class SkeletonPose;
class YamlNode;
struct MotionTransitionInfo;

class Motion
{
public:
    /**
     * @brief Default constructor for Motion class.
     *
     * Creates an empty Motion object with default initialization of all members.
     * Motion objects are typically used to store and manage animation data for skeletal animations.
     */
    Motion() = default;
    /**
     * @brief Destructor for Motion class.
     * Cleans up and releases resources associated with the Motion object.
     */
    ~Motion();

    struct TransitionInfo
    {
        MotionTransitionInfo* info;
        Motion* motion;
    };

    /**
     * @brief Loads motion data from a YAML node
     * 
     * @param motionNode Pointer to the YAML node containing motion data that needs to be parsed and loaded
     * 
     * @details This function parses and loads animation motion data from a YAML format into the current Motion object.
     * The YAML node should contain valid motion data structured according to the expected format.
     * 
     * @throw Can potentially throw exceptions if the YAML node is invalid or if parsing fails
     */
    void LoadFromYaml(const YamlNode* motionNode);

    /**
     * @brief Resets the animation motion to its initial state.
     * 
     * Resets all internal motion parameters including time, frame positions and cached values
     * to their default states, preparing the motion for a new playback cycle.
     */
    void Reset();
    /**
     * @brief Updates the motion for the given time delta
     * @param[in] dTime Time delta in seconds since the last update
     */
    void Update(float32 dTime);
    /**
     * @brief Evaluates and calculates the skeleton pose at the current animation state
     * @param[out] outPose Pointer to the SkeletonPose structure that will receive the calculated pose
     * 
     * This function computes the final pose of the skeleton based on the current motion data
     * and writes the results to the provided SkeletonPose structure. The pose includes
     * all bone transformations and positions for the current animation frame.
     */
    void EvaluatePose(SkeletonPose* outPose) const;
    /**
     * @brief Gets the delta offset of the root node relative to the initial position
     * @param[out] offset Pointer to a Vector3 where the root node offset delta will be stored
     * 
     * This function calculates and returns the difference between current root position
     * and initial position in the animation. The result is written to the provided Vector3 pointer.
     */
    void GetRootOffsetDelta(Vector3* offset) const;
    /**
     * @param withOther Motion to synchronize phase with. Used as a reference for phase synchronization
     * @brief Synchronizes the current motion's phase with another motion
     */
    void SyncPhase(const Motion* withOther, const MotionTransitionInfo* transitionInfo);

    /**
     * @brief Checks if the animation has reached its end
     * 
     * @return true if the animation has finished playing
     * @return false if the animation is still playing or hasn't started
     */
    bool IsAnimationEndReached() const;
    /**
     * Checks if the specified animation phase has reached its end.
     * @param phaseIndex Index of the animation phase to check.
     * @return true if the phase has reached its end, false otherwise.
     */
    bool IsPhaseEndReached(uint32 phaseIndex) const;
    /**
     * @brief Checks if a specific marker has been reached in the animation.
     * @param marker The name of the marker to check.
     * @return true if the marker has been reached, false otherwise.
     */
    bool IsMarkerReached(const FastName& marker) const;

    /**
     * @brief Gets a list of markers that have been reached during animation playback
     * @return A constant reference to the vector containing FastName values representing reached markers
     */
    const Vector<FastName>& GetReachedMarkers() const;

    /**
     * @brief Gets the unique identifier of the motion
     * @return Immutable reference to the FastName that represents the motion ID
     */
    const FastName& GetID() const;
    /**
     * Returns a list of parameters used in the blend tree.
     * Each parameter is represented as a FastName, which represents a unique identifier.
     * These parameters are used to control the blending behavior in the animation system.
     * 
     * @return A constant reference to a vector containing the blend tree parameters.
     */
    const Vector<FastName>& GetBlendTreeParameters() const;

    /**
     * @brief Binds the animation to a specific skeleton component.
     * 
     * This function associates the animation data with a skeleton, allowing the animation
     * to be applied to the specified skeleton component during playback.
     */
    void BindSkeleton(const SkeletonComponent* skeleton);
    /**
     * @brief Binds the root node to this motion using the specified node identifier
     * @param rootNodeID Name identifier of the node to be set as root
     */
    void BindRootNode(const FastName& rootNodeID);

    /**
     * @param parameterID Unique identifier for the parameter to be bound
     */
    bool BindParameter(const FastName& parameterID, const float32* param);
    /**
     * @brief Unbinds and releases all parameters associated with the motion.
     * 
     * This method removes all parameter bindings previously set for this motion,
     * effectively releasing any resources or references to animation parameters.
     */
    void UnbindParameters();

    /**
     * @brief Adds a transition from this motion to another motion triggered by a specific event
     * @param[in] trigger Name of the event that will trigger the transition
     * @param[in] transitionInfo Pointer to transition configuration data
     * @param[in] dstMotion Pointer to the destination motion to transition to
     * @param[in] srcPhase Source phase of the motion where transition can occur (default: max uint32)
     */
    void AddTransition(const FastName& trigger, MotionTransitionInfo* transitionInfo, Motion* dstMotion, uint32 srcPhase = std::numeric_limits<uint32>::max());
    /**
     * @brief Retrieves the transition information associated with the specified trigger.
     * @param trigger The name of the trigger to get transition information for
     * @return A const reference to the TransitionInfo object associated with the trigger
     * @throws May throw an exception if the trigger is not found
     */
    const TransitionInfo& GetTransitionInfo(const FastName& trigger) const;

protected:
    struct TransitionKey
    {
        /**
         * @brief Constructor for TransitionKey which represents a transition trigger with an optional phase
         * @param _trigger FastName reference representing the trigger identifier
         * @param _phase Optional phase parameter, defaults to maximum uint32 value
         */
        TransitionKey(const FastName& _trigger, uint32 _phase = std::numeric_limits<uint32>::max())
            : trigger(_trigger)
            , phase(_phase)
        {
        }

        /**
         * @brief Equality operator for comparing two TransitionKey objects.
         * @param other The TransitionKey object to compare with.
         * @return true if both transition keys have the same trigger and phase values, false otherwise.
         */
        inline bool operator==(const TransitionKey& other) const
        {
            return trigger == other.trigger && phase == other.phase;
        }

        FastName trigger;
        uint32 phase = std::numeric_limits<uint32>::max();
    };

    struct TransitionKeyHash
    {
        /**
         * @brief Hash function object for TransitionKey
         * 
         * A functor that generates a hash value for a TransitionKey object by combining
         * hashes of its trigger and phase members using the HashCombine function.
         * 
         * @param key The TransitionKey object to be hashed
         * @return std::size_t Combined hash value for the TransitionKey
         */
        std::size_t operator()(const TransitionKey& key) const
        {
            std::size_t seed = 0;
            HashCombine(seed, key.trigger);
            HashCombine(seed, key.phase);
            return seed;
        }
    };

    FastName id;
    BlendTree* blendTree = nullptr;

    Vector<const float32*> boundParams;

    Vector<FastName> reachedMarkers;
    UnorderedSet<FastName> reachedMarkersSet;
    UnorderedMap<TransitionKey, TransitionInfo, TransitionKeyHash> transitions;

    Vector3 rootOffset;
    uint32 animationCurrPhaseIndex = 0u;
    uint32 animationPrevPhaseIndex = 0u;
    float32 animationPhase = 0.f;
    bool animationEndReached = false;
};

/**
 * Gets the unique identifier of the motion
 * @return FastName reference containing the motion ID
 */
inline const FastName& Motion::GetID() const
{
    return id;
}

/**
* @brief Returns all animation markers that were reached during motion playback.
* @return Constant reference to the vector of FastName objects representing reached markers.
*/
inline const Vector<FastName>& Motion::GetReachedMarkers() const
{
    return reachedMarkers;
}

/**
 * @brief Checks if the animation has reached its end.
 * 
 * @return true if the animation has reached its end, false otherwise.
 */
inline bool Motion::IsAnimationEndReached() const
{
    return animationEndReached;
}

/**
 * @brief Checks if the animation has just completed a specific phase.
 * 
 * @param phaseIndex Index of the phase to check
 * @return true if the animation has just transitioned from the specified phase to a different one
 * @return false if the animation is either still in the specified phase or was not in that phase previously
 */
inline bool Motion::IsPhaseEndReached(uint32 phaseIndex) const
{
    return (animationPrevPhaseIndex == phaseIndex) && (animationCurrPhaseIndex != phaseIndex);
}

/**
 * @brief Checks if a specific animation marker has been reached during motion playback
 * @param[in] marker The marker name to check
 * @return true if the marker has been reached, false otherwise
 * 
 * This method checks if a given marker name exists in the set of markers that have been
 * reached during the animation playback. The marker tracking is managed internally by
 * the Motion class.
 */
inline bool Motion::IsMarkerReached(const FastName& marker) const
{
    return reachedMarkersSet.count(marker) > 0;
}

} //ns
