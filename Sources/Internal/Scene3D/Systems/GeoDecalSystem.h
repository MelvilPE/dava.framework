#pragma once

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"
#include "Scene3D/Components/GeoDecalComponent.h"
#include "Render/Highlevel/GeoDecalManager.h"

namespace DAVA
{
class GeoDecalSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for the GeoDecalSystem class.
     * @details This system manages geometric decals in a 3D scene, handling their creation, 
     *          rendering, and lifecycle within the scene.
     * 
     * @param scene Pointer to the Scene object this system will operate within.
     */
    GeoDecalSystem(Scene* scene);

    /**
     * @brief Bakes decals into the scene geometry.
     * 
     * This function processes and finalizes geometric decals by baking them into the underlying
     * scene geometry. The baking process permanently applies decal textures and modifications
     * to the affected geometry, which can improve rendering performance by eliminating
     * the need for real-time decal calculations.
     */
    void BakeDecals();

    /**
     * @brief Processes geo decal system update for the given time frame
     * @param timeElapsed Time in seconds since the last update
     * @details This method is called each frame to update the geo decal system state
     */
    void Process(float32 timeElapsed) override;
    /**
     * @brief Adds an entity to the GeoDecal system for processing
     * @param[in] entity Pointer to the Entity to be added to the system
     * @details This function is called when an entity with GeoDecal components needs to be registered with the system
     * @note Overrides the base system's AddEntity method
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes the entity from the GeoDecal system
     * @param[in] entity Pointer to the entity to be removed
     * @details This function overrides the base class implementation to handle the removal of an entity
     *          specifically from the GeoDecal system. It ensures proper cleanup of any GeoDecal-related
     *          components and resources associated with the entity.
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Adds a GeoDecal component to the specified entity.
     * @param entity The entity to add the component to.
     * @param component The component to be added. Must be a GeoDecal component.
     * @note This is an override of the base class method.
     */
    void AddComponent(Entity* entity, Component* component) override;
    /**
     * @brief Removes a component from the specified entity
     * @param entity Pointer to the Entity from which the component should be removed
     * @param component Pointer to the Component that should be removed
     * @override Overrides the base class method
     */
    void RemoveComponent(Entity* entity, Component* component) override;
    /**
     * @brief Prepares the system for removal by performing necessary cleanup operations.
     * 
     * This method is called before the system is removed from the scene.
     * It overrides the base class implementation to handle specific cleanup tasks for the GeoDecal system.
     */
    void PrepareForRemove() override;

private:
    struct DecalBuildInfo
    {
        Entity* entity = nullptr;
        GeoDecalComponent* component = nullptr;
        RenderObject* object = nullptr;
        RenderBatch* batch = nullptr;
        AABBox3 box;
        int32 lodIndex = -1;
        int32 switchIndex = -1;
        Vector3 projectionAxis;
        Matrix4 projectionSpaceTransform;
        Matrix4 projectionSpaceInverseTransform;
    };
    struct DecalRenderBatch
    {
        RenderBatch* batch = nullptr;
        int32 lodIndex = -1;
        int32 switchIndex = -1;
    };
    struct RenderableEntity
    {
        Entity* entity = nullptr;
        RenderObject* renderObject = nullptr;
        /**
         * @brief Default constructor for RenderableEntity
         *
         * Creates an empty renderable entity object with default initialization
         */
        RenderableEntity() = default;
        /**
         * @param[in] e The Entity object to associate with the renderable entity
         * @param[in] ro The RenderObject to associate with the renderable entity
         */
        RenderableEntity(Entity* e, RenderObject* ro)
            : entity(e)
            , renderObject(ro)
        {
        }
    };
    /**
     * @brief Constructs a geometric decal for the specified entity based on its decal component
     * 
     * @param entity The entity to build the decal for
     * @param component The geometric decal component containing decal configuration and properties
     * 
     * This function processes the given entity and its GeoDecalComponent to generate
     * the actual geometric decal in the 3D scene. The decal is built according to
     * the properties defined in the component.
     */
    void BuildDecal(Entity* entity, GeoDecalComponent* component);
    /**
    * @brief Removes all previously created decals associated with the given entity and its GeoDecal component
    * @param entity The entity from which to remove decals
    * @param component The GeoDecal component containing decal data to be removed
    */
    void RemoveCreatedDecals(Entity* entity, GeoDecalComponent* component);
    /**
     * @brief Collects renderable entities that intersect with the specified bounding box
     * @param top The root entity to start the search from
     * @param box The axis-aligned bounding box to test intersection against
     * @param [out] The vector to store collected renderable entities
     *
     * Traverses the entity hierarchy starting from the given root entity and gathers
     * all renderable entities whose bounding boxes intersect with the specified box.
     * The found entities are added to the provided vector.
     */
    void GatherRenderableEntitiesInBox(Entity* top, const AABBox3& box, Vector<RenderableEntity>&);

private:
    struct GeoDecalCacheEntry
    {
        GeoDecalManager::DecalConfig lastValidConfig;
        Vector<std::pair<Entity*, GeoDecalManager::Decal>> decals;
    };
    Map<Component*, GeoDecalCacheEntry> decals;
};
}
