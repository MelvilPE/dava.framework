#ifndef __DAVAENGINE_SCENE3D_STATIC_OCCLUSION_BUILD_SYSTEM_H__
#define __DAVAENGINE_SCENE3D_STATIC_OCCLUSION_BUILD_SYSTEM_H__

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"
#include "Base/Message.h"

namespace DAVA
{
class Camera;
class Landscape;
class RenderObject;
class StaticOcclusion;
class StaticOcclusionComponent;
class StaticOcclusionData;
class StaticOcclusionDataComponent;
class StaticOcclusionDebugDrawComponent;
class NMaterial;

/**
 * @brief System that allows building occlusion information. Required only in editor.
 */
class StaticOcclusionBuildSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor
     * @param scene Parent scene
     */
    StaticOcclusionBuildSystem(Scene* scene);
    ~StaticOcclusionBuildSystem() override;

    void AddEntity(Entity* entity) override;
    void RemoveEntity(Entity* entity) override;
    void PrepareForRemove() override;
    void Process(float32 timeElapsed) override;
    void ImmediateEvent(Component* component, uint32 event) override;

    void SetCamera(Camera* camera);

    /**
     * @brief Begin building occlusion data
     */
    void Build();

    /**
     * @brief Cancel current build process
     */
    void Cancel();

    /**
     * @brief Check if system is currently building occlusion
     * @return true if build is in progress
     */
    bool IsInBuild() const;

    /**
     * @brief Get current build status
     * @return Status code of current build
     */
    uint32 GetBuildStatus() const;

    /**
     * @brief Get string description of build status
     * @return Status description
     */
    const String& GetBuildStatusInfo() const;

private:
    void PrepareRenderObjects();
    void StartBuildOcclusion();
    void FinishBuildOcclusion();

    void SceneForceLod(int32 layerIndex);
    void CollectEntitiesForOcclusionRecursively(Vector<Entity*>& dest, Entity* entity);
    void OnEntityChanged(Entity* entity);

    Camera* camera = nullptr;
    Landscape* landscape = nullptr;
    Vector<Entity*> occlusionEntities;
    StaticOcclusion* staticOcclusion = nullptr;
    StaticOcclusionDataComponent* componentInProgress = nullptr;
    uint32 activeIndex = -1;
    uint32 objectsCount = 0;
};

inline void StaticOcclusionBuildSystem::SetCamera(Camera* _camera)
{
    camera = _camera;
}

} // ns

#endif /* __DAVAENGINE_SCENE3D_STATIC_OCCLUSION_SYSTEM_H__ */
