#ifndef __DAVAENGINE_SKELETON_SYSTEM_H__
#define __DAVAENGINE_SKELETON_SYSTEM_H__

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class Component;
class SkeletonComponent;
class SkinnedMesh;
class RenderHelper;

class SkeletonSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for the SkeletonSystem class.
     * 
     * The SkeletonSystem manages skeleton-based animations and bone transformations
     * within a 3D scene. It handles skeleton hierarchies, animation blending, and
     * bone transformations for skeletal meshes.
     * 
     * @param scene Pointer to the Scene object that this system will operate within.
     *              The scene contains all the entities and components that the
     *              skeleton system will process.
     */
    SkeletonSystem(Scene* scene);
    /**
     * @brief Destructor of the SkeletonSystem class.
     * 
     * Cleans up and releases resources associated with the skeleton animation system.
     */
    ~SkeletonSystem();

    /**
     * Adds an entity to the skeleton system for processing.
     * @param entity The entity to be added. Must contain required skeleton components.
     * @note This is an override of the base system's AddEntity method.
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes specified entity from the skeleton system and cleans up related skeleton components
     * @param entity Pointer to the Entity that needs to be removed from the skeleton system
     * @details This function is called when an entity with skeleton components is being destroyed or removed from the scene.
     *          It ensures proper cleanup of skeleton-related resources and components.
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal by performing necessary cleanup operations
     * 
     * This method is called before the system is removed from the scene to ensure proper cleanup
     * of resources and state. It overrides the base class implementation.
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes immediate events for the skeleton system.
     * @param component The component that triggered the event.
     * @param event The event identifier to be processed.
     * @override Overrides System::ImmediateEvent.
     */
    void ImmediateEvent(Component* component, uint32 event) override;
    /**
     * @brief Processes the skeleton system for the given time frame.
     * 
     * Updates skeleton animations and transformations for all entities with skeletal components.
     * This method is called each frame to advance the animation state.
     * 
     * @param[in] timeElapsed Time passed since the last frame in seconds
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Updates skinned mesh data based on skeleton state
     * @param skeleton Skeleton component to use for update
     * @param skinnedMeshObject Skinned mesh to update
     */
    void UpdateSkinnedMesh(SkeletonComponent* skeleton, SkinnedMesh* skinnedMeshObject);

    /**
     * @brief Draws debug visualization of skeletons
     * @param drawer Helper object for debug drawing
     */
    void DrawSkeletons(RenderHelper* drawer);

private:
    /**
     * @brief Updates joint transforms for a skeleton
     * @param skeleton Skeleton component to update
     */
    void UpdateJointTransforms(SkeletonComponent* skeleton);

    /**
     * @brief Rebuilds skeleton data structure
     * @param skeleton Skeleton component to rebuild
     */
    void RebuildSkeleton(SkeletonComponent* skeleton);

    /**
     * @brief Updates test skeleton animations
     * @param timeElapsed Time since last update
     */
    void UpdateTestSkeletons(float32 timeElapsed);

    Vector<Entity*> entities;
};

} //ns

#endif
