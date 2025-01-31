#ifndef __DAVAENGINE_SCENE3D_LIGHTUPDATESYSTEM_H__
#define __DAVAENGINE_SCENE3D_LIGHTUPDATESYSTEM_H__

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class RenderPass;
class RenderLayer;
class RenderObject;
class RenderBatch;
class Entity;
class Camera;
class Light;

/**
    \brief This system is required to transfer all changes from scene to render system and update render object properties.
 */
class LightUpdateSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for the LightUpdateSystem class.
     * 
     * The LightUpdateSystem is responsible for managing and updating lighting within a 3D scene.
     * It handles the processing of light sources, shadows, and related lighting calculations.
     * 
     * @param scene Pointer to the Scene object this system will operate on
     */
    LightUpdateSystem(Scene* scene);

    /**
     * @brief Adds an entity with light components to the light update system.
     * @details The system will manage and update light-related properties for the added entity.
     * @param entity Pointer to the Entity object that should be added to the system
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes an entity from the light update system
     * @param[in] entity The entity to be removed from the system
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal from the scene.
     * 
     * This method is called before the system is removed from the scene to perform any necessary cleanup operations.
     * Overrides the base class implementation.
     */
    void PrepareForRemove() override;
    /**
     * @brief Processes light updates for the scene.
     * @param[in] timeElapsed Time elapsed since last update in seconds.
     */
    void Process(float32 timeElapsed) override;

private:
    /**
     * @brief Recalculates lighting for the specified entity
     * @param[in] entity Pointer to the entity whose lighting needs to be recalculated
     * @details Updates lighting parameters and calculations for the given entity in the scene.
     *          Should be called when lighting properties or entity transform changes.
     */
    void RecalcLight(Entity* entity);
    UnorderedMap<Entity*, Light*> entityObjectMap;
};

} // ns

#endif /* __DAVAENGINE_SCENE3D_LIGHTUPDATESYSTEM_H__ */
