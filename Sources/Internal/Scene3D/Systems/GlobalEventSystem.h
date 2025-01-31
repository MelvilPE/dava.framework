#ifndef __DAVAENGINE_SCENE3D_GLOBALEVENTSYSTEM_H__
#define __DAVAENGINE_SCENE3D_GLOBALEVENTSYSTEM_H__

#include "Base/BaseTypes.h"
#include "Base/StaticSingleton.h"

namespace DAVA
{
class Component;
class Entity;
class Scene;

class GlobalEventSystem : public StaticSingleton<GlobalEventSystem>
{
public:
    /**
     * Processes an event for the specified component
     * @param component Pointer to the component that triggered the event
     * @param event Unique identifier of the event to be processed
     */
    void Event(Component* component, uint32 event);
    /**
     * @brief Groups components to process specified event
     * @param[in] scene Pointer to the scene where event occurs
     * @param[in,out] components Vector of components to be grouped
     * @param[in] event Event identifier to process
     * @details This method organizes components into groups for processing a specific event within the scene.
     */
    void GroupEvent(Scene* scene, Vector<Component*>& components, uint32 event);
    /**
     * @brief Processes and executes all cached events associated with the specified component
     * @param[in] component Pointer to the Component for which cached events should be performed
     * 
     * This method iterates through and executes all events that have been previously cached
     * for the given component. After processing, the events are removed from the cache.
     */
    void PerformAllEventsFromCache(Component* component);
    /**
     * @brief Removes all events associated with the specified component.
     * @param[in] component Pointer to the component whose events should be removed.
     */
    void RemoveAllEvents(Component* component);

private:
    Map<Component*, List<uint32>> eventsCache;
};
}

#endif //__DAVAENGINE_SCENE3D_EVENTSYSTEM_H__