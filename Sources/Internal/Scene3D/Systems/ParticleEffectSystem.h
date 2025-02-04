#pragma once

#include "Base/BaseTypes.h"
#include "Entity/SceneSystem.h"
#include "Scene3D/Components/ParticleEffectComponent.h"

namespace DAVA
{
class Component;
class ParticleForce;

class ParticleEffectSystem : public SceneSystem
{
    friend class ParticleEffectComponent;
    friend class UIParticles;

public:
    struct MaterialData
    {
        Texture* texture = nullptr;
        Texture* flowmap = nullptr;
        Texture* noise = nullptr;
        Texture* alphaRemapTexture = nullptr;
        eBlending blending = BLENDING_ALPHABLEND;
        uint64 layerId = 1;
        bool enableFog = false;
        bool enableFrameBlend = false;
        bool enableFlow = false;
        bool enableFlowAnimation = false;
        bool enableNoise = false;
        bool isNoiseAffectFlow = false;
        bool useFresnelToAlpha = false;
        bool enableAlphaRemap = false;
        bool usePerspectiveMapping = false;
        bool useThreePointGradient = false;

        /**
         * @brief Equality operator for MaterialData structure
         * @param rhs MaterialData instance to compare with
         * @return true if both MaterialData instances are equal, false otherwise
         * 
         * Compares all material properties including:
         * - Gradient configuration and layer ID (if three point gradient is used)
         * - Texture references
         * - Fog settings
         * - Frame blending settings
         * - Flow map settings and animation
         * - Noise settings and its interaction with flow
         * - Fresnel to alpha conversion
         * - Blending mode
         * - Alpha remapping settings
         * - Perspective mapping
         */
        bool operator==(const MaterialData& rhs)
        {
            bool isEqualByGradient = useThreePointGradient == rhs.useThreePointGradient;
            if (isEqualByGradient && useThreePointGradient)
                isEqualByGradient = layerId == rhs.layerId;

            return texture == rhs.texture
            && enableFog == rhs.enableFog
            && enableFrameBlend == rhs.enableFrameBlend
            && flowmap == rhs.flowmap
            && enableFlow == rhs.enableFlow
            && enableFlowAnimation == rhs.enableFlowAnimation
            && enableNoise == rhs.enableNoise
            && isNoiseAffectFlow == rhs.isNoiseAffectFlow
            && noise == rhs.noise
            && useFresnelToAlpha == rhs.useFresnelToAlpha
            && blending == rhs.blending
            && enableAlphaRemap == rhs.enableAlphaRemap
            && alphaRemapTexture == rhs.alphaRemapTexture
            && usePerspectiveMapping == rhs.usePerspectiveMapping
            && isEqualByGradient;
        }
    };

    /**
     * @brief Constructor for the ParticleEffectSystem class.
     * @details This system manages and processes particle effects within a scene,
     *          supporting both 2D and 3D particle effects.
     * 
     * @param scene Pointer to the Scene object this system will operate within
     * @param is2DMode Flag to determine if the system should operate in 2D mode (default: false)
     */
    ParticleEffectSystem(Scene* scene, bool is2DMode = false);

    /**
     * @brief Destructor for ParticleEffectSystem class.
     * Cleans up and releases resources associated with the particle effect system.
     */
    ~ParticleEffectSystem();
    /**
     * @brief Processes particle effects for a given time interval
     * @param timeElapsed The elapsed time in seconds since the last frame
     */
    void Process(float32 timeElapsed) override;
    /**
     * @brief Handles immediate events for a component in the particle effect system
     * @param component Pointer to the component that triggered the event
     * @param event Unsigned 32-bit integer representing the event type to be processed
     */
    void ImmediateEvent(Component* component, uint32 event) override;

    /**
     * @brief Adds an entity to the particle effect system for processing
     * @param entity Pointer to the entity that needs to be processed by the particle system
     * @details This entity should contain necessary particle effect components to be processed
     * @note Overrides the base system's AddEntity method
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Adds a particle effect component to the specified entity
     * @param entity The entity to which the component will be added
     * @param component The component to be added (must be a valid particle effect component)
     * @note This is an override of the base system's AddComponent method
     */
    void AddComponent(Entity* entity, Component* component) override;

    /**
     * @brief Removes an entity from the particle effect system
     * @param entity Pointer to the entity to be removed from the system
     * @details This method handles the cleanup and removal of particle effect components 
     *          associated with the given entity from the system
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Removes the specified component from the given entity
     * @param entity Pointer to the entity from which to remove the component
     * @param component Pointer to the component that should be removed
     * @override Overrides base class method
     */
    void RemoveComponent(Entity* entity, Component* component) override;
    /**
     * @brief Prepares the system for removal from the scene
     * @details This method is called before the system is removed from the scene to perform necessary cleanup operations
     * @override Overrides the base class method
     */
    void PrepareForRemove() override;

    /**
     * @brief Set global material for all particles
     * @param material Material to use
     */
    void SetGlobalMaterial(NMaterial* material);

    /**
     * @brief Set global external parameter value
     * @param name Parameter name
     * @param value Parameter value
     */
    void SetGlobalExtertnalValue(const String& name, float32 value);

    /**
     * @brief Get value of global external parameter
     * @param name Parameter name
     * @return Parameter value
     */
    float32 GetGlobalExternalValue(const String& name);

    /**
     * @brief Get all global external parameters
     * @return Map of parameter names to values
     */
    Map<String, float32> GetGlobalExternals();

    /**
     * @brief Sets whether LOD degradation is allowed for particle effects
     * @param[in] allowDegrade If true, allows LOD degradation for particle effects; if false, disables it
     * 
     * Controls whether the particle effect system can reduce the level of detail (LOD)
     * for particle effects to improve performance when needed.
     */
    inline void SetAllowLodDegrade(bool allowDegrade);
    /**
     * @brief Gets whether LOD degradation is allowed for particle effects
     * @return true if LOD degradation is allowed, false otherwise
     */
    inline bool GetAllowLodDegrade() const;

    /**
     * @brief Returns a reference to the vector containing pairs of MaterialData and corresponding NMaterial instances
     * @return Const reference to vector of pairs containing MaterialData and corresponding NMaterial pointers
     */
    inline const Vector<std::pair<MaterialData, NMaterial*>>& GetMaterialInstances() const;

    /**
     * @brief Builds and prepares materials required for particle effect rendering.
     * @param[in] component Pointer to the particle effect component containing material data to prebuild.
     * @details This function processes and initializes materials for the particle effect component
     *          before rendering to ensure optimal performance during runtime.
     */
    void PrebuildMaterials(ParticleEffectComponent* component);

protected:
    /**
     * @brief Run particle effect
     * @param effect Effect component to run
     */
    void RunEffect(ParticleEffectComponent* effect);

    /**
     * @brief Add effect to active effects list
     * @param effect Effect to activate
     */
    void AddToActive(ParticleEffectComponent* effect);

    /**
     * @brief Remove effect from active effects list
     * @param effect Effect to deactivate
     */
    void RemoveFromActive(ParticleEffectComponent* effect);

    /**
     * @brief Updates the active Level of Detail (LOD) for a particle effect component
     * @param[in] effect Pointer to the particle effect component whose LOD needs to be updated
     * @details This function evaluates and updates the appropriate LOD level for the particle effect
     * based on current viewing conditions (like distance from camera, performance settings, etc.)
     */
    void UpdateActiveLod(ParticleEffectComponent* effect);
    /**
     * Updates the particle effect state.
     * @param effect The particle effect component to update
     * @param deltaTime Time elapsed since the last update in seconds
     * @param shortEffectTime Time threshold for short-living effects optimization
     */
    void UpdateEffect(ParticleEffectComponent* effect, float32 deltaTime, float32 shortEffectTime);
    /**
     * @brief Generates a new particle in the specified particle effect group
     * @param effect Pointer to the particle effect component that owns the particle
     * @param group Reference to the particle group where the new particle will be created
     * @param currLoopTime Current time within the effect's loop cycle
     * @param worldTransform World transformation matrix for positioning the particle
     * @return Pointer to the newly generated particle, or nullptr if generation fails
     */
    Particle* GenerateNewParticle(ParticleEffectComponent* effect, ParticleGroup& group, float32 currLoopTime, const Matrix4& worldTransform);
    /**
     * @brief Updates the data for a regular particle within a particle effect system
     * @param effect Pointer to the particle effect component being updated
     * @param particle Pointer to the particle being updated
     * @param group The particle group containing parameters for the particle
     * @param overLife Current life progress of the particle (0.0 to 1.0)
     * @param simplifiedForcesCount Number of simplified forces affecting the particle
     * @param currSimplifiedForceValues Vector containing current values of simplified forces
     * @param dt Delta time for the current update
     * @param bbox Reference to the bounding box to be updated with particle position
     * @param effectAlignForces Vector of forces aligned to the effect's coordinate system
     * @param effectAlignForcesCount Number of effect-aligned forces
     * @param worldAlignForces Vector of forces aligned to the world coordinate system
     * @param worldAlignForcesCount Number of world-aligned forces
     * @param world World transformation matrix
     * @param invWorld Inverse world transformation matrix
     * @param layerOverLife Current life progress of the particle layer (0.0 to 1.0)
     */
    void UpdateRegularParticleData(ParticleEffectComponent* effect, Particle* particle, const ParticleGroup& group, float32 overLife, int32 simplifiedForcesCount, Vector<Vector3>& currSimplifiedForceValues, float32 dt, AABBox3& bbox, const Vector<ParticleForce*>& effectAlignForces, uint32 effectAlignForcesCount, const Vector<ParticleForce*>& worldAlignForces, uint32 worldAlignForcesCount, const Matrix4& world, const Matrix4& invWorld, float32 layerOverLife);

    /**
     * @brief Prepares parameters for a particle emitter based on given particle, group, and transformation.
     * 
     * @param particle Pointer to the particle instance that needs parameter preparation
     * @param group Reference to the particle group containing emission settings and properties
     * @param worldTransform Matrix representing the world transformation for particle positioning
     * 
     * @details This function sets up necessary parameters for particle emission, including
     * position, orientation, and other emission-related properties based on the provided
     * world transformation and group settings.
     */
    void PrepareEmitterParameters(Particle* particle, ParticleGroup& group, const Matrix4& worldTransform);
    /**
     * @brief Extends the given bounding box to include a particle's sphere volume
     * @param[in] position The center position of the particle in 3D space
     * @param[in] radius The radius of the particle
     * @param[in,out] bbox The bounding box to be extended to include the particle
     */
    void AddParticleToBBox(const Vector3& position, float radius, AABBox3& bbox);

    /**
     * @brief Initiates and runs a particle emitter within a particle effect at a specified position
     * 
     * @param effect The particle effect component that owns the emitter
     * @param emitter The particle emitter to be run
     * @param spawnPosition The 3D position where particles will be spawned
     * @param positionSource Source index for position calculation (default = 0)
     */
    void RunEmitter(ParticleEffectComponent* effect, ParticleEmitter* emitter, const Vector3& spawnPosition, int32 positionSource = 0);

private:
    /**
     * @brief Applies global forces to a particle during its lifetime
     * @param particle Pointer to the particle to apply forces to
     * @param dt Time delta since last update
     * @param overLife Current normalized lifetime of the particle (0.0 to 1.0)
     * @param layerOverLife Current normalized lifetime of the layer containing the particle (0.0 to 1.0)
     * @param prevParticlePosition Previous position of the particle in world space
     */
    void ApplyGlobalForces(Particle* particle, float32 dt, float32 overLife, float32 layerOverLife, Vector3 prevParticlePosition);
    /**
     * @brief Updates a particle stripe with the given parameters
     * @param[in,out] particle Pointer to the particle to be updated
     * @param[in] effectData Effect data containing particle system parameters
     * @param[in,out] group Particle group the particle belongs to
     * @param[in] dt Delta time for the update
     * @param[in,out] bbox Bounding box that will be updated with the particle's position
     * @param[in] currForceValues Vector of force values affecting the particle
     * @param[in] forcesCount Number of forces in the currForceValues array
     * @param[in] isActive Flag indicating if the particle system is active
     */
    void UpdateStripe(Particle* particle, ParticleEffectData& effectData, ParticleGroup& group, float32 dt, AABBox3& bbox, const Vector<Vector3>& currForceValues, int32 forcesCount, bool isActive);
    /**
     * @brief Simulates a single frame of the particle effect.
     * 
     * @param effect Pointer to the ParticleEffectComponent to simulate.
     *               The effect must be valid and properly initialized.
     * 
     * @details This function updates the particle effect's state for one simulation step,
     *          including particle emission, movement, and lifecycle management.
     */
    void SimulateEffect(ParticleEffectComponent* effect);
    /**
     * @brief Calculates and fills the outer and inner radiuses for a particle group's emitter
     * @param[in] group The particle group whose emitter radiuses need to be calculated
     * @param[out] radius The outer radius of the emitter to be filled
     * @param[out] innerRadius The inner radius of the emitter to be filled
     */
    void FillEmitterRadiuses(const ParticleGroup& group, float32& radius, float32& innerRadius);

    Map<String, float32> globalExternalValues;
    Vector<ParticleEffectComponent*> activeComponents;

    struct EffectGlobalForcesData
    {
        Vector<ParticleForce*> worldAlignForces;
        Vector<ParticleForce*> effectAlignForces;
    };
    /**
     * @brief Removes forces associated with the given particle effect from the global force registry
     * @param[in] effect Pointer to the particle effect component whose forces need to be removed
     * @details This method unregisters and removes all forces that were previously added by the specified
     *          particle effect component from the global force management system
     */
    void RemoveForcesFromGlobal(ParticleEffectComponent* effect);
    /**
     * @brief Extracts and processes global forces affecting a particle effect component.
     * 
     * @details This function analyzes and extracts global force parameters (such as gravity, wind, etc.)
     * that influence the behavior of particles in the specified effect component.
     * 
     * @param[in] effect Pointer to the particle effect component to process
     */
    void ExtractGlobalForces(ParticleEffectComponent* effect);

private: //materials stuff
    NMaterial* particleBaseMaterial;
    Vector<std::pair<MaterialData, NMaterial*>> particlesMaterials;
    Map<ParticleEffectComponent*, EffectGlobalForcesData> globalForces;
    NMaterial* AcquireMaterial(const MaterialData& materialData);

    bool allowLodDegrade;
    bool is2DMode;
};

/**
 * @brief Returns a constant reference to the vector of particle effect material instances.
 * 
 * @return const Vector<std::pair<ParticleEffectSystem::MaterialData, NMaterial*>>& Vector containing pairs of
 *         MaterialData and corresponding NMaterial pointers representing particle effect materials.
 */
inline const Vector<std::pair<ParticleEffectSystem::MaterialData, NMaterial*>>& ParticleEffectSystem::GetMaterialInstances() const
{
    return particlesMaterials;
}

inline void ParticleEffectSystem::SetAllowLodDegrade(bool allowDegrade)
{
    allowLodDegrade = allowDegrade;
}

/**
 * @brief Returns whether LOD degradation is allowed for particle effects
 * @return true if LOD degradation is allowed, false otherwise
 */
inline bool ParticleEffectSystem::GetAllowLodDegrade() const
{
    return allowLodDegrade;
}
};