#ifndef __DAVAENGINE_SCENE3D_EVENTSYSTEM_H__
#define __DAVAENGINE_SCENE3D_EVENTSYSTEM_H__

#include "Base/BaseTypes.h"

namespace DAVA
{
class SceneSystem;
class Component;
class EventSystem
{
public:
    enum eEventType
    {
        SWITCH_CHANGED,
        START_PARTICLE_EFFECT,
        STOP_PARTICLE_EFFECT,
        START_ANIMATION,
        STOP_ANIMATION,
        MOVE_ANIMATION_TO_THE_LAST_FRAME,
        MOVE_ANIMATION_TO_THE_FIRST_FRAME,
        SPEED_TREE_MAX_ANIMATED_LOD_CHANGED,
        WAVE_TRIGGERED,
        SOUND_COMPONENT_CHANGED,
        STATIC_OCCLUSION_COMPONENT_CHANGED,
        SKELETON_CONFIG_CHANGED,
        SNAP_TO_LANDSCAPE_HEIGHT_CHANGED,
        LOD_DISTANCE_CHANGED,
        LOD_RECURSIVE_UPDATE_ENABLED,
        GEO_DECAL_CHANGED,

        EVENTS_COUNT
    };

    /**
     * @brief Registers a scene system to receive specified events.
     * @param[in] system Pointer to the scene system that will receive the event
     * @param[in] event Event identifier to register the system for
     * @details This method allows scene systems to subscribe to specific events they want to handle.
     *          Once registered, the system will receive notifications when the specified event occurs.
     */
    void RegisterSystemForEvent(SceneSystem* system, uint32 event);
    /**
     * @brief Unregisters a scene system from receiving specified event notifications.
     * @param system The scene system to be unregistered from event handling.
     * @param event The event type identifier to unregister from.
     * @details Removes the specified system from the list of systems that receive notifications
     * for the given event type. After unregistering, the system will no longer receive
     * notifications when this event occurs.
     */
    void UnregisterSystemForEvent(SceneSystem* system, uint32 event);
    /**
     * @brief Notifies a scene system about an event related to a specific component
     * @param system The scene system to be notified
     * @param component The component related to the event
     * @param event The event identifier
     */
    void NotifySystem(SceneSystem* system, Component* component, uint32 event);
    /**
     * @brief Notifies all registered systems about an event related to a specific component.
     * 
     * @param component Pointer to the component that triggered the event
     * @param event The event identifier to be processed by the systems
     *
     * @note This function broadcasts the specified event to all systems that are registered
     *       to handle component events in the scene.
     */
    void NotifyAllSystems(Component* component, uint32 event);
    /**
     * @brief Notifies all registered systems about an event for a group of components.
     * 
     * @param[in] components Vector of pointers to components that will receive the notification
     * @param[in] event Event identifier to be broadcasted to the components
     * 
     * This method broadcasts the specified event to all systems that are registered to handle
     * notifications for the provided components. Each component in the vector will be processed
     * through the notification chain.
     */
    void GroupNotifyAllSystems(Vector<Component*>& components, uint32 event);

private:
    Vector<SceneSystem*> registeredSystems[EVENTS_COUNT];
};
}

#endif //__DAVAENGINE_SCENE3D_EVENTSYSTEM_H__