#include "Render/Material/NMaterial.h"
#include "Render/Material/NMaterialStateDynamicTexturesInsp.h"
#include "Render/Material/FXCache.h"
#include "Scene3D/Systems/QualitySettingsSystem.h"

namespace DAVA
{
///////////////////////////////////////////////////////////////////////////
///// NMaterialStateDynamicTexturesInsp implementation

NMaterialStateDynamicTexturesInsp::NMaterialStateDynamicTexturesInsp()
{
    defaultTexture = Texture::CreatePink();
}

NMaterialStateDynamicTexturesInsp::~NMaterialStateDynamicTexturesInsp()
{
    SafeRelease(defaultTexture);
}

void NMaterialStateDynamicTexturesInsp::FindMaterialTexturesRecursive(NMaterial* material, Set<FastName>& ret) const
{
    auto fxName = material->GetEffectiveFXName();
    if (fxName.IsValid())
    {
        UnorderedMap<FastName, int32> flags;
        material->CollectMaterialFlags(flags);

        // shader data
        FXDescriptor fxDescriptor = FXCache::GetFXDescriptor(fxName, flags, QualitySettingsSystem::Instance()->GetCurMaterialQuality(material->qualityGroup));
        for (auto& descriptor : fxDescriptor.renderPassDescriptors)
        {
            if (!descriptor.shader->IsValid())
                continue;

            const rhi::ShaderSamplerList& fragmentSamplers = descriptor.shader->GetFragmentSamplerList();
            for (const auto& samp : fragmentSamplers)
            {
                ret.insert(samp.uid);
            }

            const rhi::ShaderSamplerList& vertexSamplers = descriptor.shader->GetVertexSamplerList();
            for (const auto& samp : vertexSamplers)
            {
                ret.insert(samp.uid);
            }
        }
    }
    else
    {
        // if fxName is not valid (e.g global material)
        // we just add all local textures
        const MaterialConfig& config = material->GetCurrentConfig();
        for (const auto& t : config.localTextures)
            ret.insert(t.first);
    }

    if (nullptr != material->GetParent())
        FindMaterialTexturesRecursive(material->GetParent(), ret);
}

void NMaterialStateDynamicTexturesInsp::SetAllPossibleMaterialTextures(NMaterial* material, Set<FastName>& ret) const
{
    ret.insert(FastName(NMaterialTextureName::TEXTURE_ALBEDO));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_NORMAL));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_SPECULAR));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DETAIL));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_LIGHTMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LIGHTMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DECAL));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_CUBEMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_HEIGHTMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_TANGENTSPACE));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DECALMASK));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DECALTEXTURE));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_TILED_DECAL_1));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_TILED_DECAL_2));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_TILED_DECAL_3));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_MASK));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_FLOW));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_NOISE));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_ALPHA_REMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_REFLECTION));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_REFRACTION));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PARTICLES_HEATMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PARTICLES_RT));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_ENV_REFLECTION_MASK));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DIRT_NORMAL));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DIRT_HEIGHTMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_WETNESS_BOUNDARY));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_VERTEX_ANIMATION_TEXTURE));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_SHADOW_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_DEPTH_PREPASS));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_BLUE_NOISE));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_PERLIN_NOISE));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_SPECULAR_BRDF));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_SPECULAR_REFLECTION_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_DIFFUSE_IRRADIANCE_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_DYNAMIC_SRC_0));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_VEGETATION_COLOR_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_VEGETATION_DENSITY_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_BASE_COLOR_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_BASE_NORMAL_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_BASE_RM_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_MISC_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_MASK_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_ROUGHNESS_AO_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_ALBEDO_ROUGHNESS_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_NORMAL_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_LIGHTMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_TILE_ALBEDO_HEIGHT_ARRAY));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_TILE_NORMAL_ARRAY));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_TILE_ROUGHNESS_AO_ARRAY));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_TILE_ROUGHNESS_METALLIC_ARRAY));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_LANDSCAPE_TILE_AO_EMISSION_ARRAY));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DECAL_COLOR_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DECAL_NORMAL_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DECAL_RM_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DECAL_COLOR_ROUGHNESS_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DECAL_LIGHTMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DETAIL_COLOR_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DETAIL_NORMAL_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_PBR_DETAIL_ROUGHNESS_AO_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_FLORA_COLOR_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_FLORA_PBR_COLOR_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_FLORA_LANDSCAPE_NORMAL_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_FLORA_LIGHTMAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_FLORA_EDGE_MAP));
    ret.insert(FastName(NMaterialTextureName::TEXTURE_FLORA_FAKE_SHADOW));
}

InspInfoDynamic::DynamicData NMaterialStateDynamicTexturesInsp::Prepare(void* object, int filter) const
{
    NMaterial* material = static_cast<NMaterial*>(object);
    DVASSERT(material);

    Set<FastName>* data = new Set<FastName>();
    SetAllPossibleMaterialTextures(material, *data);

    // DF-10204, we don't allow change heightmap in material for new Landscape.
    data->erase(NMaterialTextureName::TEXTURE_HEIGHTMAP);

    if (filter > 0)
    {
        auto checkAndAdd = [&data](const FastName& name) {
            if (0 == data->count(name))
            {
                data->insert(name);
            }
        };

        checkAndAdd(NMaterialTextureName::TEXTURE_ALBEDO);
        checkAndAdd(NMaterialTextureName::TEXTURE_NORMAL);
        checkAndAdd(NMaterialTextureName::TEXTURE_DETAIL);
        checkAndAdd(NMaterialTextureName::TEXTURE_LIGHTMAP);
        checkAndAdd(NMaterialTextureName::TEXTURE_DECAL);
        checkAndAdd(NMaterialTextureName::TEXTURE_CUBEMAP);
        checkAndAdd(NMaterialTextureName::TEXTURE_DECALMASK);
        checkAndAdd(NMaterialTextureName::TEXTURE_DECALTEXTURE);
    }

    DynamicData ddata;
    ddata.object = object;
    ddata.data = std::shared_ptr<void>(data);
    return ddata;
}

Vector<FastName> NMaterialStateDynamicTexturesInsp::MembersList(const DynamicData& ddata) const
{
    Vector<FastName> ret;

    Set<FastName>* textures = static_cast<Set<FastName>*>(ddata.data.get());
    DVASSERT(textures);

    auto it = textures->begin();
    auto end = textures->end();

    ret.reserve(textures->size());
    while (it != end)
    {
        ret.push_back(*it);
        ++it;
    }

    return ret;
}

InspDesc NMaterialStateDynamicTexturesInsp::MemberDesc(const DynamicData& ddata, const FastName& textureName) const
{
    return InspDesc(textureName.c_str());
}

VariantType NMaterialStateDynamicTexturesInsp::MemberValueGet(const DynamicData& ddata, const FastName& textureName) const
{
    VariantType ret;

    Set<FastName>* textures = static_cast<Set<FastName>*>(ddata.data.get());

    DVASSERT(textures);

    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    if (textures->count(textureName))
    {
        Texture* tex = material->GetEffectiveTexture(textureName);
        if (nullptr != tex)
        {
            ret.SetFilePath(tex->GetPathname());
        }
        else
        {
            ret.SetFilePath(FilePath());
        }
    }

    return ret;
}

void NMaterialStateDynamicTexturesInsp::MemberValueSet(const DynamicData& ddata, const FastName& textureName, const VariantType& value)
{
    VariantType ret;

    Set<FastName>* textures = static_cast<Set<FastName>*>(ddata.data.get());
    DVASSERT(textures);

    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    if (textures->count(textureName))
    {
        if (value.type == VariantType::TYPE_NONE)
        {
            if (material->HasLocalTexture(textureName))
            {
                material->RemoveTexture(textureName);
            }
        }
        else
        {
            FilePath texPath = value.AsFilePath();
            Texture* texture = nullptr;

            if (texPath == FilePath())
            {
                texture = SafeRetain(defaultTexture);
            }
            else
            {
                texture = Texture::CreateFromFile(texPath);
            }

            if (material->HasLocalTexture(textureName))
            {
                material->SetTexture(textureName, texture);
            }
            else
            {
                material->AddTexture(textureName, texture);
            }

            SafeRelease(texture);
        }
    }
}

int NMaterialStateDynamicTexturesInsp::MemberFlags(const DynamicData& ddata, const FastName& textureName) const
{
    int flags = 0;

    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    flags |= I_VIEW;

    if (material->HasLocalTexture(textureName))
    {
        flags |= I_EDIT;
    }

    return flags;
}
};
