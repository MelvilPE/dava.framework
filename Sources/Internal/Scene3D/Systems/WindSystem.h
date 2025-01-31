#ifndef __DAVAENGINE_SCENE3D_WINDSYSTEM_H__
#define __DAVAENGINE_SCENE3D_WINDSYSTEM_H__

#include "Base/BaseTypes.h"
#include "Base/Observer.h"
#include "Entity/SceneSystem.h"

namespace DAVA
{

#define WIND_TABLE_SIZE 63

class Entity;
class WindComponent;
class WindSystem : public SceneSystem, public Observer
{
    struct WindInfo
    {
        /**
         * @brief Constructor for WindInfo structure
         * @param c Pointer to WindComponent to initialize with
         */
        WindInfo(WindComponent* c);

        WindComponent* component;
        float32 timeValue;
    };

public:
    /**
     * @brief Constructor for WindSystem
     * @param scene Pointer to Scene this system belongs to
     */
    WindSystem(Scene* scene);

    /**
     * @brief Destructor for WindSystem
     */
    ~WindSystem() override;

    /**
     * @brief Adds an entity to the wind system
     * @param entity Pointer to Entity to add
     */
    void AddEntity(Entity* entity) override;

    /**
     * @brief Removes an entity from the wind system
     * @param entity Pointer to Entity to remove
     */
    void RemoveEntity(Entity* entity) override;

    /**
     * @brief Prepares the system for removal
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes wind calculations for the given time
     * @param timeElapsed Time elapsed since last update
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Gets wind vector at specified position
     * @param inPosition Position to get wind vector for
     * @return Vector3 representing wind direction and magnitude
     */
    Vector3 GetWind(const Vector3& inPosition) const;

    /**
     * @brief Handles events from observable objects
     * @param observable Pointer to Observable that triggered the event
     */
    void HandleEvent(Observable* observable) override;

protected:
    /**
     * @brief Gets wind value from lookup table for given position
     * @param inPosition Position to get wind value for
     * @param info Pointer to WindInfo structure
     * @return Wind value at specified position
     */
    float32 GetWindValueFromTable(const Vector3& inPosition, const WindInfo* info) const;

    Vector<WindInfo*> winds;
    bool isAnimationEnabled;
    bool isVegetationAnimationEnabled;

    float32 windValuesTable[WIND_TABLE_SIZE];
};

} // ns

#endif /* __DAVAENGINE_SCENE3D_WINDSYSTEM_H__ */
