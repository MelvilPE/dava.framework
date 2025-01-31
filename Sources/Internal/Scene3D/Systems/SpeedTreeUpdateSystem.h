#ifndef __DAVAENGINE_SCENE3D_SPEEDTREEUPDATESYSTEM_H__
#define __DAVAENGINE_SCENE3D_SPEEDTREEUPDATESYSTEM_H__

#include "Base/BaseTypes.h"
#include "Base/BaseMath.h"
#include "Base/Observer.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class Entity;
class SpeedTreeComponent;

class SpeedTreeUpdateSystem : public SceneSystem, public Observer
{
public:
    /**
     * @brief Constructor for SpeedTree update system
     * @param scene Pointer to Scene this system belongs to
     */
    SpeedTreeUpdateSystem(Scene* scene);

    /**
     * @brief Destructor
     */
    ~SpeedTreeUpdateSystem() override;

    /**
     * @brief Add an entity to the system
     * @param entity Entity to add
     */
    void AddEntity(Entity* entity) override;

    /**
     * @brief Remove an entity from the system
     * @param entity Entity to remove
     */
    void RemoveEntity(Entity* entity) override;

    /**
     * @brief Prepare system for removal
     */
    void PrepareForRemove() override;

    /**
     * @brief Handle immediate events from components
     * @param component Component triggering the event
     * @param event Event identifier
     */
    void ImmediateEvent(Component* component, uint32 event) override;

    /**
     * @brief Process SpeedTree updates
     * @param timeElapsed Time elapsed since last update
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Handle events from observable objects
     * @param observable Observable object triggering event
     */
    void HandleEvent(Observable* observable) override;

    /**
     * @brief Called when scene has finished loading
     */
    void SceneDidLoaded() override;

protected:
    /**
     * @brief Update animation flags for entity
     * @param entity Entity to update
     */
    void UpdateAnimationFlag(Entity* entity);

private:
    Vector<SpeedTreeComponent*> allTrees;

    bool isAnimationEnabled;
    bool isVegetationAnimationEnabled;

    friend class SpeedTreeComponent;
};

} // ns

#endif /* __DAVAENGINE_SCENE3D_SPEEDTREEUPDATESYSTEM_H__ */
