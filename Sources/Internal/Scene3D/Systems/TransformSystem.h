#pragma once

#include "Base/BaseTypes.h"
#include "Math/MathConstants.h"
#include "Math/Matrix4.h"
#include "Base/Singleton.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class Entity;
class Transform;
class TransformComponent;
class Transform;

class TransformSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for TransformSystem
     * @param scene Pointer to Scene this system belongs to
     */
    TransformSystem(Scene* scene);

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
     * @brief Processes transform updates
     * @param timeElapsed Time elapsed since last update
     */
    void Process(float32 timeElapsed) override;

private:
    Vector<Entity*> updatableEntities;

    /**
     * @brief Marks entity as needing update
     * @param entity Entity to mark for update
     */
    void EntityNeedUpdate(Entity* entity);

    /**
     * @brief Recursively adds child entities for update
     * @param entity Root entity to process
     */
    void HierarchicAddToUpdate(Entity* entity);

    /**
     * @brief Finds nodes requiring updates
     * @param entity Entity to check
     */
    void FindNodeThatRequireUpdate(Entity* entity);

    /**
     * @brief Transforms all child entities
     * @param entity Parent entity
     */
    void TransformAllChildEntities(Entity* entity);

    int32 passedNodes;
    int32 multipliedNodes;
};
};
