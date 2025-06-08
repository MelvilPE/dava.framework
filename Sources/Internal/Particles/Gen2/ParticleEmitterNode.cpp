#include "ParticleEmitterNode.h"

#include "Reflection/ReflectionRegistrator.h"

#include "Scene3D/Systems/QualitySettingsSystem.h"

#include "FileSystem/YamlParser.h"
#include "Utils/StringFormat.h"
#include "Utils/Utils.h"

#include "Logger/Logger.h"

namespace DAVA
{
DAVA_VIRTUAL_REFLECTION_IMPL(ParticleEmitterNode)
{
    ReflectionRegistrator<ParticleEmitterNode>::Begin()
    .End();
}

ParticleEmitterNode::ParticleEmitterNode()
    : DataNode()
{
}

ParticleEmitterNode::~ParticleEmitterNode()
{
}

void ParticleEmitterNode::Save(KeyedArchive* archive, SerializationContext* serializationContext)
{
    //DataNode::Save(archive, serializationContext);
    archive->LoadFromYamlString(nodeYaml);
}

void ParticleEmitterNode::Load(KeyedArchive* archive, SerializationContext* serializationContext)
{
    //DataNode::Load(archive, serializationContext);
    nodeYaml = archive->SaveToYamlString();
}

String ParticleEmitterNode::GetNodeYaml()
{
    return nodeYaml;
}

void ParticleEmitterNode::SetNodeYaml(String value)
{
    nodeYaml = value;
}
}; // namespace DAVA
