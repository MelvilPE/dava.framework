#ifndef __DAVAENGINE_SCENE3D_RENDERUPDATESYSTEM_H__
#define __DAVAENGINE_SCENE3D_RENDERUPDATESYSTEM_H__

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"
#include "Base/UnordererMap.h"

namespace DAVA
{
class RenderPass;
class RenderLayer;
class RenderObject;
class RenderBatch;
class Entity;
class Camera;

/**
    \brief This system is required to transfer all changes from scene to render system and update render object properties.
 */
class RenderUpdateSystem : public SceneSystem
{
public:
    RenderUpdateSystem(Scene* scene);

    /**
     * @brief Add entity to the update system
     * @param entity Entity to add
     */
    void AddEntity(Entity* entity) override;

    /**
     * @brief Remove entity from the update system
     * @param entity Entity to remove
     */
    void RemoveEntity(Entity* entity) override;

    void PrepareForRemove() override;

    /**
     * @brief Process scene updates for the frame
     * @param timeElapsed Time since last update
     */
    void Process(float32 timeElapsed) override;

private:
    /**
     * @brief Updates active indexes for render objects
     * @param entity Entity owning the render object
     * @param object Render object to update
     */
    void UpdateActiveIndexes(Entity* entity, RenderObject* object);
    UnorderedMap<Entity*, RenderObject*> entityObjectMap;
};

} // ns

#endif /* __DAVAENGINE_RENDER_RENDERSYSTEM_H__ */
