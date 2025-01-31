#pragma once

#include "Base/BaseMath.h"
#include "Base/BaseTypes.h"
#include "Base/FastName.h"
#include "Base/UnordererMap.h"
#include "Reflection/Reflection.h"
#include "Scene3D/SkeletonAnimation/SkeletonPose.h"
#include "Scene3D/Components/SkeletonComponent.h"

#include "Private/Motion.h"
#include "Private/MotionTransition.h"

namespace DAVA
{
class BlendTree;
class YamlNode;
class MotionLayer;
class Motion;
struct MotionTransitionInfo;

class MotionLayer
{
    /**
    * @brief Default constructor for MotionLayer class.
    * Creates an empty motion layer with default values.
    */
    MotionLayer() = default;

public:
    enum eMotionBlend
    {
        BLEND_OVERRIDE,
        BLEND_ADD,
        BLEND_DIFF,

        BLEND_COUNT
    };

    /**
     * @brief Creates a MotionLayer instance from YAML configuration.
     * @param[in] motionNode Pointer to YAML node containing motion layer configuration data
     * @return Pointer to newly created MotionLayer instance, or nullptr if creation fails
     * @details Parses YAML node data to construct a MotionLayer object with its properties.
     *          The node should contain valid motion layer configuration parameters.
     */
    static MotionLayer* LoadFromYaml(const YamlNode* motionNode);

    /**
     * @brief Gets the name of the motion layer.
     * @return Constant reference to the FastName representing the layer's name.
     */
    const FastName& GetName() const;
    /**
     * @brief Returns the current blending mode of the motion layer.
     * 
     * @return eMotionBlend The blending mode used for this motion layer's animation.
     *         This value determines how this layer's animation is combined with other layers.
     */
    eMotionBlend GetBlendMode() const;
    /**
     * @brief Returns the current skeleton pose stored in this motion layer
     * @return A constant reference to current SkeletonPose object
     */
    const SkeletonPose& GetCurrentSkeletonPose() const;
    /**
     * @brief Gets the current delta offset of the root bone in the motion layer
     * @return Constant reference to a Vector3 representing the current root offset delta
     */
    const Vector3& GetCurrentRootOffsetDelta() const;

    /**
     * Triggers an event identified by the specified name
     * @param trigger Name of the event to be triggered
     */
    void TriggerEvent(const FastName& trigger); //TODO: *Skinning* make adequate naming

    /**
     * Updates animation state based on the given time delta
     * @param dTime Time elapsed since the last update in seconds
     */
    void Update(float32 dTime);

    /**
     * @brief Binds the motion layer to a specific skeleton.
     * 
     * This function establishes a connection between the motion layer and a skeleton component,
     * allowing the layer to animate the specified skeleton's joints and bones.
     * 
     * @param[in] skeleton Pointer to the skeleton component that will be animated by this motion layer.
     *                     Must not be nullptr.
     */
    void BindSkeleton(const SkeletonComponent* skeleton);

    /**
     * Returns an array of parameter IDs associated with this motion layer.
     * @return A constant reference to the Vector containing FastName parameter identifiers.
     */
    const Vector<FastName>& GetParameterIDs() const;
    /**
    * @param parameterID Unique identifier for the parameter to bind
    */
    bool BindParameter(const FastName& parameterID, const float32* param);
    /**
     * @brief Unbinds a parameter from the motion layer.
     * @param[in] parameterID The unique identifier of the parameter to be unbound.
     * @return Returns true if the parameter was successfully unbound, false otherwise.
     */
    bool UnbindParameter(const FastName& parameterID);
    /**
    * Unbinds all parameters attached to the motion layer. 
    * This function releases any parameter associations previously set on the layer.
    */
    void UnbindParameters();

    /**
     * @return Constant reference to a vector containing motion identifiers (FastName) for this layer
     */
    const Vector<FastName>& GetMotionIDs() const;
    /**
     * @return Vector of pairs, where each pair contains markers that were reached during animation.
     *         First element in pair is marker name, second element is animation name.
     */
    const Vector<std::pair<FastName, FastName>>& GetReachedMarkers() const;
    /**
     * @brief Gets list of motions that have finished playing in this layer
     * @return Vector of FastNames representing ended motions
     */
    const Vector<FastName>& GetEndedMotions() const;

protected:
    FastName layerID;
    eMotionBlend blendMode = BLEND_COUNT;

    Vector<Motion> motions;
    Vector<MotionTransitionInfo> transitions;

    Vector<FastName> motionsIDs;
    Vector<FastName> parameterIDs;

    MotionTransition motionTransition; //transition from 'current' to 'next' motion
    Motion* currentMotion = nullptr;
    Motion* nextMotion = nullptr;
    Motion* pendingMotion = nullptr;
    MotionTransitionInfo* pendingTransition = nullptr;

    Vector3 currentRootOffsetDelta;
    Vector3 rootExtractionMask = Vector3(0.f, 0.f, 0.f);
    Vector3 rootResetMask = Vector3(1.f, 1.f, 1.f);

    FastName rootNodeID;
    uint32 rootNodeJointIndex = SkeletonComponent::INVALID_JOINT_INDEX;

    SkeletonPose currentPose;
    Vector<std::pair<FastName, FastName>> reachedMarkers; /*[motion-id, phase-id]*/
    Vector<FastName> endedMotions;

    DAVA_REFLECTION(MotionLayer);
};

/**
 * @brief Returns the identification name of the motion layer.
 * @return The FastName reference representing the layer ID.
 */
inline const FastName& MotionLayer::GetName() const
{
    return layerID;
}

/**
 * @brief Returns the current blend mode of the motion layer
 * @return Current blend mode value from eMotionBlend enum
 */
inline MotionLayer::eMotionBlend MotionLayer::GetBlendMode() const
{
    return blendMode;
}

/**
 * @brief Gets the current skeleton pose for this motion layer
 * @return const reference to the current SkeletonPose
 */
inline const SkeletonPose& MotionLayer::GetCurrentSkeletonPose() const
{
    return currentPose;
}

/**
 * @brief Returns the current root offset delta vector
 * @return const Vector3& Current root offset delta vector
 */
inline const Vector3& MotionLayer::GetCurrentRootOffsetDelta() const
{
    return currentRootOffsetDelta;
}

/**
 * @brief Returns a reference to the vector containing parameter identifiers.
 * @return Const reference to a vector of FastName objects representing parameter IDs.
 */
inline const Vector<FastName>& MotionLayer::GetParameterIDs() const
{
    return parameterIDs;
}

/**
 * @brief Returns a reference to the vector of reached markers.
 * 
 * @return const Vector<std::pair<FastName, FastName>>& A constant reference to the vector 
 *         containing pairs of markers that have been reached during animation
 */
inline const Vector<std::pair<FastName, FastName>>& MotionLayer::GetReachedMarkers() const
{
    return reachedMarkers;
}

/**
 * @brief Returns a vector of ended motion names in the current motion layer
 * @return Constant reference to the vector containing FastName objects representing ended motions
 */
inline const Vector<FastName>& MotionLayer::GetEndedMotions() const
{
    return endedMotions;
}

} //ns