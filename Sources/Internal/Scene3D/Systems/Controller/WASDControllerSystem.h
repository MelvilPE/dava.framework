#pragma once

#include "Entity/SceneSystem.h"

namespace DAVA
{
class Camera;
class UIEvent;
class InputCallback;

class WASDControllerSystem : public SceneSystem
{
    enum eDirection
    {
        DIRECTION_STRAIGHT = 1,
        DIRECTION_INVERSE = -1
    };

public:
    /**
     * @brief Constructor for the WASD Controller System that handles keyboard-based movement control
     * @param scene Pointer to the Scene object this controller system will operate within
     * @details This system implements WASD-style movement controls commonly used in games and 3D applications
     *          where W moves forward, A strafes left, S moves backward, and D strafes right
     */
    WASDControllerSystem(Scene* scene);
    /**
     * @brief Virtual destructor for WASDControllerSystem class.
     * 
     * Ensures proper cleanup when derived classes are deleted through a base class pointer.
     */
    virtual ~WASDControllerSystem();

    /**
     * @brief Adds an entity to the WASD controller system
     * @param entity The entity to be added to the WASD controller system. Should contain necessary components for WASD control.
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes an entity from the WASD controller system
     * @param entity The entity to be removed from the system
     * @details This method overrides the base system's RemoveEntity method to handle cleanup of WASD controller components
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal from the scene.
     * 
     * This method is called before the system is removed from the scene to perform any necessary cleanup operations.
     * Overrides the base class implementation to handle WASD controller specific cleanup.
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes the WASD controller system for the given time period
     * @param[in] timeElapsed Time passed since the last update in seconds
     */
    void Process(float32 timeElapsed) override;

    /**
     * @brief Gets the current movement speed of the controller
     * @return The movement speed as a float32 value
     */
    float32 GetMoveSpeeed() const;
    /**
     * @brief Sets the movement speed for the WASD controller
     * @param moveSpeed The speed at which the controller will move, measured in units per second
     */
    void SetMoveSpeed(float32 moveSpeed);

private:
    /**
     * @brief Moves the camera forward or backward based on the specified direction
     * @param camera Pointer to the camera object to be moved
     * @param speed Movement speed multiplier
     * @param direction Direction of movement (forward or backward)
     */
    void MoveForward(Camera* camera, float32 speed, eDirection direction);
    /**
     * @brief Moves the camera right along its local axis
     * @param camera Pointer to the camera to move
     * @param speed Movement speed value
     * @param direction Direction enum specifying movement direction/behavior
     */
    void MoveRight(Camera* camera, float32 speed, eDirection direction);

    float32 moveSpeed;

    Vector<Entity*> entities;
};

/**
 * @brief Returns the current movement speed of the WASD controller
 * @return Movement speed value as float32
 */
inline float32 WASDControllerSystem::GetMoveSpeeed() const
{
    return moveSpeed;
}

/**
 * @brief Sets the movement speed for the WASD controller.
 * @param[in] _moveSpeed New movement speed value to be set.
 */
inline void WASDControllerSystem::SetMoveSpeed(float32 _moveSpeed)
{
    moveSpeed = _moveSpeed;
}
};