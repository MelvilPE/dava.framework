#pragma once

#include "Base/Any.h"
#include "Base/BaseTypes.h"
#include "Entity/Component.h"
#include "Reflection/Reflection.h"
#include "Scene3D/Components/UnregisteredComponent.h"
#include "Scene3D/Entity.h"
#include "Scene3D/SceneFile/SerializationContext.h"
#include "Sound/SoundEvent.h"

namespace DAVA
{
class SoundComponent;
struct SoundComponentElement : public InspBase
{
    SoundComponentElement(SoundEvent* _soundEvent, uint32 _flags, const Vector3& _localDirection)
        : soundEvent(_soundEvent)
        , localDirection(_localDirection)
        , flags(_flags)
    {
    }

    SoundEvent* soundEvent;
    Vector3 localDirection;
    uint32 flags;

    bool operator==(const SoundComponentElement& other) const;
};

class SoundComponent : public UnregisteredComponent
{
public:
    enum eEventFlags
    {
        FLAG_AUTO_DISTANCE_TRIGGER = 1 << 0
    };

    inline uint32 GetEventsCount() const;
    inline SoundEvent* GetSoundEvent(uint32 index) const;

    void Trigger();
    void Stop();
    void Trigger(uint32 index);
    void Stop(uint32 index);

    void SetSoundEventFlags(uint32 eventIndex, uint32 flags);
    inline uint32 GetSoundEventFlags(uint32 eventIndex) const;

    void AddSoundEvent(SoundEvent* _event, uint32 flags = 0, const Vector3& direction = Vector3(1.f, 0.f, 0.f));
    void RemoveSoundEvent(SoundEvent* event);
    void RemoveAllEvents();

    void SetLocalDirection(uint32 eventIndex, const Vector3& direction);
    void SetLocalDirection(const DAVA::Vector3& direction);
    inline const Vector3& GetLocalDirection(uint32 eventIndex) const;

protected:
    Vector<SoundComponentElement> events;

    DAVA_VIRTUAL_REFLECTION(SoundComponent, UnregisteredComponent);
};

// Inline
inline SoundEvent* SoundComponent::GetSoundEvent(uint32 index) const
{
    DVASSERT(index < static_cast<uint32>(events.size()));
    return events[index].soundEvent;
}

inline uint32 SoundComponent::GetEventsCount() const
{
    return static_cast<uint32>(events.size());
}

inline uint32 SoundComponent::GetSoundEventFlags(uint32 index) const
{
    DVASSERT(index < static_cast<uint32>(events.size()));
    return events[index].flags;
}

inline const Vector3& SoundComponent::GetLocalDirection(uint32 index) const
{
    DVASSERT(index < static_cast<uint32>(events.size()));
    return events[index].localDirection;
}

template <>
bool AnyCompare<SoundComponentElement>::IsEqual(const Any& v1, const Any& v2);
extern template struct AnyCompare<SoundComponentElement>;
} // namespace DAVA
