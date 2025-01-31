#pragma once

#include <Base/BaseTypes.h>
#include <Entity/SceneSystem.h>
#include <Base/Message.h>

namespace DAVA
{
class Camera;
class RenderObject;
class StaticOcclusionComponent;
class StaticOcclusionData;
class StaticOcclusionDataComponent;
class StaticOcclusionDebugDrawComponent;
class NMaterial;
class PolygonGroup;

// System that allow to use occlusion information during rendering
class StaticOcclusionSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for StaticOcclusionSystem
     * @param scene Scene to which this system belongs
     */
    StaticOcclusionSystem(Scene* scene);

    /**
     * @brief Sets the camera used for occlusion culling
     * @param camera Camera to use for occlusion tests
     */
    void SetCamera(Camera* camera);

    /**
     * @brief Registers an entity with the system
     * @param entity Entity to register
     */
    void RegisterEntity(Entity* entity) override;

    /**
     * @brief Unregisters an entity from the system
     * @param entity Entity to unregister
     */
    void UnregisterEntity(Entity* entity) override;

    /**
     * @brief Registers a component with the system
     * @param entity Owner entity of the component
     * @param component Component to register
     */
    void RegisterComponent(Entity* entity, Component* component) override;

    /**
     * @brief Unregisters a component from the system
     * @param entity Owner entity of the component
     * @param component Component to unregister
     */
    void UnregisterComponent(Entity* entity, Component* component) override;

    /**
     * @brief Adds an entity to the system
     * @param entity Entity to add
     */
    void AddEntity(Entity* entity) override;

    /**
     * @brief Removes an entity from the system
     * @param entity Entity to remove
     */
    void RemoveEntity(Entity* entity) override;

    /**
     * @brief Prepares the system for removal
     */
    void PrepareForRemove() override;

    /**
     * @brief Process occlusion updates
     * @param timeElapsed Time elapsed since last update
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Adds a render object to occlusion testing
     * @param renderObject RenderObject to add
     */
    void AddRenderObjectToOcclusion(RenderObject* renderObject);

    /**
     * @brief Removes a render object from occlusion testing
     * @param renderObject RenderObject to remove
     */
    void RemoveRenderObjectFromOcclusion(RenderObject* renderObject);

    /**
     * @brief Clears all occlusion objects
     */
    void ClearOcclusionObjects();

    /**
     * @brief Recursively collects occlusion objects from entity hierarchy
     * @param entity Root entity to start collection from
     */
    void CollectOcclusionObjectsRecursively(Entity* entity);

    /**
     * @brief Invalidates current occlusion state
     */
    void InvalidateOcclusion();

    /**
     * @brief Recursively invalidates occlusion indices
     * @param entity Root entity to start invalidation from
     */
    void InvalidateOcclusionIndicesRecursively(Entity* entity);

private:
    /**
     * @brief Processes static occlusion for one data set
     * @param blockIndex Index of the block to process
     * @param data Occlusion data to process
     */
    void ProcessStaticOcclusionForOneDataSet(uint32 blockIndex, StaticOcclusionData* data);

    /**
     * @brief Undoes occlusion visibility changes
     */
    void UndoOcclusionVisibility();

private:
    Camera* camera = nullptr;
    StaticOcclusionData* activePVSSet = nullptr;
    uint32 activeBlockIndex = 0;
    Vector<StaticOcclusionDataComponent*> staticOcclusionComponents;
    Vector<RenderObject*> indexedRenderObjects;
    bool isInPvs = false;

    uint32 occludedObjectsCount = 0;
    uint32 visibleObjestsCount = 0;
};

class StaticOcclusionDebugDrawSystem : public SceneSystem
{
public:
    StaticOcclusionDebugDrawSystem(Scene* scene);

    void AddEntity(Entity* entity) override;
    void RemoveEntity(Entity* entity) override;
    void ImmediateEvent(Component* component, uint32 event) override;
    void PrepareForRemove() override;

    ~StaticOcclusionDebugDrawSystem();

protected:
    void SetScene(Scene* scene) override;

private:
    void UpdateGeometry(StaticOcclusionDebugDrawComponent* component);

    void CreateStaticOcclusionDebugDrawVertices(StaticOcclusionDebugDrawComponent* target, StaticOcclusionComponent* source);
    void CreateStaticOcclusionDebugDrawGridIndice(StaticOcclusionDebugDrawComponent* target, StaticOcclusionComponent* source);
    void CreateStaticOcclusionDebugDrawCoverIndice(StaticOcclusionDebugDrawComponent* target, StaticOcclusionComponent* source);

    void RemoveComponentFromEntity(Entity* entity);

    NMaterial *gridMaterial, *coverMaterial;
    uint32 vertexLayoutId;
    Vector<Entity*> entities;
};

inline void StaticOcclusionSystem::SetCamera(Camera* _camera)
{
    camera = _camera;
}

} // ns
