#ifndef __DAVAENGINE_SCENE3D_SOUNDUPDATESYSTEM_H__
#define __DAVAENGINE_SCENE3D_SOUNDUPDATESYSTEM_H__

#include "Base/BaseTypes.h"
#include "Base/BaseObject.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class SoundEvent;
class SoundComponent;

/**
 * @brief System responsible for updating sound components and managing auto-trigger sounds
 */
class SoundUpdateSystem : public SceneSystem
{
    /**
     * @brief Structure holding auto-trigger sound data
     */
    struct AutoTriggerSound
    {
        /**
         * @brief Constructor
         * @param owner Entity owning the sound
         * @param sound Sound event to trigger
         */
        AutoTriggerSound(Entity* owner, SoundEvent* sound);

        Entity* owner;
        SoundEvent* soundEvent;
        float32 maxSqDistance;
    };

public:
    /**
     * @brief Constructor for the SoundUpdateSystem class.
     * 
     * System responsible for updating sound-related components and behaviors in the scene.
     * 
     * @param scene Pointer to the Scene object this system will operate on
     */
    SoundUpdateSystem(Scene* scene);
    /**
     * @brief Destructor for the SoundUpdateSystem class.
     * 
     * Virtual destructor that ensures proper cleanup of the sound update system resources.
     */
    ~SoundUpdateSystem() override;

    /**
     * @brief Processes immediate events for components in the sound update system.
     * @details This method handles immediate event processing for components that require sound-related updates.
     * 
     * @param component The component that triggered the event
     * @param event The event identifier to process
     * 
     * @override Overrides the base system's ImmediateEvent method
     */
    void ImmediateEvent(Component* component, uint32 event) override;
    /**
     * @brief Process updates for the Sound System
     * @param timeElapsed Time in seconds since the last update
     * @details This method is called each frame to handle sound-related updates in the scene
     */
    void Process(float32 timeElapsed) override;
    /**
     * @brief Adds an entity to the sound update system for processing.
     * 
     * @param[in] entity Pointer to the Entity to be added to the system.
     *                   The entity should have sound-related components to be processed.
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes an entity from the sound update system
     * @param[in] entity Pointer to the entity being removed from the system
     * @override Overrides System::RemoveEntity
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares system resources for removal or cleanup.
     *
     * Called when the system is about to be removed. Performs necessary cleanup
     * operations and resource deallocation to ensure proper system shutdown.
     */
    void PrepareForRemove() override;

    /**
     * @brief Activates the sound update system
     *
     * Overrides the base system's Activate method to initialize and enable sound processing
     * in the scene.
     */
    void Activate() override;
    /**
     * @brief Deactivates the sound update system
     *
     * Overrides the base class Deactivate method to handle sound system cleanup and shutdown.
     * Called when the system needs to be deactivated, typically during scene unloading or
     * system shutdown.
     */
    void Deactivate() override;

protected:
    /**
     * @brief Add auto-trigger sound
     * @param soundOwner Entity that owns the sound
     * @param sound Sound event to auto-trigger
     */
    void AddAutoTriggerSound(Entity* soundOwner, SoundEvent* sound);

    /**
     * @brief Remove auto-trigger sound
     * @param soundOwner Entity that owns the sound
     * @param sound Optional sound event to remove (removes all if null)
     */
    void RemoveAutoTriggerSound(Entity* soundOwner, SoundEvent* sound = 0);

    Vector<AutoTriggerSound> autoTriggerSounds;

    Vector<Entity*> sounds;
    Vector<SoundEvent*> pausedEvents;

    friend class SoundComponent;
};

} // ns

#endif /* __DAVAENGINE_SCENE3D_SOUNDUPDATESYSTEM_H__ */
