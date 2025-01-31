#pragma once

#include "Entity/Private/FamilyRepository.h"

namespace DAVA
{
class Component;
class Type;

class EntityFamily
{
private:
    /**
     * @brief Constructs an EntityFamily with the given components
     * @param components Vector of Component pointers to initialize the family with
     */
    EntityFamily(const Vector<Component*>& components);

    /**
     * @brief Copy constructor
     * @param other EntityFamily instance to copy from
     */
    EntityFamily(const EntityFamily& other);

public:
    /**
     * @brief Gets existing EntityFamily or creates new one for given components
     * @param components Vector of Component pointers to get/create family for
     * @return Pointer to EntityFamily instance
     */
    static EntityFamily* GetOrCreate(const Vector<Component*>& components);

    /**
     * @brief Releases the EntityFamily instance
     * @param family Reference to EntityFamily pointer to release
     */
    static void Release(EntityFamily*& family);

    /**
     * @brief Gets the component index for specified type and index
     * @param type Pointer to Type instance
     * @param index Component index to look up
     * @return Component index value
     */
    uint32 GetComponentIndex(const Type* type, uint32 index) const;

    /**
     * @brief Gets the count of components for specified type
     * @param type Pointer to Type instance
     * @return Number of components of specified type
     */
    uint32 GetComponentsCount(const Type* type) const;

    /**
     * @brief Gets the component mask for this family
     * @return Reference to ComponentMask
     */
    const ComponentMask& GetComponentsMask() const;

    /**
     * @brief Equality comparison operator
     * @param rhs Right-hand side EntityFamily to compare with
     * @return true if families are equal, false otherwise
     */
    bool operator==(const EntityFamily& rhs) const;

private:
    Vector<uint32> componentsIndices;
    Vector<uint32> componentsCount;
    ComponentMask componentsMask;
    Atomic<int32> refCount;

    template <typename EntityFamilyType>
    friend class FamilyRepository;

    static FamilyRepository<EntityFamily> repository;
};
}
