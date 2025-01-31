#ifndef __DAVAENGINE_SCENE3D_UPDATABLESYSTEM_H__
#define __DAVAENGINE_SCENE3D_UPDATABLESYSTEM_H__

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class IUpdatableBeforeTransform;
class IUpdatableAfterTransform;
/**
 * @brief System for updating entities in a scene
 */
class UpdateSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for UpdateSystem
     * @param scene Pointer to Scene this system belongs to
     */
    UpdateSystem(Scene* scene);

    /**
     * @brief Adds an entity to the system
     * @param entity Pointer to Entity to add
     */
    void AddEntity(Entity* entity) override;

    /**
     * @brief Removes an entity from the system
     * @param entity Pointer to Entity to remove
     */
    void RemoveEntity(Entity* entity) override;

    /**
     * @brief Prepares system for removal
     */
    void PrepareForRemove() override;

    /**
     * @brief Updates entities before transform calculations
     * @param timeElapsed Time elapsed since last update
     */
    void UpdatePreTransform(float32 timeElapsed);

    /**
     * @brief Updates entities after transform calculations
     * @param timeElapsed Time elapsed since last update
     */
    void UpdatePostTransform(float32 timeElapsed);

private:
    Vector<IUpdatableBeforeTransform*> updatesBeforeTransform;
    Vector<IUpdatableAfterTransform*> updatesAfterTransform;
};
}

#endif //__DAVAENGINE_SCENE3D_UPDATABLESYSTEM_H__