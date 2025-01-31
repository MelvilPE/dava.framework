#ifndef __DAVAENGINE_SCENE3D_LANDSCAPESYSTEM_H__
#define __DAVAENGINE_SCENE3D_LANDSCAPESYSTEM_H__

#include "Base/BaseTypes.h"
#include "Base/BaseMath.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class Entity;
class Landscape;

class LandscapeSystem : public SceneSystem
{
public:
    /**
     * @brief Constructs a LandscapeSystem for managing landscape-related functionality within a scene
     * @param scene Pointer to the Scene instance this landscape system will be associated with
     */
    LandscapeSystem(Scene* scene);
    /**
     * @brief Virtual destructor for LandscapeSystem class.
     * 
     * Ensures proper cleanup of derived classes through virtual inheritance.
     */
    virtual ~LandscapeSystem();

    /**
     * @brief Adds an entity to the landscape system for processing
     * @param[in] entity Pointer to the Entity that should be added to the landscape system
     * @details This method is called when an entity with landscape-related components needs to be processed by the landscape system
     * @note This is an override of the base system's AddEntity method
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes the specified entity from the landscape system
     * @param entity The entity to be removed from the landscape system
     * @note This is an override of the base system's RemoveEntity method
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal, performing necessary cleanup tasks.
     * @details This method is called before the system is removed from the scene to ensure proper cleanup of resources and state.
     * @override Overrides the base class method to provide landscape-specific cleanup functionality.
     */
    void PrepareForRemove() override;
    /**
     * @brief Processes the landscape system for the current frame
     * @param timeElapsed Time elapsed since the last frame in seconds
     * 
     * Updates the landscape system state and performs necessary calculations
     * for the current frame based on the elapsed time.
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Returns all landscape objects in the scene.
     * @return Vector containing pointers to all Landscape objects.
     */
    Vector<Landscape*> GetLandscapeObjects();
    /**
     * @brief Returns a constant reference to the vector containing all landscape entities in the scene
     * @return Constant reference to the vector of landscape entities
     */
    const Vector<Entity*>& GetLandscapeEntities();

protected:
    /**
     * @brief Draws visual metrics for a specified landscape patch
     * @param[in] landscape Pointer to the Landscape object to draw metrics for
     * @param[in] subdivLevel Subdivision level of the patch
     * @param[in] x X-coordinate of the patch in the landscape grid
     * @param[in] y Y-coordinate of the patch in the landscape grid
     */
    void DrawPatchMetrics(Landscape* landscape, uint32 subdivLevel, uint32 x, uint32 y);

    Vector<Entity*> landscapeEntities;
};

} // ns

#endif /* __DAVAENGINE_SCENE3D_LANDSCAPESYSTEM_H__ */
