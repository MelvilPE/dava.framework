#include "Scene3D/Components/UnregisteredComponent.h"
#include "Scene3D/Entity.h"
#include "Scene3D/Scene.h"
#include "Scene3D/SceneFileV2.h"
#include "Scene3D/Systems/EventSystem.h"
#include "Scene3D/Systems/GlobalEventSystem.h"

#include "Reflection/ReflectionRegistrator.h"
#include "Reflection/ReflectedMeta.h"

#include "FileSystem/FileSystem.h"

namespace DAVA
{
DAVA_VIRTUAL_REFLECTION_IMPL(UnregisteredComponent)
{
    ReflectionRegistrator<UnregisteredComponent>::Begin()
    .ConstructorByPointer()
    .Field("UnregisteredComponent", &UnregisteredComponent::unregisteredComponent)[M::DisplayName("Unregistered Component")]
    .End();
}

UnregisteredComponent::UnregisteredComponent()
{
    unregisteredComponent = "";
}

UnregisteredComponent::UnregisteredComponent(const KeyedArchive& archive)
{
    unregisteredComponent = archive.SaveToYamlString();
}

UnregisteredComponent::~UnregisteredComponent()
{
    
}

Component* UnregisteredComponent::Clone(Entity* toEntity)
{
    return nullptr;
}

void UnregisteredComponent::Serialize(KeyedArchive* archive, SerializationContext* serializationContext)
{
    archive->LoadFromYamlString(unregisteredComponent);
}

void UnregisteredComponent::Deserialize(KeyedArchive* archive, SerializationContext* serializationContext)
{
    unregisteredComponent = archive->SaveToYamlString();
}
};