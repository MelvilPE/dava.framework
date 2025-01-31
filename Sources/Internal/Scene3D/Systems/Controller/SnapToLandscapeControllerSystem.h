#ifndef __DAVAENGINE_SNAPTOLANDSCAPE_CONTROLLER_SYSTEM_H__
#define __DAVAENGINE_SNAPTOLANDSCAPE_CONTROLLER_SYSTEM_H__

#include "Entity/SceneSystem.h"
#include "Math/Vector.h"

namespace DAVA
{
class Camera;
class Landscape;
class SnapToLandscapeControllerSystem : public SceneSystem
{
public:
    /**
     * @brief System that controls snapping of objects to landscape.
     * @details This system manages the automatic positioning of objects to conform to the terrain's surface.
     * 
     * @param scene Pointer to the Scene object this system will operate within.
     */
    SnapToLandscapeControllerSystem(Scene* scene);
    /**
     * @brief Destructor for SnapToLandscapeControllerSystem class.
     * Cleans up resources and performs necessary cleanup for the system.
     */
    ~SnapToLandscapeControllerSystem() override;

    /**
     * @brief Adds an entity to the Snap To Landscape Controller System
     * @param[in] entity Pointer to the Entity to be added to the system
     * @details This function overrides the base system's AddEntity method and handles
     * the addition of entities that need to be snapped to the landscape
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes the entity from the SnapToLandscapeControllerSystem
     * 
     * @param entity Pointer to the Entity that should be removed from the system
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal from the scene.
     * 
     * Performs necessary cleanup operations before the system is removed from the scene.
     * This method is called as part of the system's lifecycle when it's being removed.
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes the snap to landscape controller logic for the current frame.
     * @param timeElapsed The elapsed time since the last frame in seconds.
     * 
     * Updates the position of controlled entities by snapping them to the landscape
     * geometry according to their snap settings and the elapsed time.
     */
    void Process(float32 timeElapsed) override;
    /**
     * @brief Processes an immediate event for the specified component.
     * @param component Pointer to the component that receives the event
     * @param event Unsigned 32-bit integer representing the event type to be processed
     * @override Overrides the base class implementation
     */
    void ImmediateEvent(Component* component, uint32 event) override;

private:
    /**
     * @brief Snaps an entity to the landscape surface.
     * 
     * @details This function adjusts the position of the given entity to snap it to the surface of the specified landscape.
     * The entity's vertical position (Y-coordinate) will be modified to match the landscape height at its current X,Z position.
     * 
     * @param[in] landscape Pointer to the Landscape object that the entity will snap to
     * @param[in] entity Pointer to the Entity that needs to be snapped to the landscape
     * @param[in] forceSnap If true, forces the snap operation regardless of any conditions that might normally prevent it. Defaults to false
     */
    void SnapToLandscape(Landscape* landscape, Entity* entity, bool forceSnap = false);

    Vector<Entity*> entities;
    Map<Entity*, Vector3> positions;
};
};

#endif //__DAVAENGINE_SNAPTOLANDSCAPE_CONTROLLER_SYSTEM_H__