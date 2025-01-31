#pragma once

#include "Scene3D/Systems/SlotSystem.h"

#include "Concurrency/Mutex.h"
#include "Base/RefPtr.h"
#include "Base/Hash.h"

namespace DAVA
{
class AsyncSlotExternalLoader final : public SlotSystem::ExternalEntityLoader
{
public:
    /**
     * @brief Loads an external slot asynchronously from a file
     * @param rootEntity The root entity where the loaded content will be attached
     * @param path The file path to load the slot from
     * @param finishCallback Callback function that will be called when loading is complete, receives a string parameter with loading results
     */
    void Load(RefPtr<Entity> rootEntity, const FilePath& path, const DAVA::Function<void(String&&)>& finishCallback) override;
    /**
     * @brief Processes the async loading tasks in a single frame
     * @details Called every frame to handle loading of external slots, manages loading queue and updates loading progress
     * @param delta Time elapsed since last frame in seconds
     */
    void Process(float32 delta) override;
    /**
     * @brief Resets the async slot external loader to its initial state
     * 
     * @details This method overrides the base class reset functionality to clear 
     * any ongoing loading operations and restore the loader to its default state.
     */
    void Reset() override;

    /**
     * @brief Implements asynchronous loading of entity from file
     * @param rootEntity Root entity where loaded data will be attached
     * @param path Path to the file that should be loaded
     * @details This internal implementation method handles the actual loading process
     *          of external data into the specified root entity
     */
    void LoadImpl(RefPtr<Entity> rootEntity, const FilePath& path);

private:
    /**
    * @brief Processes and applies the next job in the async loading queue.
    * This method handles the next pending job in the loading queue, applying
    * any necessary updates or loading operations. It should be called
    * sequentially to process jobs one at a time.
    */
    void ApplyNextJob();
    struct HashRefPtrEntity
    {
        size_t operator()(const RefPtr<Entity>& pointer) const;
    };

    struct LoadingResult
    {
        RefPtr<Scene> scene;
        String error;
        Function<void(String&&)> finishCallback;
    };
    UnorderedMap<RefPtr<Entity>, LoadingResult, HashRefPtrEntity> jobsMap;
    Mutex jobsMutex;

    Mutex queueMutes;
    struct LoadTask
    {
        RefPtr<Entity> rootEntity;
        FilePath filePath;
    };
    List<LoadTask> loadingQueue;
    bool isLoadingActive = false;
};

/**
 * @brief Hash functor for Entity reference pointers
 * 
 * Calculates a hash value for a RefPtr<Entity> by converting the raw pointer
 * address to a size_t value.
 * 
 * @param pointer Reference pointer to an Entity object to be hashed
 * @return size_t Hash value generated from the pointer address
 */
inline size_t AsyncSlotExternalLoader::HashRefPtrEntity::operator()(const RefPtr<Entity>& pointer) const
{
    return reinterpret_cast<size_t>(pointer.Get());
}

} // namespace DAVA
