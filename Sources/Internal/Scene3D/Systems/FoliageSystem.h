#ifndef __DAVAENGINE_SCENE3D_FOLIAGE_SYSTEM_H__
#define __DAVAENGINE_SCENE3D_FOLIAGE_SYSTEM_H__

#include "Base/BaseTypes.h"
#include "Math/MathConstants.h"
#include "Math/Matrix4.h"
#include "Base/Singleton.h"
#include "Entity/SceneSystem.h"
#include "Render/Highlevel/RenderBatch.h"

namespace DAVA
{
class Entity;
class Scene;
class Landscape;
class VegetationRenderObject;

class FoliageSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for the FoliageSystem class.
     * @details Creates a new FoliageSystem instance responsible for managing foliage rendering and behavior in the scene.
     * @param[in] scene Pointer to the Scene object this system will be associated with.
     */
    FoliageSystem(Scene* scene);
    /**
     * @brief Destructor for the FoliageSystem class.
     * 
     * Cleans up resources and destroys the foliage system instance.
     * The override specifier indicates this destructor overrides a virtual destructor in a base class.
     */
    ~FoliageSystem() override;

    /**
     * @brief Adds an entity to the foliage system.
     * 
     * @param entity Pointer to the Entity to be added to the system.
     * 
     * @note This is an override of the base system's AddEntity method.
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes an entity from the foliage system
     * @param entity Entity to be removed from the system
     * @details Called when an entity is being removed from the scene or when its components
     *          relevant to the foliage system are being removed
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Called before the system is removed from the scene
     * 
     * Performs necessary cleanup operations to ensure proper system removal.
     * As this is an override, it implements the virtual method from the base class.
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes the foliage system for a given time step
     * @param timeElapsed Time elapsed since the last update in seconds
     * @details Updates foliage animation, LOD calculations, and other time-dependent behaviors
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Synchronizes foliage placement and state with the current landscape configuration
     * 
     * Updates the positioning and state of foliage elements to match any changes in the
     * underlying landscape geometry and properties. This ensures foliage remains properly
     * anchored to the terrain and reflects any modifications made to the landscape.
     */
    void SyncFoliageWithLandscape();

    /**
     * @brief Sets perturbation parameters for foliage animation at a specific point
     * @param point The center point of the perturbation effect in 3D space
     * @param force The direction and magnitude of the force causing the perturbation
     * @param distance The maximum distance from the point where the perturbation effect will be applied
     */
    void SetPerturbation(const Vector3& point, const Vector3& force, float32 distance);

    /**
     * @brief Collects all materials used by foliage instances in the scene.
     * 
     * Traverses through foliage instances and gathers unique materials into the provided set.
     * 
     * @param[out] materials Reference to a set that will be filled with pointers to unique foliage materials
     */
    void CollectFoliageMaterials(Set<NMaterial*>& materials);

    /**
     * @brief Controls the visibility of foliage in the scene
     * @param show If true, shows the foliage; if false, hides it
     */
    void SetFoliageVisible(bool show);
    /**
     * @brief Checks if foliage is currently visible in the scene
     * @return true if foliage is visible, false otherwise
     */
    bool IsFoliageVisible() const;

    /**
     * @brief Debug visualization function for drawing vegetation elements in the scene.
     * 
     * This function is used for debugging purposes to visualize foliage/vegetation elements
     * in the 3D scene. When called, it renders visual representations of vegetation objects
     * that can help in debugging placement, distribution, and other vegetation-related issues.
     */
    void DebugDrawVegetation();

private:
    /**
     * @brief Processes a vegetation render object for foliage system updates.
     * 
     * @param[in,out] vegetationRenderObject Pointer to the vegetation render object to be processed
     * @param[in] timeElapsed Time elapsed since the last update in seconds
     * 
     * @details This function handles the processing and updating of vegetation render objects,
     * which may include animation updates, LOD calculations, and other time-based modifications
     * required for the foliage system.
     */
    void ProcessVegetationRenderObject(VegetationRenderObject*, float32 timeElapsed);

private:
    Entity* landscapeEntity = nullptr;
    DAVA::Vector<Entity*> foliageEntities;
};
};

#endif
