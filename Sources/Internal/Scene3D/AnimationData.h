#ifndef __DAVAENGINE_ANIMATION_DATA_H__
#define __DAVAENGINE_ANIMATION_DATA_H__

#include "Base/BaseTypes.h"
#include "Scene3D/Entity.h"
#include "Scene3D/SceneNodeAnimationKey.h"
#include "Scene3D/DataNode.h"
namespace DAVA
{
// ...existing code...
class AnimationData : public DataNode
{
protected:
    /**
     * @brief Virtual destructor
     */
    virtual ~AnimationData();

public:
    /**
     * @brief Default constructor
     */
    AnimationData();

    /**
     * @brief Interpolate animation at given time
     * @param t Time to interpolate at
     * @param startIdxCache Cache for start index
     * @return Interpolated animation key
     */
    SceneNodeAnimationKey Interpolate(float32 t, uint32& startIdxCache) const;

    /**
     * @brief Get animation key for specific frame
     * @param frameIndex Frame index to get key for
     * @return Animation key for frame
     */
    SceneNodeAnimationKey GetKeyForFrame(int32 frameIndex) const;

    /**
     * @brief Add new animation key
     * @param key Animation key to add
     */
    void AddKey(const SceneNodeAnimationKey& key);

    /**
     * @brief Get total number of keys
     * @return Number of animation keys
     */
    int32 GetKeyCount() const;

    /**
     * @brief Set animation duration
     * @param _duration Duration to set
     */
    void SetDuration(float32 _duration);

    /**
     * @brief Get animation duration
     * @return Animation duration
     */
    float32 GetDuration() const;

    /**
     * @brief Set inverse pose matrix
     * @param mat Matrix to set
     */
    void SetInvPose(const Matrix4& mat);

    /**
     * @brief Get inverse pose matrix
     * @return Reference to inverse pose matrix
     */
    const Matrix4& GetInvPose() const;

    /**
     * Saves the animation data to the specified archive
     * @param[in] archive Archive where the animation data will be stored
     * @param[in] serializationContext Context used during the serialization process
     */
    void Save(KeyedArchive* archive, SerializationContext* serializationContext) override;
    /**
     * @brief Loads animation data from a keyed archive
     * @param archive Archive containing serialized animation data
     * @param serializationContext Context for serialization operations
     */
    void Load(KeyedArchive* archive, SerializationContext* serializationContext) override;

    /**
     * @brief Creates and returns a deep copy of the current AnimationData object
     * @return Pointer to newly created AnimationData object that is an exact copy of current instance
     */
    AnimationData* Clone() const;

    /**
     * @brief Applies a transformation matrix to the animation data
     * 
     * This method transforms all the animation data (such as keyframes, positions, rotations)
     * by the given transformation matrix. The transformation is permanent and affects
     * all frames of the animation.
     * 
     * @param transform The 4x4 transformation matrix to be applied to the animation data
     */
    void BakeTransform(const Matrix4& transform);

public:
    Matrix4 invPose;
    DAVA::Vector<SceneNodeAnimationKey> keys;
    float32 duration;
};

/**
 * @brief Returns the total duration of the animation in seconds.
 * @returns Duration value as a float32 (in seconds).
 */
inline float32 AnimationData::GetDuration() const
{
    return duration;
}

/**
 * @brief Returns the total number of animation keys.
 * @return Number of keys in the animation data as 32-bit integer.
 */
inline int32 AnimationData::GetKeyCount() const
{
    return static_cast<int32>(keys.size());
}
};

#endif // __DAVAENGINE_ANIMATION_DATA_H__
