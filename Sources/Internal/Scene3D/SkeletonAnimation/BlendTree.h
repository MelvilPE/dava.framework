#pragma once

#include "Base/BaseMath.h"
#include "Base/BaseTypes.h"
#include "Base/FastName.h"

namespace DAVA
{
class SkeletonAnimation;
class SkeletonComponent;
class SkeletonPose;
class YamlNode;
class BlendTree
{
    /**
     * @brief Default constructor for BlendTree
     * 
     * Creates an empty blend tree instance. A blend tree is a data structure 
     * used for skeletal animation blending that allows complex animation transitions 
     * and combinations to be created from multiple animation clips.
     */
    BlendTree() = default;

public:
    enum eNodeType : uint8
    {
        TYPE_ANIMATION,
        TYPE_LERP_1D,
        TYPE_LERP_2D,
        TYPE_ADD,
        TYPE_DIFF,

        TYPE_COUNT
    };

    struct MarkerInfo
    {
        float32 phaseID = 0.f; //integer part - phase-index, fractional part - phase-value
        FastName markerID;

        operator float32() const
        {
            return phaseID;
        }
    };

    /**
     * @brief Destructor for the BlendTree class.
     * Cleans up all resources associated with the blend tree.
     */
    ~BlendTree();
    /**
     * @brief Creates and loads a BlendTree from YAML data
     * @param yamlNode Pointer to the YAML node containing blend tree data
     * @return Pointer to the newly created BlendTree instance, or nullptr if loading fails
     * @throws may throw exceptions if YAML data is invalid or malformed
     */
    static BlendTree* LoadFromYaml(const YamlNode* yamlNode);

    /**
     * @brief Binds the given skeleton component to the blend tree for animation purposes
     * @param[in] skeleton Pointer to the skeleton component to be bound
     * 
     * This function establishes a connection between the blend tree and a skeleton component,
     * allowing the blend tree to apply animations to the specified skeleton.
     */
    void BindSkeleton(const SkeletonComponent* skeleton);
    /**
     * @brief Binds a root node to the blend tree using the specified node identifier.
     * 
     * @param rootNodeID The unique identifier (FastName) of the node to be set as root.
     * 
     * This function establishes the root node for the blend tree hierarchy. The root node
     * serves as the starting point for all blend operations in the animation system.
     */
    void BindRootNode(const FastName& rootNodeID);

    /**
     * @brief Evaluates and calculates the skeleton pose at a specific phase using blend parameters
     * 
     * @param phaseIndex Index of the animation phase to evaluate
     * @param phase Current phase value within the animation (typically between 0 and 1)
     * @param parameters Vector of pointers to blend parameters used for pose calculation
     * @param outPose Pointer to the SkeletonPose object where the resulting pose will be stored
     * 
     * This function computes the final skeleton pose by evaluating the blend tree at the specified
     * phase using the provided blend parameters. The result is written to the outPose parameter.
     */
    void EvaluatePose(uint32 phaseIndex, float32 phase, const Vector<const float32*>& parameters, SkeletonPose* outPose) const;
    /**
     * @brief Evaluates the duration of a specific animation phase based on given parameters
     * @param phaseIndex Index of the animation phase to evaluate
     * @param parameters Vector of pointers to parameter values used in duration calculation
     * @return Duration of the specified phase in seconds
     */
    float32 EvaluatePhaseDuration(uint32 phaseIndex, const Vector<const float32*>& parameters) const;
    /**
     * @param phaseIndex0 The first animation phase index in the animation sequence.
     */
    void EvaluateRootOffset(uint32 phaseIndex0, float32 phase0, uint32 phaseIndex1, float32 phase1, const Vector<const float32*>& parameters, Vector3* outOffset);

    /**
     * @brief Returns a vector of parameter identifiers used in the blend tree.
     * @return Const reference to a vector containing FastName identifiers of parameters.
     */
    const Vector<FastName>& GetParameterIDs() const;
    /**
     * @return A constant reference to a vector containing marker information used in the blend tree.
     * Each marker contains information about animation markers such as start/end points, events, or triggers.
     */
    const Vector<MarkerInfo>& GetMarkersInfo() const;

    /**
     * @brief Returns the total number of animation phases in the blend tree.
     * 
     * @return Number of phases in the blend tree as an unsigned 32-bit integer.
     */
    uint32 GetPhasesCount() const;

protected:
    struct Animation
    {
        SkeletonAnimation* skeletonAnimation = nullptr;
        Vector<float32> phaseEnds;
        bool treatAsPose = false;
    };

    struct BlendNode
    {
        struct BlendData
        {
            int32 beginChildIndex;
            int32 endChildIndex;
            int32 parameterIndex;
        };
        struct AnimationData
        {
            int32 index;
        };

        union
        {
            BlendData blendData;
            AnimationData animData;
        };

        Vector2 coord;
        eNodeType type = TYPE_COUNT;
    };

    /**
     * @brief Loads a mask that specifies which bone animations to ignore during blending
     * @param[in] yamlNode YAML node containing the ignore mask data
     * @param[out] ignoreMask Set that will be populated with bone indices to ignore
     * 
     * Processes a YAML configuration node to extract bone indices that should be excluded
     * from animation blending. The resulting indices are stored in the provided set.
     */
    void LoadIgnoreMask(const YamlNode* yamlNode, UnorderedSet<uint32>* ignoreMask);
    /**
     * @brief Retrieves the timestamp from a clip node in the YAML configuration
     * @param yamlNode Pointer to the YAML node containing clip data
     * @return The timestamp value as a float32
     */
    float32 GetClipTimestamp(const YamlNode* yamlNode) const;
    /**
     * @brief Calculates and returns the phase ID for a given timestamp within an animation
     * @param[in] animation The animation to process
     * @param[in] timestamp The time point in the animation (in seconds)
     * @return The phase ID as a float value corresponding to the timestamp position in the animation
     */
    float32 GetPhaseIDByTimestamp(const BlendTree::Animation& animation, float32 timestamp) const;
    /**
     * @brief Recursively loads a blend node from YAML data into the blend tree
     * 
     * @param yamlNode Source YAML node containing blend node data
     * @param blendTree Destination blend tree to load the node into
     * @param nodeIndex Index of the current node being loaded
     * @param ignoreMask Set of node indices to ignore during loading (passed by value)
     */
    void LoadBlendNodeRecursive(const YamlNode* yamlNode, BlendTree* blendTree, uint32 nodeIndex, UnorderedSet<uint32> ignoreMask = UnorderedSet<uint32>() /*should be by value*/);
    /**
     * @param phaseIndex First phase index in the blend tree
     * @param phase First phase value (0.0f to 1.0f) representing animation progress
     * @param phaseIndex1 Second phase index in the blend tree
     * @param phase1 Second phase value (0.0f to 1.0f) representing animation progress
     * @param node Node in blend tree to evaluate
     * @param parameters Vector of parameter values used for blending animations
     * @param outPose [out] Output skeleton pose after blending
     * @param outPhaseDuration [out] Output duration of the blended animation phase
     * @param outOffset [out] Output offset vector after blending
     */
    void EvaluateRecursive(uint32 phaseIndex, float32 phase, uint32 phaseIndex1, float32 phase1, const BlendNode& node, const Vector<const float32*>& parameters, SkeletonPose* outPose, float32* outPhaseDuration, Vector3* outOffset) const;

    /**
     * @brief Calculates the time value for a specific phase in the given animation.
     * 
     * @param[in] animation The animation to get the phase time from
     * @param[in] phaseIndex Index of the phase in the animation
     * @return float32 Time value for the specified animation phase
     */
    float32 GetAnimationPhaseTime(const Animation& animation, uint32 phaseIndex) const;
    /**
     * @brief Calculates local time within an animation based on phase parameters.
     * 
     * @param animation Reference to the Animation object to compute time for
     * @param phaseIndex Index of the animation phase
     * @param phase Current phase value within the animation
     * 
     * @return Local time value (in seconds) within the animation
     */
    float32 GetAnimationLocalTime(const Animation& animation, uint32 phaseIndex, float32 phase) const;

    Vector<BlendNode> nodes;
    Vector<Animation> animations;
    Vector<MarkerInfo> markers;
    Vector<FastName> parameterIDs;
    uint32 phasesCount = 0;
};

/**
 * @brief Returns a constant reference to the array of parameter identifiers
 * @return Const reference to Vector<FastName> containing parameter IDs used in the blend tree
 */
inline const Vector<FastName>& BlendTree::GetParameterIDs() const
{
    return parameterIDs;
}

/**
* Returns the vector of marker information associated with the blend tree.
* @return A const reference to the vector containing marker information.
*/
inline const Vector<BlendTree::MarkerInfo>& BlendTree::GetMarkersInfo() const
{
    return markers;
}

/**
* Returns the total number of phases in the blend tree.
* @return Number of phases as an unsigned 32-bit integer.
*/
inline uint32 BlendTree::GetPhasesCount() const
{
    return phasesCount;
}

} //ns