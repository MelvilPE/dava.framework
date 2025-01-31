#pragma once

#include "Base/BaseTypes.h"
#include "Base/FastName.h"
#include "Entity/SceneSystem.h"
#include "Scene3D/Components/SkeletonComponent.h"

namespace DAVA
{
class Component;
class Entity;
class MotionComponent;
class MotionSingleComponent;
class SimpleMotion;

class MotionSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for the MotionSystem class.
     * 
     * The MotionSystem is responsible for handling motion and animations of entities within the scene.
     * It manages movement, transformations, and kinematic calculations for scene objects.
     * 
     * @param scene Pointer to the Scene object that this motion system will operate on
     */
    MotionSystem(Scene* scene);
    /**
     * @brief Destructor for the MotionSystem class.
     * Cleans up resources and destroys the motion system instance.
     */
    ~MotionSystem();

    /**
     * @brief Adds an entity to the Motion System for processing
     * @param entity Pointer to the Entity that should be added to the system
     * @details This method overrides the base system's AddEntity method to handle 
     *          motion-specific entity processing
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes an entity from the motion system.
     * 
     * @param entity The entity to be removed from the motion system
     * 
     * @details This function unregisters the entity from motion handling and removes 
     * all associated motion-related components from the system.
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal from the scene.
     * 
     * Called when the system is about to be removed from its parent scene. 
     * Performs necessary cleanup and resource deallocation before the system is destroyed.
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes an immediate event for a component in the motion system
     * @param component Pointer to the component that triggered the event
     * @param event Event identifier to be processed
     * @note This method overrides the base class implementation
     */
    void ImmediateEvent(Component* component, uint32 event) override;
    /**
     * @brief Process game logic for one frame
     * @param[in] timeElapsed Time elapsed since previous frame in seconds
     */
    void Process(float32 timeElapsed) override;

protected:
    /**
     * @brief Sets the Scene object for the MotionSystem
     * @param[in] scene Pointer to the Scene object to be set. Can be nullptr to clear current scene
     * @details This method overrides base class implementation to establish connection between MotionSystem and Scene
     */
    void SetScene(Scene* scene) override;

private:
    /**
     * @brief Updates motion layers for the given motion component.
     * @details Processes and updates animation layers within the motion component over the specified time delta.
     * 
     * @param[in,out] motionComponent Pointer to the motion component to be updated
     * @param[in] dTime Time delta for the update, in seconds
     */
    void UpdateMotionLayers(MotionComponent* motionComponent, float32 dTime);

    Vector<MotionComponent*> activeComponents;
    MotionSingleComponent* motionSingleComponent = nullptr;
};

} //ns
