#ifndef __DAVAENGINE_SCENE3D_WAVESYSTEM_H__
#define __DAVAENGINE_SCENE3D_WAVESYSTEM_H__

#include "Base/BaseTypes.h"
#include "Base/BaseMath.h"
#include "Base/Observer.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{
class Entity;
class WaveComponent;
class WaveSystem : public SceneSystem, public Observer
{
    struct WaveInfo
    {
        /**
         * @brief Constructor for WaveInfo structure
         * @param component Pointer to WaveComponent to initialize with
         */
        WaveInfo(WaveComponent* component);

        WaveComponent* component;
        float32 maxRadius;
        float32 maxRadiusSq;
        Vector3 center;
        float32 currentWaveRadius;
    };

public:
    /**
     * @brief Constructor for WaveSystem
     * @param scene Pointer to Scene this system belongs to
     */
    WaveSystem(Scene* scene);

    /**
     * @brief Destructor for WaveSystem
     */
    virtual ~WaveSystem();

    /**
     * @brief Prepares the system for removal
     */
    void PrepareForRemove() override;

    /**
     * @brief Handles immediate events for components
     * @param component Pointer to Component triggering the event
     * @param event Event identifier
     */
    void ImmediateEvent(Component* component, uint32 event) override;

    /**
     * @brief Processes wave calculations for the given time
     * @param timeElapsed Time elapsed since last update
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Gets wave disturbance at specified position
     * @param inPosition Position to get wave disturbance for
     * @return Vector3 representing wave disturbance
     */
    Vector3 GetWaveDisturbance(const Vector3& inPosition) const;

    /**
     * @brief Handles events from observable objects
     * @param observable Pointer to Observable that triggered the event
     */
    void HandleEvent(Observable* observable) override;

protected:
    /**
     * @brief Clears all wave data
     */
    void ClearWaves();

    bool isWavesEnabled;
    bool isVegetationAnimationEnabled;

    Vector<WaveInfo*> waves;

    friend class WaveComponent;
};

} // ns

#endif /* __DAVAENGINE_SCENE3D_WINDSYSTEM_H__ */
