#include "Classes/Library/Private/SceneImporter.h"

#include <Engine/Engine.h>
#include <FileSystem/FileSystem.h>
#include <Logger/Logger.h>
#include <Render/Shader.h>
#include <Render/Material/NMaterialNames.h>
#include <Scene3D/Components/ComponentHelpers.h>
#include <Scene3D/Components/RenderComponent.h>
#include <Scene3D/Components/SlotComponent.h>
#include <Scene3D/Components/CustomPropertiesComponent.h>
#include <Scene3D/Lod/LodComponent.h>
#include <Utils/StringFormat.h>

struct SceneImporter::ImportParams
{
    static DAVA::Vector<const DAVA::Type*> reImportComponentsIds;

    DAVA::UnorderedMap<const DAVA::Type*, DAVA::Map<DAVA::FastName, DAVA::Vector<DAVA::Component*>>> componentsMap;
    DAVA::Map<DAVA::FastName, DAVA::RefPtr<DAVA::KeyedArchive>> materialsMap;
};

DAVA::Vector<const DAVA::Type*> SceneImporter::ImportParams::reImportComponentsIds = {};

namespace SceneImporterDetails
{
void SaveConfigToArchive(const DAVA::FilePath& sceneDirPath, const DAVA::MaterialConfig& config, DAVA::RefPtr<DAVA::KeyedArchive> archive)
{
    using namespace DAVA;

    if (config.fxName.IsValid())
    {
        archive->SetString(NMaterialSerializationKey::FXName, config.fxName.c_str());
    }

    if (config.name.IsValid())
    {
        archive->SetString(NMaterialSerializationKey::ConfigName, config.name.c_str());
    }

    ScopedPtr<KeyedArchive> propertiesArchive(new KeyedArchive());
    for (const auto& propertyPair : config.localProperties)
    {
        NMaterialProperty* property = propertyPair.second;

        uint32 dataSize = ShaderDescriptor::CalculateDataSize(property->type, property->arraySize) * sizeof(float32);
        uint32 storageSize = sizeof(uint8) + sizeof(uint32) + dataSize;
        uint8* propertyStorage = new uint8[storageSize];

        memcpy(propertyStorage, &property->type, sizeof(uint8));
        memcpy(propertyStorage + sizeof(uint8), &property->arraySize, sizeof(uint32));
        memcpy(propertyStorage + sizeof(uint8) + sizeof(uint32), property->data.get(), dataSize);

        propertiesArchive->SetByteArray(propertyPair.first.c_str(), propertyStorage, storageSize);

        SafeDeleteArray(propertyStorage);
    }
    archive->SetArchive(NMaterialSerializationKey::PropertiesKey, propertiesArchive);

    ScopedPtr<KeyedArchive> texturesArchive(new KeyedArchive());
    for (const auto& texturePair : config.localTextures)
    {
        if (!NMaterialTextureName::IsRuntimeTexture(texturePair.first) && !texturePair.second->path.IsEmpty())
        {
            String textureRelativePath = texturePair.second->path.GetRelativePathname(sceneDirPath);
            if (textureRelativePath.size() > 0)
            {
                texturesArchive->SetString(texturePair.first.c_str(), textureRelativePath);
            }
        }
    }
    archive->SetArchive(NMaterialSerializationKey::TexturesKey, texturesArchive);

    ScopedPtr<KeyedArchive> flagsArchive(new KeyedArchive());
    for (const auto& flagPair : config.localFlags)
    {
        if (!NMaterialFlagName::IsRuntimeFlag(flagPair.first))
            flagsArchive->SetInt32(flagPair.first.c_str(), flagPair.second);
    }
    archive->SetArchive(NMaterialSerializationKey::FlagsKey, flagsArchive);
}

void SaveMaterial(const DAVA::FilePath& sceneDirPath, DAVA::NMaterial* material, DAVA::RefPtr<DAVA::KeyedArchive> archive)
{
    using namespace DAVA;

    const FastName& qualityGroup = material->GetQualityGroup();
    if (qualityGroup.IsValid())
        archive->SetString(NMaterialSerializationKey::QualityGroup, qualityGroup.c_str());

    uint32 configsCount = material->GetConfigCount();
    archive->SetUInt32(NMaterialSerializationKey::ConfigCount, configsCount);
    for (uint32 i = 0; i < configsCount; ++i)
    {
        DAVA::RefPtr<KeyedArchive> configArchive(new KeyedArchive());
        const DAVA::MaterialConfig& config = material->GetConfig(i);
        SaveConfigToArchive(sceneDirPath, config, configArchive);
        archive->SetArchive(Format(NMaterialSerializationKey::ConfigArchive.c_str(), i), configArchive.Get());
    }
}

void LoadConfigFromArchive(const DAVA::FilePath& sceneDirPath, DAVA::MaterialConfig& config, DAVA::RefPtr<DAVA::KeyedArchive> archive)
{
    using namespace DAVA;
    if (archive == nullptr)
    {
        DVASSERT(false);
        return;
    }

    if (archive->IsKeyExists(NMaterialSerializationKey::FXName))
    {
        config.fxName = FastName(archive->GetString(NMaterialSerializationKey::FXName));
    }

    if (archive->IsKeyExists(NMaterialSerializationKey::ConfigName))
    {
        config.name = FastName(archive->GetString(NMaterialSerializationKey::ConfigName));
    }

    if (archive->IsKeyExists(NMaterialSerializationKey::PropertiesKey))
    {
        const KeyedArchive::UnderlyingMap& propsMap = archive->GetArchive(NMaterialSerializationKey::PropertiesKey)->GetArchieveData();
        for (const auto& iter : propsMap)
        {
            const VariantType* propVariant = iter.second;
            DVASSERT(VariantType::TYPE_BYTE_ARRAY == propVariant->type);
            DVASSERT(propVariant->AsByteArraySize() >= static_cast<int32>(sizeof(uint8) + sizeof(uint32)));

            const uint8* ptr = propVariant->AsByteArray();
            FastName propName = FastName(iter.first);
            uint8 propType = *ptr;
            ptr += sizeof(uint8);
            uint32 propSize = *(reinterpret_cast<const uint32*>(ptr));
            ptr += sizeof(uint32);
            const float32* data = reinterpret_cast<const float32*>(ptr);

            NMaterialProperty* newProp = new NMaterialProperty();
            newProp->name = propName;
            newProp->type = rhi::ShaderProp::Type(propType);
            newProp->arraySize = propSize;
            newProp->data.reset(new float32[ShaderDescriptor::CalculateDataSize(newProp->type, newProp->arraySize)]);
            newProp->SetPropertyValue(data);
            config.localProperties[newProp->name] = newProp;
        }
    }

    if (archive->IsKeyExists(NMaterialSerializationKey::TexturesKey))
    {
        const KeyedArchive::UnderlyingMap& texturesMap = archive->GetArchive(NMaterialSerializationKey::TexturesKey)->GetArchieveData();
        for (const auto& iter : texturesMap)
        {
            String relativePathname = iter.second->AsString();
            MaterialTextureInfo* texInfo = new MaterialTextureInfo();
            texInfo->path = sceneDirPath + relativePathname;
            config.localTextures[FastName(iter.first)] = texInfo;
        }
    }

    if (archive->IsKeyExists(NMaterialSerializationKey::FlagsKey))
    {
        const KeyedArchive::UnderlyingMap& flagsMap = archive->GetArchive(NMaterialSerializationKey::FlagsKey)->GetArchieveData();
        for (const auto& iter : flagsMap)
        {
            config.localFlags[FastName(iter.first)] = iter.second->AsInt32();
        }
    }
}

void LoadMaterial(const DAVA::FilePath& sceneDirPath, DAVA::NMaterial* material, DAVA::RefPtr<DAVA::KeyedArchive> archive)
{
    using namespace DAVA;
    while (material->GetConfigCount() > 1)
    {
        material->RemoveConfig(material->GetConfigCount() - 1);
    }

    if (archive->IsKeyExists(NMaterialSerializationKey::QualityGroup))
    {
        material->SetQualityGroup(FastName(archive->GetString(NMaterialSerializationKey::QualityGroup).c_str()));
    }

    uint32 configCount = archive->GetUInt32(NMaterialSerializationKey::ConfigCount, 1);
    Vector<MaterialConfig> configs(configCount, MaterialConfig());

    for (uint32 i = 0; i < configCount; ++i)
    {
        RefPtr<KeyedArchive> configArchive = RefPtr<KeyedArchive>::ConstructWithRetain(archive->GetArchive(Format(NMaterialSerializationKey::ConfigArchive.c_str(), i)));
        LoadConfigFromArchive(sceneDirPath, configs[i], configArchive);
    }

    for (const MaterialConfig& config : configs)
    {
        material->InsertConfig(material->GetConfigCount(), config);
    }
    material->RemoveConfig(0);
}

void AccumulateImportParamsImpl(DAVA::Entity* entity, const DAVA::FilePath& sceneDirPath, SceneImporter::ImportParams* params)
{
    using namespace DAVA;

    for (const DAVA::Type* type : SceneImporter::ImportParams::reImportComponentsIds)
    {
        DAVA::Map<DAVA::FastName, DAVA::Vector<DAVA::Component*>>& componentsMap = params->componentsMap[type];
        DAVA::Vector<DAVA::Component*>& components = componentsMap[DAVA::FastName(entity->GetFullName())];

        DAVA::uint32 componentsCount = entity->GetComponentCount(type);
        components.reserve(componentsCount);
        for (DAVA::uint32 i = 0; i < componentsCount; ++i)
        {
            components.push_back(entity->GetComponent(type, i)->Clone(nullptr));
        }
    }

    RenderComponent* renderComponent = GetRenderComponent(entity);
    if (renderComponent != nullptr)
    {
        RenderObject* renderObject = renderComponent->GetRenderObject();
        for (uint32 i = 0; i < renderObject->GetRenderBatchCount(); ++i)
        {
            RenderBatch* batch = renderObject->GetRenderBatch(i);
            NMaterial* material = batch->GetMaterial();
            while (material != nullptr)
            {
                auto materialIter = params->materialsMap.find(material->GetMaterialName());
                if (materialIter == params->materialsMap.end())
                {
                    RefPtr<KeyedArchive> parentMaterialArchive;
                    parentMaterialArchive.ConstructInplace();
                    SaveMaterial(sceneDirPath, material, parentMaterialArchive);
                    params->materialsMap.emplace(material->GetMaterialName(), parentMaterialArchive);
                }
                material = material->GetParent();
            }
        }
    }

    for (int32 i = 0; i < entity->GetChildrenCount(); ++i)
    {
        AccumulateImportParamsImpl(entity->GetChild(i), sceneDirPath, params);
    }
}

void RestoreSceneParamsImpl(DAVA::Entity* entity, const DAVA::FilePath& sceneDirPath, SceneImporter::ImportParams* params, DAVA::Set<DAVA::FastName>& loadedMaterials)
{
    using namespace DAVA;

    const FastName& entityName = entity->GetName();

    for (const DAVA::Type* type : SceneImporter::ImportParams::reImportComponentsIds)
    {
        DAVA::Map<DAVA::FastName, DAVA::Vector<DAVA::Component*>>& componentsMap = params->componentsMap[type];
        DAVA::Vector<DAVA::Component*>& components = componentsMap[DAVA::FastName(entity->GetFullName())];

        if (components.empty() == true)
        {
            continue;
        }

        DAVA::uint32 componentsCount = entity->GetComponentCount(type);
        DAVA::Vector<DAVA::Component*> componentsToRemove;
        componentsToRemove.reserve(componentsCount);
        for (DAVA::uint32 i = 0; i < componentsCount; ++i)
        {
            componentsToRemove.push_back(entity->GetComponent(type, i));
        }

        for (DAVA::Component* componentToRemove : componentsToRemove)
        {
            entity->RemoveComponent(componentToRemove);
        }
        componentsToRemove.clear();

        for (DAVA::Component* componentToAdd : components)
        {
            entity->AddComponent(componentToAdd);
        }
        components.clear();
    }

    RenderComponent* renderComponent = GetRenderComponent(entity);
    if (renderComponent != nullptr)
    {
        RenderObject* renderObject = renderComponent->GetRenderObject();
        for (uint32 i = 0; i < renderObject->GetRenderBatchCount(); ++i)
        {
            RenderBatch* batch = renderObject->GetRenderBatch(i);
            NMaterial* material = batch->GetMaterial();

            while (material != nullptr)
            {
                FastName materialName = material->GetMaterialName();
                if (loadedMaterials.count(materialName) == 0)
                {
                    auto materialIter = params->materialsMap.find(materialName);
                    if (materialIter != params->materialsMap.end())
                    {
                        LoadMaterial(sceneDirPath, material, materialIter->second);
                    }
                    loadedMaterials.insert(materialName);
                }
                material = material->GetParent();
            }
        }
    }

    for (int32 i = 0; i < entity->GetChildrenCount(); ++i)
    {
        RestoreSceneParamsImpl(entity->GetChild(i), sceneDirPath, params, loadedMaterials);
    }
}
}

SceneImporter::SceneImporter(const DAVA::FilePath& scenePathname_)
    : scenePathname(scenePathname_)
{
    params = std::make_unique<ImportParams>();

    if (params->reImportComponentsIds.empty())
    {
        params->reImportComponentsIds = {
            DAVA::Type::Instance<DAVA::LodComponent>(),
            DAVA::Type::Instance<DAVA::SlotComponent>(),
            DAVA::Type::Instance<DAVA::CustomPropertiesComponent>()
        };
    }
}

SceneImporter::~SceneImporter()
{
    for (const auto& typeIter : params->componentsMap)
    {
        const DAVA::Map<DAVA::FastName, DAVA::Vector<DAVA::Component*>>& subMap = typeIter.second;
        for (const auto& nameMap : subMap)
        {
            const DAVA::Vector<DAVA::Component*>& components = nameMap.second;
            for (DAVA::Component* component : components)
            {
                DAVA::SafeDelete(component);
            }
        }
    }

    params->componentsMap.clear();
    params->materialsMap.clear();
}

void SceneImporter::AccumulateParams()
{
    using namespace DAVA;
    if (GetEngineContext()->fileSystem->Exists(scenePathname) == true)
    {
        ScopedPtr<Scene> scene(new Scene());
        SceneFileV2::eError loadResult = scene->LoadScene(scenePathname);
        if (loadResult == SceneFileV2::ERROR_NO_ERROR)
        {
            SceneImporterDetails::AccumulateImportParamsImpl(scene.get(), scenePathname.GetDirectory(), params.get());
        }
    }
}

void SceneImporter::RestoreParams()
{
    using namespace DAVA;
    ScopedPtr<Scene> scene(new Scene());
    SceneFileV2::eError result = scene->LoadScene(scenePathname);
    if (result == SceneFileV2::ERROR_NO_ERROR)
    {
        Set<FastName> loadedMaterials;
        SceneImporterDetails::RestoreSceneParamsImpl(scene.get(), scenePathname.GetDirectory(), params.get(), loadedMaterials);

        result = scene->SaveScene(scenePathname, false);
    }

    if (result != SceneFileV2::ERROR_NO_ERROR)
    {
        DAVA::Logger::Error("[%s] Failed to apply reimport params for scene %s {%u}", __FUNCTION__, scenePathname.GetStringValue().c_str(), result);
    }
}
