#pragma once

#include "Base/BaseTypes.h"

#include "Entity/SceneSystem.h"
#include "Math/Vector.h"

namespace DAVA
{
class ParticleDebugRenderPass;
class ParticleDebugDrawQuadRenderPass;
class Texture;
class NMaterial;
class RenderObject;
class RenderSystem;

enum eParticleDebugDrawMode
{
    WIREFRAME,
    LOW_ALPHA,
    OVERDRAW
};

class ParticleEffectDebugDrawSystem : public SceneSystem
{
public:
    /**
     * @brief Constructor for the ParticleEffectDebugDrawSystem class.
     * 
     * This system handles the debug visualization of particle effects in the 3D scene.
     * 
     * @param scene Pointer to the Scene object that this system will operate on.
     */
    ParticleEffectDebugDrawSystem(Scene* scene);
    /**
     * @brief Virtual destructor of ParticleEffectDebugDrawSystem.
     * Ensures proper cleanup of the particle effect debug drawing system resources.
     */
    ~ParticleEffectDebugDrawSystem();

    /**
     * @brief Prepares the system for removal. Empty implementation as no cleanup is needed.
     * @override Overrides the base class method.
     */
    void PrepareForRemove() override
    {
    }
    /**
     * @brief Performs drawing of particle effect debug information.
     * 
     * This function handles the visualization of debug information for particle effects 
     * in the 3D scene, which may include particle emitters, boundaries, and other 
     * relevant debug data.
     */
    void Draw();

    /**
     * @brief Generates debug materials used for particle effect visualization.
     * 
     * Creates and initializes materials required for debug rendering of particle
     * effects within the 3D scene. These materials are specifically used for
     * debug visualization purposes.
     */
    void GenerateDebugMaterials();
    /**
     * @brief Initializes or regenerates the materials required for quad rendering.
     * 
     * This method is responsible for creating and setting up the necessary materials 
     * used in rendering particle effect debug quads. This includes creating materials
     * for both filled and wireframe visualization modes.
     */
    void GenerateQuadMaterials();

    /**
     * Generates a heat texture used for particle effect visualization.
     * 
     * @return A pointer to the generated Texture object. The caller is responsible for managing the texture lifecycle.
     */
    Texture* GenerateHeatTexture() const;
    /**
     * Performs linear interpolation between colors based on the normalized width.
     * @param normalizedWidth Value between 0 and 1 representing the position for color interpolation
     * @return Interpolated color as Vector4 (RGBA)
     */
    Vector4 LerpColors(float normalizedWidth) const;

    /**
     * @brief Returns the current particle effect debug draw mode
     * @return Current debug draw mode for particle effects
     */
    inline eParticleDebugDrawMode GetDrawMode() const;
    /**
     * @brief Sets the debug draw mode for particle effects visualization.
     * @param[in] mode The debug draw mode to set. Can be one of the eParticleDebugDrawMode enum values
     *                 that controls how particle effects are visually represented in debug mode.
     */
    inline void SetDrawMode(eParticleDebugDrawMode mode);

    /**
     * @return true if the system is drawing only selected particle effects, false if drawing all particle effects
     */
    inline bool GetIsDrawOnlySected() const;
    /**
     * @brief Sets whether to draw only selected particle effects.
     * @param[in] showOnlySelected If true, only selected particle effects will be drawn.
     *                            If false, all particle effects will be drawn.
     */
    inline void SetIsDrawOnlySelected(bool showOnlySelected);

    /**
     * @brief Returns an array of materials used by the particle effect
     * @return Constant pointer to constant vector of material pointers
     */
    inline const Vector<NMaterial*>* const GetMaterials() const;

    /**
     * @brief Sets the collection of particle effect objects that are currently selected.
     * This method updates the internal state to track which particle effects should be highlighted or processed differently in debug visualization.
     * 
     * @param selectedParticles An unordered set containing pointers to RenderObject instances representing the selected particle effects
     */
    void SetSelectedParticles(UnorderedSet<RenderObject*> selectedParticles);
    /**
     * @brief Sets the alpha threshold value for particle effect debug drawing
     * @param[in] threshold The alpha threshold value (between 0.0 and 1.0)
     * @details This threshold determines the minimum alpha value at which particles will be rendered in debug mode
     */
    void SetAlphaThreshold(float32 threshold);

private:
    struct TextureKey
    {
        Vector4 color = {};
        float32 time = 0.0f;
        /**
         * @brief A key structure used for texture animation in particle effects.
         * 
         * @param color The color value represented as a 4D vector (RGBA)
         * @param time The time value for this texture key in seconds
         */
        TextureKey(Vector4 color, float32 time)
            : color(color)
            , time(time)
        {
        }
    };

    /**
     * @brief Restores the system's state after its execution has been interrupted or suspended.
     * 
     * This function is responsible for bringing the particle effect debug draw system back to
     * its operational state after any interruption in its normal execution flow.
     */
    void Restore();
    /**
     * @brief Generates texture data for heat visualization
     * @param[out] data Pointer to the buffer where the heat texture data will be written
     * @param[in] dataSize Size of the buffer in bytes
     * @param[in] width Width of the texture in pixels
     * @param[in] height Height of the texture in pixels, defaults to 1
     */
    void GenerateHeatTextureData(unsigned char* data, size_t dataSize, int32 width, int32 height = 1) const;

    UnorderedSet<RenderObject*> selectedParticles;
    ParticleDebugRenderPass* renderPass = nullptr;
    ParticleDebugDrawQuadRenderPass* drawQuadPass = nullptr;
    RenderSystem* renderSystem = nullptr;

    bool isDrawOnlySelected = false;
    eParticleDebugDrawMode drawMode = OVERDRAW;

    NMaterial* wireframeMaterial = nullptr;
    NMaterial* overdrawMaterial = nullptr;
    NMaterial* showAlphaMaterial = nullptr;

    NMaterial* quadMaterial = nullptr;
    NMaterial* quadHeatMaterial = nullptr;

    Texture* heatTexture = nullptr;

    Vector<NMaterial*> materials;
};

/**
 * @brief Gets the current particle effect debug draw mode.
 * @return Current debug draw mode for particle effects.
 */
eParticleDebugDrawMode ParticleEffectDebugDrawSystem::GetDrawMode() const
{
    return drawMode;
}

/**
 * @brief Sets the debug draw mode for particle effect visualization
 * @param[in] mode New debug draw mode to be applied
 */
void ParticleEffectDebugDrawSystem::SetDrawMode(eParticleDebugDrawMode mode)
{
    drawMode = mode;
}

/**
 * @brief Returns whether the debug drawing is limited to selected particle effects only.
 * @return True if only selected particle effects are being drawn, false if all effects are drawn.
 */
bool ParticleEffectDebugDrawSystem::GetIsDrawOnlySected() const
{
    return isDrawOnlySelected;
}

/**
 * @brief Controls whether to draw debug information only for selected particle effects.
 * @param enable If true, debug information will be shown only for selected particle effects.
 *               If false, debug information will be shown for all particle effects.
 */
void ParticleEffectDebugDrawSystem::SetIsDrawOnlySelected(bool enable)
{
    isDrawOnlySelected = enable;
}

/**
 * @brief Returns a constant pointer to the vector of materials used by the particle effect debug drawing system
 * @return Constant pointer to a constant vector of material pointers
 */
const Vector<NMaterial*>* const ParticleEffectDebugDrawSystem::GetMaterials() const
{
    return &materials;
}
}