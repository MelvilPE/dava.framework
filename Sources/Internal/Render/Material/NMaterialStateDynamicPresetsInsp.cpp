#include "Render/Material/NMaterial.h"
#include "Render/Material/FXCache.h"
#include "Render/Material/NMaterialStateDynamicPresetsInsp.h"

namespace DAVA
{
///////////////////////////////////////////////////////////////////////////
///// NMaterialStateDynamicPresetsInsp implementation

InspInfoDynamic::DynamicData NMaterialStateDynamicPresetsInsp::Prepare(void* object, int filter) const
{
    InspInfoDynamic::DynamicData ddata;
    ddata.object = object;

    return ddata;
}

Vector<FastName> NMaterialStateDynamicPresetsInsp::MembersList(const DynamicData& ddata) const
{
    static Vector<FastName> ret;

    if (ret.empty())
    {
        ret.push_back(NMaterialPresetName::ADDITIVE);
        ret.push_back(NMaterialPresetName::REFLECTION_MASK);
        ret.push_back(NMaterialPresetName::MULTIPLICATIVE);
        ret.push_back(NMaterialPresetName::FORCE_AFTER_OPAQUE_LAYER);
        ret.push_back(NMaterialPresetName::DISABLE_SHADOW);
        ret.push_back(NMaterialPresetName::FORCE_ALPHA_TEST_LAYER);
        ret.push_back(NMaterialPresetName::ALPHA_TEST);
        ret.push_back(NMaterialPresetName::FOREGROUND_EFFECT);
        ret.push_back(NMaterialPresetName::HIGHLIGHT_ANIMATION);
        ret.push_back(NMaterialPresetName::LOD_TRANSITION);
        ret.push_back(NMaterialPresetName::DISABLE_SHADOW_TRANSITION);
        ret.push_back(NMaterialPresetName::PRESET_OPAQUE);
        ret.push_back(NMaterialPresetName::CLAMP);
        ret.push_back(NMaterialPresetName::LIGHTMAP);
        ret.push_back(NMaterialPresetName::NO_DEPTH_TEST);
        ret.push_back(NMaterialPresetName::FORCE_VEGETATION_LAYER);
        ret.push_back(NMaterialPresetName::ALPHA_BLEND_ADDITIVE);
        ret.push_back(NMaterialPresetName::DEBUG);
        ret.push_back(NMaterialPresetName::WATER_DEFORMATION);
        ret.push_back(NMaterialPresetName::ALPHA_MASK);
        ret.push_back(NMaterialPresetName::FORCE_DRAW_DEPTH_PREPASS);
        ret.push_back(NMaterialPresetName::DISABLE_DEPTH_WRITE);
        ret.push_back(NMaterialPresetName::DEBUG_COLOR_UNLIT);
        ret.push_back(NMaterialPresetName::DISABLE_SHADOW_RUNTIME_ONLY);
        ret.push_back(NMaterialPresetName::ALPHA_BLEND);
        ret.push_back(NMaterialPresetName::SIMPLE_COLOR);
        ret.push_back(NMaterialPresetName::BLEND_WITH_CONST_ALPHA);
        ret.push_back(NMaterialPresetName::FORCE_AFTER_DYNAMIC_DECALS_LAYER);
        ret.push_back(NMaterialPresetName::DYNAMIC_DECAL_LAYER);
        ret.push_back(NMaterialPresetName::FLOW_MAP);
        ret.push_back(NMaterialPresetName::FORCE_TRANCLUCENT_LAYER);
        ret.push_back(NMaterialPresetName::VERTEX_COLOR);
        ret.push_back(NMaterialPresetName::CULL_FRONT_DEPTH_GREATER);
    }

    return ret;
}

InspDesc NMaterialStateDynamicPresetsInsp::MemberDesc(const DynamicData& ddata, const FastName& member) const
{
    return InspDesc(member.c_str());
}

VariantType NMaterialStateDynamicPresetsInsp::MemberValueGet(const DynamicData& ddata, const FastName& member) const
{
    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    VariantType ret;
    ret.SetBool(0 != material->GetEffectiveFlagValue(member));
    return ret;
}

void NMaterialStateDynamicPresetsInsp::MemberValueSet(const DynamicData& ddata, const FastName& member, const VariantType& value)
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

int NMaterialStateDynamicPresetsInsp::MemberFlags(const DynamicData& ddata, const FastName& member) const
{
    NMaterial* material = static_cast<NMaterial*>(ddata.object);
    DVASSERT(material);

    return I_VIEW | (material->HasLocalFlag(member) ? I_EDIT : 0);
}
};
