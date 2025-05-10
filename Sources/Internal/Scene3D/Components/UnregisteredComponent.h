#pragma once

#include "Entity/Component.h"
#include "FileSystem/KeyedArchive.h"
#include "Reflection/Reflection.h"
#include "Scene3D/SceneFile/SerializationContext.h"

namespace DAVA
{
class UnregisteredComponent : public Component
{
protected:
    virtual ~UnregisteredComponent();

public:
    UnregisteredComponent();

    Component* Clone(Entity* toEntity) override;
    void Serialize(KeyedArchive* archive, SerializationContext* serializationContext) override;
    void Deserialize(KeyedArchive* archive, SerializationContext* serializationContext) override;

    DAVA_VIRTUAL_REFLECTION(UnregisteredComponent, Component);

private:
    UnregisteredComponent(const KeyedArchive& archive);

private:
    String unregisteredComponent;
};
};
