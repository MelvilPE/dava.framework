#ifndef __DAVAENGINE_ANIMATION_SYSTEM_H__
#define __DAVAENGINE_ANIMATION_SYSTEM_H__

#include "Base/BaseTypes.h"
#include "Math/MathConstants.h"
#include "Math/Matrix4.h"
#include "Base/Singleton.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class Entity;
class AnimationComponent;

class AnimationSystem : public SceneSystem
{
public:
    /**
     * @brief Constructs an AnimationSystem instance.
     * @details The AnimationSystem handles all animation-related operations within a scene,
     *          including skeletal animations, property animations, and their updates.
     * 
     * @param scene Pointer to the Scene instance this animation system belongs to.
     */
    AnimationSystem(Scene* scene);
    /**
     * @brief Destructor of animation system that handles cleanup of animation-related resources
     */
    ~AnimationSystem();

    /**
     * @brief Removes an entity from the animation system, stopping any ongoing animations.
     * @param entity The entity to be removed from the animation system.
     * @override This method overrides the base class implementation.
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the animation system for removal from the scene.
     * This method is called before the animation system is removed to ensure proper cleanup.
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes animations for the current frame
     * @param timeElapsed Time elapsed since the last frame in seconds
     * 
     * Updates and processes all active animations in the scene with the given time delta.
     * This method is called each frame to advance animation states.
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Processes an immediate animation event for the specified component.
     * @details Handles immediate animation events that need to be processed right away without delay.
     * @param component Pointer to the component that triggered the event.
     * @param event The event identifier to be processed.
     */
    void ImmediateEvent(Component* component, uint32 event) override;

private:
    Vector<AnimationComponent*> activeComponents;
    /**
     * Adds an animation component to the active animation list for processing.
     * @param[in] comp Pointer to the animation component to be added to the active list.
     */
    void AddToActive(AnimationComponent* comp);
    /**
     * @brief Removes the specified animation component from active animations
     * @param comp Pointer to the AnimationComponent to be removed
     */
    void RemoveFromActive(AnimationComponent* comp);
    /**
     * @brief Moves the animation to a specific frame
     * @param comp Pointer to the AnimationComponent to be manipulated
     * @param frameIndex Index of the frame to move the animation to
     * 
     * Positions the animation at the specified frame index without playing through intermediate frames.
     * Frame index must be within valid range for the animation component.
     */
    void MoveAnimationToFrame(AnimationComponent* comp, int frameIndex);
};
};

#endif //__DAVAENGINE_ANIMATION_SYSTEM_H__
