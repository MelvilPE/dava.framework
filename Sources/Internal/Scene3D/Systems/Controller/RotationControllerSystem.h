#pragma once

#include "Entity/SceneSystem.h"
#include "Math/Vector.h"

namespace DAVA
{
class Camera;
class UIEvent;
class RotationControllerSystem : public SceneSystem
{
    static const float32 maxViewAngle;

public:
    /**
     * @brief Constructor for the RotationControllerSystem that handles rotation-based object control in a 3D scene.
     * @param scene Pointer to the Scene object this system will operate within.
     * @details This system manages rotation-based transformations and controls for objects in the 3D scene.
     *          It is responsible for processing rotation updates and applying them to relevant scene entities.
     */
    RotationControllerSystem(Scene* scene);
    /**
     * @brief Destructor for the RotationControllerSystem class.
     * @details Cleans up resources and properly destroys the rotation controller system.
     */
    ~RotationControllerSystem() override;

    /**
     * @brief Adds an entity to the rotation controller system for processing
     * @param[in] entity Pointer to the entity that will be added to the system
     * @details Entity must have required components for rotation control to be processed by the system
     */
    void AddEntity(Entity* entity) override;
    /**
     * @brief Removes an entity from the rotation controller system
     * @param entity Pointer to the Entity to be removed from the system
     */
    void RemoveEntity(Entity* entity) override;
    /**
     * @brief Prepares the system for removal by cleaning up resources and state
     * 
     * This method is called before the system is removed from the scene to ensure proper cleanup.
     * Overrides the base class implementation.
     */
    void PrepareForRemove() override;

    /**
     * @brief Processes rotation controller updates for the specified time interval
     * @param timeElapsed Time elapsed since the last update in seconds
     * 
     * Updates the rotation of entities with rotation controllers based on the elapsed time.
     * This method is called every frame to apply continuous rotation transformations.
     */
    void Process(float32 timeElapsed) override;

    /**
     * Handles input events for rotation control
     * @param event The UI event to process
     * @return true if the event was handled, false otherwise
     */
    bool Input(UIEvent* event) override;

    /**
     * @brief Returns the current rotation speed value
     * @return Speed of rotation in radians per second
     */
    float32 GetRotationSpeeed() const;
    /**
     * @brief Sets the rotation speed for the controller
     * @param[in] rotateSpeed The speed of rotation in radians per second
     */
    void SetRotationSpeeed(float32 rotateSpeed);

    /**
     * @brief Gets the current rotation point in 3D space
     * @return A constant reference to a Vector3 representing the rotation point coordinates
     */
    const Vector3& GetRotationPoint() const;
    /**
     * @brief Sets the point around which rotation occurs
     * @param[in] point The 3D coordinates of the rotation point in world space
     */
    void SetRotationPoint(const Vector3& point);

    /**
     * Recalculates view angles for the specified camera
     * @param camera Pointer to the camera whose view angles need to be recalculated
     */
    void RecalcCameraViewAngles(Camera* camera);

private:
    /**
     * @brief Rotates the direction of the given camera.
     * 
     * @param camera Pointer to the Camera object whose direction will be rotated.
     *              Must not be nullptr.
     */
    void RotateDirection(Camera* camera);
    /**
     * @brief Rotates camera position around a pivot point based on current control settings.
     * 
     * @param[in] camera Pointer to the camera object whose position will be rotated
     * 
     * @details This function modifies the camera's position by rotating it around a defined pivot point,
     * taking into account the current rotation settings and control parameters.
     */
    void RotatePosition(Camera* camera);
    /**
     * @brief Rotates camera position around a specified point in 3D space.
     * 
     * @param camera Pointer to the camera object to be rotated
     * @param pos The point in 3D space around which the camera will rotate (pivot point)
     * 
     * This function adjusts the camera's position by rotating it around a fixed point
     * while maintaining its current orientation. The rotation is applied to the camera's
     * position relative to the specified pivot point.
     */
    void RotatePositionAroundPoint(Camera* camera, const Vector3& pos);

    Vector3 rotationPoint;

    Vector2 rotateStartPoint;
    Vector2 rotateStopPoint;

    float32 curViewAngleZ;
    float32 curViewAngleY;

    float32 rotationSpeed;

    Vector<Entity*> entities;

    Camera* oldCamera;
};

/**
 * @brief Returns the current rotation speed
 * @return Current rotation speed value in radians per second
 */
inline float32 RotationControllerSystem::GetRotationSpeeed() const
{
    return rotationSpeed;
}

inline void RotationControllerSystem::SetRotationSpeeed(float32 rotateSpeed)
{
    rotationSpeed = rotateSpeed;
}

inline const Vector3& RotationControllerSystem::GetRotationPoint() const
{
    return rotationPoint;
}
inline void RotationControllerSystem::SetRotationPoint(const Vector3& point)
{
    rotationPoint = point;
}
};
