#include "Render/Material/NMaterialStateDynamicFlagsInsp.h"
#include "Render/Material/FXCache.h"
#include "Render/Material/NMaterial.h"

namespace DAVA
{
///////////////////////////////////////////////////////////////////////////
///// NMaterialStateDynamicFlagsInsp implementation

InspInfoDynamic::DynamicData NMaterialStateDynamicFlagsInsp::Prepare(void* object, int filter) const
{
    InspInfoDynamic::DynamicData ddata;
    ddata.object = object;

    return ddata;
}

Vector<FastName> NMaterialStateDynamicFlagsInsp::MembersList(const DynamicData& ddata) const
{
    static Vector<FastName> ret;

    if (ret.empty())
    {
        ret.push_back(NMaterialFlagName::FLAG_BLENDING);
        ret.push_back(NMaterialFlagName::FLAG_VERTEXFOG);
        ret.push_back(NMaterialFlagName::FLAG_FOG_LINEAR);
        ret.push_back(NMaterialFlagName::FLAG_FOG_HALFSPACE);
        ret.push_back(NMaterialFlagName::FLAG_FOG_HALFSPACE_LINEAR);
        ret.push_back(NMaterialFlagName::FLAG_FOG_ATMOSPHERE);
        ret.push_back(NMaterialFlagName::FLAG_FOG_ATMOSPHERE_NO_ATTENUATION);
        ret.push_back(NMaterialFlagName::FLAG_FOG_ATMOSPHERE_NO_SCATTERING);
        ret.push_back(NMaterialFlagName::FLAG_TEXTURESHIFT);
        ret.push_back(NMaterialFlagName::FLAG_ENABLE_FOG);
        ret.push_back(NMaterialFlagName::FLAG_ENABLE_HIGH_QUALITY_FOG);
        ret.push_back(NMaterialFlagName::FLAG_TEXTURE_SHIFT);
        ret.push_back(NMaterialFlagName::FLAG_TEXTURE0_ANIMATION_SHIFT);
        ret.push_back(NMaterialFlagName::FLAG_WAVE_ANIMATION);
        ret.push_back(NMaterialFlagName::FLAG_FAST_NORMALIZATION);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_MASK);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_ROTATION);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_TRANSFORM);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_ANIMATED_EMISSION);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_BLEND_NORMAL);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_OVERRIDE_ROUGHNESS_METALLIC);
        ret.push_back(NMaterialFlagName::FLAG_ALBEDO_TRANSFORM);
        ret.push_back(NMaterialFlagName::FLAG_FLATCOLOR);
        ret.push_back(NMaterialFlagName::FLAG_FLAT_ALBEDO);
        ret.push_back(NMaterialFlagName::FLAG_DISTANCE_ATTENUATION);
        ret.push_back(NMaterialFlagName::FLAG_SPECULAR);
        ret.push_back(NMaterialFlagName::FLAG_SEPARATE_NORMALMAPS);
        ret.push_back(NMaterialFlagName::FLAG_VERTEX_COLOR);
        ret.push_back(NMaterialFlagName::FLAG_SPEED_TREE_OBJECT);
        ret.push_back(NMaterialFlagName::FLAG_SPEEDTREE_JOINT_TRANSFORM);
        ret.push_back(NMaterialFlagName::FLAG_SPEEDTREE_JOINT_LENGTHWISE_TRANSFORM);
        ret.push_back(NMaterialFlagName::FLAG_SPHERICAL_LIT);
        ret.push_back(NMaterialFlagName::FLAG_TANGENT_SPACE_WATER_REFLECTIONS);
        ret.push_back(NMaterialFlagName::FLAG_DEBUG_UNITY_Z_NORMAL);
        ret.push_back(NMaterialFlagName::FLAG_DEBUG_Z_NORMAL_SCALE);
        ret.push_back(NMaterialFlagName::FLAG_DEBUG_NORMAL_ROTATION);
        ret.push_back(NMaterialFlagName::FLAG_HARD_SKINNING);
        ret.push_back(NMaterialFlagName::FLAG_SOFT_SKINNING);
        ret.push_back(NMaterialFlagName::FLAG_FLOWMAP_SKY);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_MASK);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_FLOWMAP);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_FLOWMAP_ANIMATION);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_PERSPECTIVE_MAPPING);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_THREE_POINT_GRADIENT);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_NOISE);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_FRESNEL_TO_ALPHA);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_ALPHA_REMAP);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_VERTEX_ANIMATION);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_VERTEX_ANIMATION_MASK);
        ret.push_back(NMaterialFlagName::FLAG_LIGHTMAPONLY);
        ret.push_back(NMaterialFlagName::FLAG_TEXTUREONLY);
        ret.push_back(NMaterialFlagName::FLAG_SETUPLIGHTMAP);
        ret.push_back(NMaterialFlagName::FLAG_VIEWALBEDO);
        ret.push_back(NMaterialFlagName::FLAG_VIEWNORMAL);
        ret.push_back(NMaterialFlagName::FLAG_VIEWNORMAL_FINAL);
        ret.push_back(NMaterialFlagName::FLAG_VIEWROUGHNESS);
        ret.push_back(NMaterialFlagName::FLAG_VIEWMETALLIC);
        ret.push_back(NMaterialFlagName::FLAG_VIEWAMBIENT_OCCLUSION);
        ret.push_back(NMaterialFlagName::FLAG_VIEWAMBIENT);
        ret.push_back(NMaterialFlagName::FLAG_VIEWDIFFUSE);
        ret.push_back(NMaterialFlagName::FLAG_VIEWSPECULAR);
        ret.push_back(NMaterialFlagName::FLAG_ALPHA_EROSION);
        ret.push_back(NMaterialFlagName::FLAG_SOFT_PARTICLES);
        ret.push_back(NMaterialFlagName::FLAG_FRAME_BLEND);
        ret.push_back(NMaterialFlagName::FLAG_FORCE_2D_MODE);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_USE_INSTANCING);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_LOD_MORPHING);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_MORPHING_COLOR);
        ret.push_back(NMaterialFlagName::FLAG_HEIGHTMAP_FLOAT_TEXTURE);
        ret.push_back(NMaterialFlagName::FLAG_ALPHA_TEST);
        ret.push_back(NMaterialFlagName::FLAG_ALPHA_TEST_VALUE);
        ret.push_back(NMaterialFlagName::FLAG_ALPHA_STEP_VALUE);
        ret.push_back(NMaterialFlagName::FLAG_ILLUMINATION_USED);
        ret.push_back(NMaterialFlagName::FLAG_ILLUMINATION_SHADOW_CASTER);
        ret.push_back(NMaterialFlagName::FLAG_ILLUMINATION_SHADOW_RECEIVER);
        ret.push_back(NMaterialFlagName::FLAG_TEST_OCCLUSION);
        ret.push_back(NMaterialFlagName::FLAG_FORCED_SHADOW_DIRECTION);
        ret.push_back(NMaterialFlagName::FLAG_PARTICLES_DEBUG_SHOW_HEATMAP);
        ret.push_back(NMaterialFlagName::FLAG_GEO_DECAL);
        ret.push_back(NMaterialFlagName::FLAG_GEO_DECAL_SPECULAR);
        ret.push_back(NMaterialFlagName::FLAG_VERTEX_DISTORTION_DEPRECATED);
        ret.push_back(NMaterialFlagName::FLAG_NORMAL_DETAIL);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_USE_RELAXMAP);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_HEIGHT_BLEND);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_SEPARATE_LIGHTMAP_CHANNEL);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_HAS_METALLIC_AND_EMISSION);
        ret.push_back(NMaterialFlagName::FLAG_LANDSCAPE_SCALED_TILES_NON_PBR);
        ret.push_back(NMaterialFlagName::FLAG_ALLOW_POINT_LIGHTS);
        ret.push_back(NMaterialFlagName::FLAG_FORCE_POINT_LIGHTS_ENABLED);
        ret.push_back(NMaterialFlagName::FLAG_ENVIRONMENT_MAPPING);
        ret.push_back(NMaterialFlagName::FLAG_ENVIRONMENT_MAPPING_NORMALMAP);
        ret.push_back(NMaterialFlagName::FLAG_BLEND_BY_ANGLE);
        ret.push_back(NMaterialFlagName::FLAG_SHADOW_RECEIVER);
        ret.push_back(NMaterialFlagName::FLAG_USE_SHADOW_MAP);
        ret.push_back(NMaterialFlagName::FLAG_DEBUG_SHADOW_CASCADES);
        ret.push_back(NMaterialFlagName::FLAG_SHADOW_CASCADES_COUNT);
        ret.push_back(NMaterialFlagName::FLAG_SHADOW_CASCADES_BLEND);
        ret.push_back(NMaterialFlagName::FLAG_SHADOW_PCF);
        ret.push_back(NMaterialFlagName::FLAG_DECAL_ALBEDO);
        ret.push_back(NMaterialFlagName::FLAG_DECAL_NORMAL);
        ret.push_back(NMaterialFlagName::FLAG_DECAL_TREAD);
        ret.push_back(NMaterialFlagName::FLAG_FADE_OUT_WITH_TIME);
        ret.push_back(NMaterialFlagName::DEPTH_TARGET_IS_FRAMEBUFFER);
        ret.push_back(NMaterialFlagName::DEPTH_PREPASS_ENABLED);
        ret.push_back(NMaterialFlagName::FLAG_VEGETATION_BEND);
        ret.push_back(NMaterialFlagName::VEGETATION_LIT);
        ret.push_back(NMaterialFlagName::FLAG_PUSH_TO_NEAR_PLANE_HACK);
        ret.push_back(NMaterialFlagName::FLAG_DISTANCE_FADE_OUT);
        ret.push_back(NMaterialFlagName::FLAG_POINT_LIGHTS_OVERRIDE_SHADOW);
        ret.push_back(NMaterialFlagName::FLAG_DIFFUSE_IN_LIGHTMAP);
        ret.push_back(NMaterialFlagName::FLAG_AMBIENT_ATTENUATION_BOX);
        ret.push_back(NMaterialFlagName::PASS_NAME_ENGINE_DEFINE);
        ret.push_back(NMaterialFlagName::FLAG_DECAL_TEXTURE_COUNT);
        ret.push_back(NMaterialFlagName::FLAG_VIEW_MODE_OVERDRAW_HEAT);
        ret.push_back(NMaterialFlagName::FLAG_WATER_RIPPLES_ENABLED);
        ret.push_back(NMaterialFlagName::FLAG_WATER_RIPPLES_ALLOWED);
        ret.push_back(NMaterialFlagName::FLAG_INSTANCED_CHAIN);
        ret.push_back(NMaterialFlagName::FLAG_DIRT_COVERAGE);
        ret.push_back(NMaterialFlagName::FLAG_WETNESS_MULTILEVEL);
        ret.push_back(NMaterialFlagName::FLAG_WETNESS_SIMPLIFIED);
        ret.push_back(NMaterialFlagName::FLAG_COLORBLIND_MODE);
        ret.push_back(NMaterialFlagName::FLAG_DEBUG_UNLIT);
        ret.push_back(NMaterialFlagName::FLAG_HIGHLIGHT_COLOR);
        ret.push_back(NMaterialFlagName::FLAG_HIGHLIGHT_WAVE_ANIM);
        ret.push_back(NMaterialFlagName::FLAG_DECAL_VERTICAL_FADE);
        ret.push_back(NMaterialFlagName::FLAG_DECAL_BACK_SIDE_FADE);
        ret.push_back(NMaterialFlagName::FLAG_WIND_ANIMATION);
        ret.push_back(NMaterialFlagName::FLAG_SHADER_ULTRA);
        ret.push_back(NMaterialFlagName::FLAG_MATERIAL_DECAL);
        ret.push_back(NMaterialFlagName::FLAG_MATERIAL_DETAIL);
        ret.push_back(NMaterialFlagName::FLAG_MATERIAL_LIGHTMAP);
        ret.push_back(NMaterialFlagName::FLAG_PBR_DECAL);
        ret.push_back(NMaterialFlagName::FLAG_PBR_DETAIL);
        ret.push_back(NMaterialFlagName::FLAG_PBR_LIGHTMAP);
        ret.push_back(NMaterialFlagName::FLAG_IGNORE_GLOBAL_FLAT_COLOR);
        ret.push_back(NMaterialFlagName::FLAG_IGNORE_LIGHTMAP_ADJUSTMENT);
        ret.push_back(NMaterialFlagName::FLAG_IGNORE_BASE_COLOR_PBR_TINT);
        ret.push_back(NMaterialFlagName::FLAG_IGNORE_ROUGHNESS_PBR_TINT);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_LOD_TRANSITION_NEAR);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_LOD_TRANSITION_FAR);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_BILLBOARD);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_AMBIENT_ANIMATION);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_WIND_ANIMATION);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_WAVE_ANIMATION);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_PBR_LIGHTING);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_NORMAL_MAP);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_EDGE_MAP);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_FAKE_SHADOW);
        ret.push_back(NMaterialFlagName::FLAG_FLORA_LAYING);
        ret.push_back(NMaterialFlagName::FLAG_EMISSIVE_COLOR);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_EMISSIVE_COLOR);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_NOISE_SPREADING);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_SPATIAL_SPREADING);
        ret.push_back(NMaterialFlagName::FLAG_EMISSIVE_ALBEDO);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_EMISSIVE_ALBEDO);
        ret.push_back(NMaterialFlagName::FLAG_TILED_DECAL_ANIM_MASK);
        ret.push_back(NMaterialFlagName::FLAG_WATER_RENDER_OBJECT);
        ret.push_back(NMaterialFlagName::FLAG_WATER_TESSELLATION);
        ret.push_back(NMaterialFlagName::FLAG_WATER_DEFORMATION);
        ret.push_back(NMaterialFlagName::FLAG_BILLBOARD_FACE_MAIN_CAMERA);
        ret.push_back(NMaterialFlagName::FLAG_VERTEX_VERTICAL_OFFSET);
        ret.push_back(NMaterialFlagName::FLAG_VERTEX_DISPLACEMENT);
        ret.push_back(NMaterialFlagName::FLAG_GLOBAL_TINT);
        ret.push_back(NMaterialFlagName::FLAG_GLOBAL_PBR_TINT);
    }

    return ret;
}

InspDesc NMaterialStateDynamicFlagsInsp::MemberDesc(const DynamicData& ddata, const FastName& member) const
{
    return InspDesc(member.c_str());
}

VariantType NMaterialStateDynamicFlagsInsp::MemberValueGet(const DynamicData& ddata, const FastName& member) const
{
    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    VariantType ret;
    ret.SetBool(0 != material->GetEffectiveFlagValue(member));
    return ret;
}

void NMaterialStateDynamicFlagsInsp::MemberValueSet(const DynamicData& ddata, const FastName& member, const VariantType& value)
{
    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    // empty value is thread as flag remove
    if (value.GetType() == VariantType::TYPE_NONE)
    {
        if (material->HasLocalFlag(member))
        {
            material->RemoveFlag(member);
        }
    }
    else
    {
        int32 newValue = 0;
        if ((value.GetType() == VariantType::TYPE_BOOLEAN) && value.AsBool())
        {
            newValue = 1;
        }

        if (material->HasLocalFlag(member))
        {
            material->SetFlag(member, newValue);
        }
        else
        {
            material->AddFlag(member, newValue);
        }
    }
}

int NMaterialStateDynamicFlagsInsp::MemberFlags(const DynamicData& ddata, const FastName& member) const
{
    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    return I_VIEW | (material->HasLocalFlag(member) ? I_EDIT : 0);
}
}; // namespace DAVA