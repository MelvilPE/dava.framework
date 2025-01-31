#ifndef __DAVAENGINE_SCENE3D_SWITCHSYSTEM_H__
#define __DAVAENGINE_SCENE3D_SWITCHSYSTEM_H__

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class SwitchSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for the switch system
     * @param scene Pointer to Scene this system belongs to
     */
    SwitchSystem(Scene* scene);

    /**
     * @brief Process updates for switch states
     * @param timeElapsed Time elapsed since last update
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Handle immediate events from components
     * @param component Component triggering the event
     * @param event Event identifier
     */
    void ImmediateEvent(Component* component, uint32 event) override;

    /**
     * @brief Add an entity to the switch system
     * @param entity Entity to add
     */
    void AddEntity(Entity* entity) override;

    /**
     * @brief Remove an entity from the switch system
     * @param entity Entity to remove
     */
    void RemoveEntity(Entity* entity) override;

    /**
     * @brief Prepare system for removal
     */
    void PrepareForRemove() override;

private:
    Set<Entity*> updatableEntities;

    /**
     * @brief Set switch hierarchy for given entity
     * @param entity Entity to set switch for
     * @param switchIndex Index of switch to set
     */
    void SetSwitchHierarchy(Entity* entity, int32 switchIndex);
};
}

#endif //__DAVAENGINE_SCENE3D_SWITCHSYSTEM_H__
