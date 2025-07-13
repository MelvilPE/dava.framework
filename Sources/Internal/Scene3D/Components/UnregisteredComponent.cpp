#include "Scene3D/Components/UnregisteredComponent.h"
#include "Scene3D/Entity.h"
#include "Scene3D/Scene.h"
#include "Scene3D/SceneFileV2.h"
#include "Scene3D/Systems/EventSystem.h"
#include "Scene3D/Systems/GlobalEventSystem.h"

#include "Reflection/ReflectedMeta.h"
#include "Reflection/ReflectionRegistrator.h"

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
    UnregisteredComponent* newUnregisteredComponent = new UnregisteredComponent();
    newUnregisteredComponent->SetUnregisteredComponentYaml(unregisteredComponent);
    return newUnregisteredComponent;
}

void UnregisteredComponent::Serialize(KeyedArchive* archive, SerializationContext* serializationContext)
{
    archive->LoadFromYamlString(unregisteredComponent);
}

void UnregisteredComponent::Deserialize(KeyedArchive* archive, SerializationContext* serializationContext)
{
    unregisteredComponent = archive->SaveToYamlString();
}

void UnregisteredComponent::SetUnregisteredComponentYaml(String data)
{
    unregisteredComponent = data;
}

String UnregisteredComponent::GetUnregisteredComponentYaml()
{
    return unregisteredComponent;
}
}; // namespace DAVA