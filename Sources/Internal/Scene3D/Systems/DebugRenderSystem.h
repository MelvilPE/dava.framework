#ifndef __DAVAENGINE_SCENE3D_DEBUGRENDERSYSTEM_H__
#define __DAVAENGINE_SCENE3D_DEBUGRENDERSYSTEM_H__

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class Camera;

class DebugRenderSystem : public SceneSystem
{
public:
    /**
     * @brief Constructs a DebugRenderSystem for visual debugging.
     * @param[in] scene Pointer to the Scene object this system will operate on.
     * 
     * A system responsible for rendering debug information and visual aids in a 3D scene.
     * This can include wireframes, bounding boxes, collision shapes, and other debug visualizations.
     */
    DebugRenderSystem(Scene* scene);
    /**
     * @brief Destructor for DebugRenderSystem class.
     * Cleans up and releases resources used by the debug rendering system.
     */
    ~DebugRenderSystem();

    /**
     * @brief Processes debug rendering for one frame
     * @param[in] timeElapsed Time elapsed since last frame in seconds
     */
    void Process(float32 timeElapsed) override;
    /**
     * @brief Adds a new entity to the debug render system
     * @param entity Pointer to the Entity that needs to be added to the system
     * 
     * Implements the base System interface method for adding entities.
     * The entity will be processed for debug rendering in subsequent frames.
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes an entity from the debug render system
     * @param[in] entity Pointer to the Entity to be removed from the system
     * @details This method is used to remove entities that no longer need debug rendering
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal from the scene.
     * 
     * Called before the system is removed from the scene to perform necessary cleanup
     * and release resources.
     * 
     * @note This is an override of the base class method.
     */
    void PrepareForRemove() override;

    /**
     * @brief Sets the camera used for debug rendering
     * @param[in] camera Pointer to the camera object to be used for debug rendering
     * @note Camera can be nullptr to disable debug rendering
     */
    void SetCamera(Camera* camera);

private:
    Vector<Entity*> entities;
    Camera* camera;
};
}

#endif //__DAVAENGINE_SCENE3D_DEBUGRENDERSYSTEM_H__