#pragma once

#include "Base/AllocatorFactory.h"
#include "Animation/Interpolation.h"
#include "Base/BaseMath.h"
#include "Base/BaseTypes.h"
#include "Base/FastName.h"

#include "Scene3D/SkeletonAnimation/SkeletonPose.h"

namespace DAVA
{
class Motion;
class SkeletonPose;
class YamlNode;

struct MotionTransitionInfo
{
public:
    enum eType : uint8
    {
        TYPE_REPLACE,
        TYPE_CROSS_FADE,
        TYPE_FROZEN_FADE,

        TYPE_COUNT
    };

    enum eSync : uint8
    {
        SYNC_IMMIDIATE,
        SYNC_WAIT_END,
        SYNC_WAIT_PHASE_END,
        SYNC_WAIT_MARKER,

        SYNC_COUNT
    };

    /**
     * @brief Loads motion transition information from a YAML node.
     * @param[in] transitionNode Pointer to YAML node containing transition data
     * @return MotionTransitionInfo object populated with data from the YAML node
     * @throws YamlException if required fields are missing or have invalid format
     */
    static MotionTransitionInfo LoadFromYaml(const YamlNode* transitionNode);

    eType type = TYPE_REPLACE;
    eSync sync = SYNC_IMMIDIATE;
    Interpolation::Func func;

    Vector<uint32> phaseMap;
    FastName markerToWait;
    uint32 phaseToWait = std::numeric_limits<uint32>::max();
    float32 duration = 0.f;
    bool syncPhase = false;
    bool inversePhase = false;
};

class MotionTransition
{
public:
    /**
     * @brief Default constructor for MotionTransition class.
     * 
     * Creates a new instance of MotionTransition with default member values.
     */
    MotionTransition() = default;

    /**
     * @brief Resets the motion transition with the provided transition information and motions
     * 
     * @param transitionInfo Pointer to transition information containing parameters for motion blending
     * @param srcMotion Pointer to the source motion that will be blended from
     * @param dstMotion Pointer to the destination motion that will be blended to
     * 
     * @details The function initializes a new transition between two skeletal animation motions using
     * the specified transition parameters. It prepares the interpolation data needed to smoothly 
     * blend from the source motion to the destination motion.
     */
    void Reset(const MotionTransitionInfo* transitionInfo, Motion* srcMotion, Motion* dstMotion);

    /**
     * @brief Updates the motion transition
     * @param dTime Time delta in seconds since the last update
     */
    void Update(float32 dTime);
    /**
     * @brief Evaluates motion transition and calculates resulting skeleton pose and offset
     * @param[out] outPose Pointer to SkeletonPose where the resulting pose will be stored
     * @param[out] outOffset Pointer to Vector3 where the resulting offset will be stored
     */
    void Evaluate(SkeletonPose* outPose, Vector3* outOffset);

    /**
     * @brief Checks if the motion transition has been completed
     * @return true if the transition is complete, false otherwise
     */
    bool IsComplete() const;
    /**
     * @brief Checks if the motion transition has started.
     * @return true if the transition is in progress, false if not yet started
     */
    bool IsStarted() const;

    /**
     * @brief Checks if the current motion transition can be interrupted by another transition
     * @param other The transition info that potentially interrupts the current transition
     * @param srcMotion Source motion of the transition
     * @param dstMotion Destination motion of the transition
     * @return true if the transition can be interrupted, false otherwise
     */
    bool CanBeInterrupted(const MotionTransitionInfo* other, const Motion* srcMotion, const Motion* dstMotion) const;
    /**
     * @brief Interrupts the current motion transition with another transition
     * @param other Pointer to the transition info that will interrupt the current transition
     * @param srcMotion Pointer to the source motion being interrupted
     * @param dstMotion Pointer to the destination motion to transition to
     * @details This method handles interruption of an ongoing motion transition by another transition,
     *          ensuring proper handling of blending between the source and destination motions
     */
    void Interrupt(const MotionTransitionInfo* other, Motion* srcMotion, Motion* dstMotion);

protected:
    const MotionTransitionInfo* transitionInfo = nullptr;

    Motion* srcMotion = nullptr;
    Motion* dstMotion = nullptr;

    SkeletonPose workPose;
    SkeletonPose frozenPose;
    Vector3 frozenOffset;
    float32 transitionPhase = 0.f;

    bool srcFrozen = false;
    bool inversed = false;
    bool started = false;
};

/**
 * @brief Checks if the motion transition has been started.
 * @return true if the motion transition is started, false otherwise.
 */
inline bool MotionTransition::IsStarted() const
{
    return started;
}

/**
 * @brief Checks if the motion transition is complete.
 * 
 * A transition is considered complete if any of these conditions are met:
 * - The transition phase has reached or exceeded 1.0
 * - The transition duration is effectively zero (less than EPSILON)
 * - The transition type is MotionTransitionInfo::TYPE_REPLACE
 * 
 * @note This method assumes transitionInfo is not null and will assert if it is
 * 
 * @return true if the transition is complete, false otherwise
 */
inline bool MotionTransition::IsComplete() const
{
    DVASSERT(transitionInfo != nullptr);
    return IsStarted() && ((transitionPhase >= 1.f) || (transitionInfo->duration < EPSILON) || (transitionInfo->type == MotionTransitionInfo::TYPE_REPLACE));
}

} //ns