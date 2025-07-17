#include "Scene3D/Components/SoundComponent.h"
#include "Base/FastName.h"
#include "Engine/Engine.h"
#include "Engine/EngineContext.h"
#include "Reflection/ReflectedMeta.h"
#include "Reflection/ReflectionRegistrator.h"
#include "Scene3D/Components/ComponentHelpers.h"
#include "Scene3D/Components/TransformComponent.h"
#include "Scene3D/Entity.h"
#include "Scene3D/Scene.h"
#include "Scene3D/Systems/EventSystem.h"
#include "Scene3D/Systems/GlobalEventSystem.h"
#include "Scene3D/Systems/SoundUpdateSystem.h"
#include "Sound/SoundEvent.h"
#include "Sound/SoundSystem.h"
#include "Utils/Utils.h"

namespace DAVA
{
template <>
bool AnyCompare<SoundComponentElement>::IsEqual(const Any& v1, const Any& v2)
{
    return v1.Get<SoundComponentElement>() == v2.Get<SoundComponentElement>();
}

bool SoundComponentElement::operator==(const SoundComponentElement& other) const
{
    return soundEvent == other.soundEvent &&
    localDirection == other.localDirection &&
    flags == other.flags;
}

DAVA_VIRTUAL_REFLECTION_IMPL(SoundComponent)
{
    ReflectionRegistrator<SoundComponent>::Begin()
    .ConstructorByPointer()
    .End();
}

void SoundComponent::AddSoundEvent(SoundEvent* _event, uint32 flags /*= 0*/, const Vector3& direction /*= Vector3(1.f, 0.f, 0.f)*/)
{
    DVASSERT(_event);

    SafeRetain(_event);
    events.push_back(SoundComponentElement(_event, flags, direction));

    GlobalEventSystem::Instance()->Event(this, EventSystem::SOUND_COMPONENT_CHANGED);
}

void SoundComponent::RemoveSoundEvent(SoundEvent* event)
{
    uint32 eventCount = static_cast<uint32>(events.size());
    for (uint32 i = 0; i < eventCount; ++i)
    {
        if (events[i].soundEvent == event)
        {
            events[i].soundEvent->Stop(true);
            SafeRelease(events[i].soundEvent);
            RemoveExchangingWithLast(events, i);

            return;
        }
    }
}

void SoundComponent::RemoveAllEvents()
{
    uint32 eventsCount = static_cast<uint32>(events.size());
    for (uint32 i = 0; i < eventsCount; ++i)
    {
        events[i].soundEvent->Stop(true);
        SafeRelease(events[i].soundEvent);
    }

    events.clear();
}

void SoundComponent::Trigger()
{
    uint32 eventsCount = static_cast<uint32>(events.size());
    for (uint32 i = 0; i < eventsCount; ++i)
        Trigger(i);
}

void SoundComponent::Stop()
{
    uint32 eventsCount = static_cast<uint32>(events.size());
    for (uint32 i = 0; i < eventsCount; ++i)
        Stop(i);
}

void SoundComponent::Trigger(uint32 index)
{
    DVASSERT(index < events.size());

    SoundComponentElement& sound = events[index];
    sound.soundEvent->Trigger();

    if ((sound.flags & SoundComponent::FLAG_AUTO_DISTANCE_TRIGGER) && entity && entity->GetScene())
    {
        entity->GetScene()->soundSystem->AddAutoTriggerSound(entity, sound.soundEvent);
    }
}

void SoundComponent::Stop(uint32 index)
{
    DVASSERT(index < events.size());

    SoundComponentElement& sound = events[index];
    sound.soundEvent->Stop();

    if ((sound.flags & SoundComponent::FLAG_AUTO_DISTANCE_TRIGGER) && entity && entity->GetScene())
    {
        entity->GetScene()->soundSystem->RemoveAutoTriggerSound(entity, sound.soundEvent);
    }
}

void SoundComponent::SetSoundEventFlags(uint32 index, uint32 flags)
{
    DVASSERT(index < static_cast<uint32>(events.size()));

    if (events[index].flags != flags)
    {
        Stop(index);
        events[index].flags = flags;

        GlobalEventSystem::Instance()->Event(this, EventSystem::SOUND_COMPONENT_CHANGED);
    }
}

void SoundComponent::SetLocalDirection(uint32 eventIndex, const Vector3& direction)
{
    DVASSERT(eventIndex < static_cast<uint32>(events.size()));
    events[eventIndex].localDirection = direction;
}

void SoundComponent::SetLocalDirection(const DAVA::Vector3& direction)
{
    uint32 eventsCount = static_cast<uint32>(events.size());
    for (uint32 i = 0; i < eventsCount; ++i)
        SetLocalDirection(i, direction);
}
}; // namespace DAVA
