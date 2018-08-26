/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/ObjectRegistry.h
 *
 * @brief Header file for the object registry (ObjectRegistry) class template.
 */

#ifndef KL_OBJECT_REGISTRY_H
#define KL_OBJECT_REGISTRY_H 1

#include "koala/Definitions.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/base_class.hpp"
#include "cereal/types/polymorphic.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

#include <atomic>

namespace kl
{
/**
 * @brief Forward declaration of the Koala class.
 */
class Koala;

/**
 * @brief Forward declaration of the RegisteredObjectTemplate class template.
 */
template <typename TBASE, typename TALIAS>
class RegisteredObjectTemplate;

/**
 * @brief Forward declaration of the HierarchicalObjectTemplate class template.
 */
template <typename TBASE, typename TALIAS>
class HierarchicalObjectTemplate;

/**
 * @brief Forward declaration of the HierarchicalVisualizationUtility class template.
 */
template <typename T>
class HierarchicalVisualizationUtility;

/**
 * @brief ObjectRegistryBase abstract class.
 */
class ObjectRegistryBase
{
public:
    /**
     * @brief Default constructor.
     */
    ObjectRegistryBase() = default;

    /**
     * @brief Default copy constructor.
     */
    ObjectRegistryBase(const ObjectRegistryBase &) = default;

    /**
     * @brief Default move constructor.
     */
    ObjectRegistryBase(ObjectRegistryBase &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    ObjectRegistryBase &operator=(const ObjectRegistryBase &) = default;

    /**
     * @brief Default move assignment operator.
     */
    ObjectRegistryBase &operator=(ObjectRegistryBase &&) = default;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~ObjectRegistryBase() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the printable base object name.
     *
     * @return The printable base object name.
     */
    virtual auto PrintableBaseName() const noexcept -> std::string = 0;

protected:
    using sPtr = std::shared_ptr<ObjectRegistryBase>;  ///< Alias for a shared pointer.
    using sPtrSet = std::unordered_set<sPtr>;  ///< Alias for an unordered set of shared pointers.

    friend class Koala;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &);
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief ObjectRegistry class template.
 */
template <typename TBASE, typename TALIAS = std::string>
class ObjectRegistry : public ObjectRegistryBase,
                       public std::enable_shared_from_this<ObjectRegistry<TBASE, TALIAS>>
{
private:
    using TBASE_D = std::decay_t<TBASE>;    ///< Alias for decayed TBASE type.
    using TALIAS_D = std::decay_t<TALIAS>;  ///< Alias for decayed TALIAS type.

protected:
    using sPtr = std::shared_ptr<ObjectRegistry>;  ///< Alias for shared pointer to the registry.
    using wPtr = std::weak_ptr<ObjectRegistry>;    ///< Alias for weak pointer to the registry.
    using TBASE_sPtr = std::shared_ptr<TBASE_D>;   ///< Alias for shared pointer to decayed TBASE.
    using Koala_wPtr = std::weak_ptr<Koala>;       ///< Alias for weak pointer to Koala.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Constructor.
     *
     * @param wpKoala Weak pointer to the instance of Koala.
     * @param printableBaseName A print-worthy name for the base object type.
     */
    ObjectRegistry(Koala_wPtr wpKoala, std::string printableBaseName) noexcept;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a shared pointer to the object.
     *
     * @return A shared pointer to the object.
     */
    auto GetSharedPointer() noexcept;

    /**
     * @brief Get the registry mutex.
     *
     * @return The registry mutex.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(, Mutex, m_mutex, const);

    /**
     * @brief Set the koala weak pointer.
     *
     * @param wpKoala The koala weak pointer.
     */
    KL_SIMPLE_MOVE_SETTER_LOCK(KoalaWeakPointer, Koala_wPtr, wpKoala, m_wpKoala, WriteLock,
                               m_mutex);

    friend class Koala;
    friend TBASE;  ///< The template type.
    friend class HierarchicalObjectTemplate<TBASE_D, TALIAS_D>;
    friend class RegisteredObjectTemplate<TBASE_D, TALIAS_D>;

    template <typename T>
    friend class HierarchicalVisualizationUtility;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    using ObjectAliasToIdMap =
        std::map<TALIAS_D, ID_t>;  ///< Alias for map from object aliases to IDs.
    using ObjectIdToAliasMap =
        std::unordered_map<ID_t, TALIAS_D>;  ///< Alias for map from IDs to object aliases.
    using ObjectIdMap =
        std::unordered_map<ID_t, TBASE_sPtr>;  ///< Alias for object ID to shared pointer map.
    using ObjectTypeMultiMap =
        std::unordered_multimap<std::string,
                                TBASE_sPtr>;  ///< Alias for object types to shared pointer map.

    mutable kl::Mutex m_mutex;  ///< A mutex for locking this object during concurrent access.

    Koala_wPtr m_wpKoala;  ///< Weak pointer to the instance of Koala that owns this registry.
    std::string m_printableBaseName;      ///< A print-worthy name for the base-type object.
    mutable std::atomic<ID_t> m_idCount;  ///< A counter for assigning IDs to new objects.

    ObjectIdMap m_objectIdMap;  ///< The map from the object IDs to shared pointers.
    ObjectTypeMultiMap
        m_objectTypeMultiMap;  ///< A map from the object types to the shared pointers.
    ObjectAliasToIdMap m_objectAliasToIdMap;  ///< A map from the object aliases to IDs.
    ObjectIdToAliasMap m_objectIdToAliasMap;  ///< A map from the IDs to the object aliases.

    /**
     * @brief Get the instance of Koala.
     *
     * @return The instance of Koala.
     */
    auto &GetKoala() const;

    /**
     * @brief Add an already-created object to the registry while it is being constructed.
     *
     * @param objectAlias The alias by which to save the object.
     * @param spObject Shared pointer to the object.
     *
     * @return The object ID (which must be given to the object during construction).
     */
    template <typename TTHISALIAS, typename TOBJECT>
    auto AddSharedPointerByAlias(TTHISALIAS &&objectAlias,
                                 const std::shared_ptr<TOBJECT> &spObject);

    /**
     * @brief Create an object and save it by its alias.
     *
     * @param objectAlias The alias by which to save the object.
     * @param parameters The parameters with which to instantiate the object.
     *
     * @return Shared pointer to the object.
     */
    template <typename TOBJECT, typename TTHISALIAS, typename... TPARAMETERS>
    auto CreateSharedPointerByAlias(TTHISALIAS &&objectAlias, TPARAMETERS &&... parameters);

    /**
     * @brief Create an object.
     *
     * @param parameters The parameters with which to instantiate the object.
     *
     * @return Shared pointer to the object.
     */
    template <typename TOBJECT, typename... TPARAMETERS>
    auto CreateSharedPointer(TPARAMETERS &&... parameters) noexcept(
        std::is_nothrow_constructible<std::decay_t<TOBJECT>, ObjectRegistry::wPtr, const ID_t,
                                      Koala_wPtr, TPARAMETERS...>::value);

    /**
     * @brief Tag dispatcher for getting a shared pointer to an object by object or by alias.
     *
     * @param arg The object or the alias of the object .
     *
     * @return Shared pointer to the object.
     */
    template <typename T,
              typename = std::enable_if_t<std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                                          std::is_base_of<TBASE_D, std::decay_t<T>>::value>>
    auto GetSharedPointer(T &&arg) const;

    /**
     * @brief Get the shared pointer to the object with a given ID.
     *
     * @param objectId The ID of the object to get.
     *
     * @return Shared pointer to the object.
     */
    auto GetSharedPointer(const ID_t objectId) const;

    /**
     * @brief Delete the given shared pointer.
     *
     * @param spObject The shared pointer to the object.
     *
     * @return Success.
     */
    auto DeleteSharedPointer(TBASE_sPtr &&spObject) noexcept;

    /**
     * @brief Delete the object with a given name.
     *
     * @param objectAlias The alias of the object to delete.
     *
     * @return Success.
     */
    template <typename TTHISALIAS>
    auto DeleteImpl(TTHISALIAS &&objectAlias, std::true_type) noexcept;

    /**
     * @brief Delete the object given the object itself.
     *
     * @param object The object to delete.
     *
     * @return Success.
     */
    template <typename TOBJECT = TBASE_D>
    auto DeleteImpl(TOBJECT &&object, std::false_type) noexcept;

    /**
     * @brief Get the shared pointer to the object from a copy of the object.
     *
     * @param object The copy of the object.
     *
     * @return Shared pointer to the object.
     */
    template <typename TOBJECT = TBASE_D>
    auto GetSharedPointerImpl(TOBJECT &&object, std::false_type) const;

    /**
     * @brief Get the shared pointer to the object with a given alias.
     *
     * @param objectAlias The alias of the object to get.
     *
     * @return Shared pointer to the object.
     */
    template <typename TTHISALIAS>
    auto GetSharedPointerImpl(TTHISALIAS &&objectAlias, std::true_type) const;

    /**
     * @brief Find out whether an object exists in the registry from a copy of the object.
     *
     * @param object The copy of the object.
     *
     * @return Whether the object exists in the registry.
     */
    template <typename TDESIRED, typename TOBJECT = TBASE_D>
    auto DoesObjectExistImpl(TOBJECT &&object, std::false_type) const noexcept;

    /**
     * @brief Find out whether an object exists in the registry with a given alias.
     *
     * @param objectAlias The alias of the object.
     *
     * @return Whether the object exists in the registry.
     */
    template <typename TDESIRED, typename TTHISALIAS>
    auto DoesObjectExistImpl(TTHISALIAS &&objectAlias, std::true_type) const noexcept;

    /**
     * @brief Find out whether an object has an alias (implementation).
     *
     * @param objectId The object's ID.
     *
     * @return Whether the object has an alias.
     */
    auto HasAliasImpl(const ID_t objectId) const noexcept;

    /**
     * @brief Add an alias to an object (implementation).
     *
     * @param objectId The object's ID.
     * @param alias The alias to add.
     */
    template <typename TTHISALIAS>
    void AddAliasImpl(const ID_t objectId, TTHISALIAS &&alias);

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);

    /**
     * @brief Default constructor (for serialization purposes).
     */
    ObjectRegistry() noexcept;

    /**
     * @brief Method template for loading and constructing serialized object.
     *
     * @param archive The cereal archive object.
     * @param construct The cereal construction object for this object.
     */
    template <typename TARCHIVE>
    static void load_and_construct(TARCHIVE &archive,
                                   cereal::construct<ObjectRegistry<TBASE, TALIAS>> &construct);
#endif  // #ifdef KOALA_ENABLE_CEREAL

public:
    /**
     * @brief Deleted copy constructor.
     */
    ObjectRegistry(const ObjectRegistry &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    ObjectRegistry(ObjectRegistry &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    ObjectRegistry &operator=(const ObjectRegistry &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    ObjectRegistry &operator=(ObjectRegistry &&) = delete;

    /**
     * @brief Default destructor.
     */
    ~ObjectRegistry() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the printable base object name.
     *
     * @return The printable base object name.
     */
    auto PrintableBaseName() const noexcept -> std::string override;

    /**
     * @brief Create an object.
     *
     * @param parameters The parameters with which to instantiate the object.
     *
     * @return The new object.
     */
    template <typename TOBJECT = TBASE_D, typename... TPARAMETERS>
    auto &Create(TPARAMETERS &&... parameters);

    /**
     * @brief Create an object and save it by its alias.
     *
     * @param objectAlias The alias by which to save the object.
     * @param parameters The parameters with which to instantiate the object.
     *
     * @return The new object.
     */
    template <typename TOBJECT = TBASE_D, typename TTHISALIAS, typename... TPARAMETERS>
    auto &CreateByAlias(TTHISALIAS &&objectAlias, TPARAMETERS &&... parameters);

    /**
     * @brief Get the object with a given alias.
     *
     * @param objectAlias The alias of the object to get.
     *
     * @return The retrieved object.
     */
    template <typename TOBJECT = TBASE_D, typename TTHISALIAS,
              typename = std::enable_if_t<
                  std::is_same<std::decay_t<TALIAS>, std::decay_t<TTHISALIAS>>::value>>
    auto &Get(TTHISALIAS &&objectAlias) const;

    /**
     * @brief Get the object from a copy of the object.
     *
     * @param object The copy of the object.
     *
     * @return The retrieved object.
     */
    template <typename TOBJECT = TBASE_D,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    auto &Get(TOBJECT &&object) const;

    /**
     * @brief Get the object with a given ID.
     *
     * @param objectId The ID of the object to get.
     *
     * @return The retrieved object.
     */
    template <typename TOBJECT = TBASE_D>
    auto &Get(const ID_t objectId) const;

    /**
     * @brief Get all the objects of a given type by value.
     *
     * @return The list of retrieved objects.
     */
    template <typename TOBJECT = TBASE_D>
    auto GetAllList() const noexcept;

    /**
     * @brief Get the range-based container for getting all objects.
     *
     * @return The range-based container for getting all objects.
     */
    template <typename TOBJECT = TBASE_D>
    auto GetAll() const;

    /**
     * @brief Get the number of objects of a given type.
     *
     * @return The number of objects of a given type.
     */
    template <typename TOBJECT = TBASE_D>
    auto Count() const noexcept;

    /**
     * @brief Get the total number of objects in the registry.
     *
     * @return The total number of objects in the registry.
     */
    auto CountAll() const noexcept;

    /**
     * @brief Tag dispatcher for deleting objects by object or by alias.
     *
     * @param arg The object or the alias of the object to delete.
     *
     * @return Success.
     */
    template <typename T,
              typename = std::enable_if_t<std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                                          std::is_base_of<TBASE_D, std::decay_t<T>>::value>>
    auto Delete(T &&arg) noexcept;

    /**
     * @brief Delete the object with a given ID.
     *
     * @param objectId The ID of the object to delete.
     *
     * @return Success.
     */
    auto Delete(const ID_t objectId) noexcept;

    /**
     * @brief Delete all the objects in the registry.
     */
    void DeleteAll() noexcept;

    /**
     * @brief Get the object alias, given the object.
     *
     * @param object The object whose alias to get.
     *
     * @return The alias of the object.
     */
    template <typename TOBJECT = TBASE_D,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    auto GetAlias(TOBJECT &&object) const;

    /**
     * @brief Get the object alias, given the object ID.
     *
     * @param objectId The ID of the object whose alias to get.
     *
     * @return The alias of the object.
     */
    auto GetAlias(const ID_t objectId) const;

    /**
     * @brief Get whether the object has an alias, given the object.
     *
     * @param object The object.
     *
     * @return Whether the object has an alias.
     */
    template <typename TOBJECT = TBASE_D,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    auto HasAlias(TOBJECT &&object) const noexcept;

    /**
     * @brief Get whether the object has an alias, given the object ID.
     *
     * @param objectId The object ID.
     *
     * @return Whether the object has an alias.
     */
    auto HasAlias(const ID_t objectId) const noexcept;

    /**
     * @brief Add an alias to an object, given the object.
     *
     * @param object The object.
     * @param alias The alias to add.
     */
    template <typename TOBJECT = TBASE_D, typename TTHISALIAS,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    void AddAlias(TOBJECT &&object, TTHISALIAS &&alias);

    /**
     * @brief Add an alias to an object, given the object ID.
     *
     * @param objectId The object ID.
     * @param alias The alias to add.
     */
    template <typename TTHISALIAS>
    void AddAlias(const ID_t objectId, TTHISALIAS &&alias);

    /**
     * @brief Find out whether an object exists in the registry.
     *
     * @param arg The alias or object.
     *
     * @return Whether the object exists in the registry.
     */
    template <typename TDESIRED, typename T,
              typename = std::enable_if_t<std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                                          std::is_base_of<TBASE_D, std::decay_t<T>>::value>>
    auto DoesObjectExist(T &&arg) const noexcept;

    /**
     * @brief Find out whether an object exists in the registry.
     *
     * @param objectId The ID of the object.
     *
     * @return Whether the object exists in the registry.
     */
    template <typename TDESIRED>
    auto DoesObjectExist(const ID_t objectId) const noexcept;
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Registry/ObjectRegistry.txx"

#endif  // #ifndef KL_OBJECT_REGISTRY_H
