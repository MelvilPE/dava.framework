#ifndef __DAVAENGINE_SCENE_NODE_ANIMATION_H__
#define __DAVAENGINE_SCENE_NODE_ANIMATION_H__

#include "Base/BaseTypes.h"
#include "Scene3D/Entity.h"
#include "Scene3D/SceneNodeAnimationKey.h"
namespace DAVA
{
/*
	TODO: efficient pack & unpack of animations (Vector / Quaternion spline approximation)
 */
class SceneNodeAnimationList;
class SceneNodeAnimation : public BaseObject
{
protected:
    ~SceneNodeAnimation() override;

public:
    /// Constructs a SceneNodeAnimation with the specified number of animation keys.
    /// @param keyCount The number of animation keys to allocate for this animation.
    SceneNodeAnimation(uint32 keyCount);

    /// Interpolates the animation keys and returns the resulting animation key.
    /// @param t The interpolation factor, between 0.0 and 1.0.
    /// @return The interpolated animation key.
    SceneNodeAnimationKey& Intepolate(float32 t);

    /// Sets the animation key at the specified index.
    /// @param index The index of the animation key to set.
    /// @param key The new animation key to set at the specified index.
    void SetKey(int32 index, const SceneNodeAnimationKey& key);

    /// Returns the number of animation keys in this SceneNodeAnimation.
    inline int32 GetKeyCount();
    /// Returns a pointer to the array of animation keys for this SceneNodeAnimation.
    inline SceneNodeAnimationKey* GetKeys();

    /// Sets the duration of the animation.
    /// @param _duration The new duration of the animation.
    void SetDuration(float32 _duration);
    /// Returns the duration of the animation.
    /// @return The duration of the animation.
    inline float32 GetDuration();

    /// Sets the name of the bind node for this animation.
    /// @param bindName The new name of the bind node.
    void SetBindName(const FastName& bindName);
    /// Sets the node of animation this animation is supposed for.
    /// @param bindNode The new node to bind this animation to.
    void SetBindNode(Entity* bindNode);

    /// Sets the inverse pose matrix for this SceneNodeAnimation.
    /// @param mat The new inverse pose matrix.
    void SetInvPose(const Matrix4& mat);
    /// Returns the inverse pose matrix for this SceneNodeAnimation.
    /// @return The inverse pose matrix.
    const Matrix4& GetInvPose() const;

    /// Updates the animation over the specified time elapsed.
    /// @param timeElapsed The amount of time elapsed since the last update.
    virtual void Update(float32 timeElapsed);
    /// Executes the animation update.
    virtual void Execute();

    /// Returns the current time of the animation.
    /// @return The current time of the animation.
    inline float32 GetCurrentTime();
    /// Returns the normal duration of the animation, which is the duration of the last animation key.
    /// @return The normal duration of the animation.
    inline float32 GetNormalDuration();

    /**
     * @brief Sets the start position of the scene node animation.
     *
     * This function sets the initial position of the scene node animation to the specified vector.
     *
     * @param position The new start position as a Vector3.
     * @return The updated start position as a Vector3.
     */
    Vector3 SetStartPosition(const Vector3& position);
    /**
     * Shifts the starting position of the node animation by the specified offset.
     * @param position Vector3 containing the offset values to shift the starting position
     */
    void ShiftStartPosition(const Vector3& position);

    /**
     * @brief Sets the parent animation list for this animation
     * @param[in] list Pointer to the parent SceneNodeAnimationList to set
     */
    void SetParent(SceneNodeAnimationList* list);
    SceneNodeAnimationList* GetParent();

    // this is node of animation this animation is supposed for
    Entity* bindNode = nullptr;
    FastName bindName;
    float32 weight = 0.0f;
    float32 delayTime = 0.0f;
    float32 currentTime = 0.0f;
    float32 duration = 0.0f;
    int32 startIdx = 0;
    uint32 keyCount = 0;
    SceneNodeAnimationKey* keys = nullptr;
    SceneNodeAnimationKey currentValue; //-V730_NOINIT
    Matrix4 invPose;
    bool apply = true;

private:
    SceneNodeAnimationList* parent = nullptr;
};

inline float32 SceneNodeAnimation::GetCurrentTime()
{
    return currentTime;
}

inline float32 SceneNodeAnimation::GetDuration()
{
    return duration;
}

inline float32 SceneNodeAnimation::GetNormalDuration()
{
    if (keyCount == 0)
        return 0;

    return keys[keyCount - 1].time;
}

inline int32 SceneNodeAnimation::GetKeyCount()
{
    return keyCount;
}

inline SceneNodeAnimationKey* SceneNodeAnimation::GetKeys()
{
    return keys;
}
};

#endif // __DAVAENGINE_SCENE_NODE_ANIMATION_KEY_H__
