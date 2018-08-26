/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Templates/RegisteredObjectTemplate.h
 *
 * @brief Header file for the registered object template (RegisteredObjectTemplate) abstract class
 * template.
 */

#ifndef KL_REGISTERED_OBJECT_TEMPLATE_H
#define KL_REGISTERED_OBJECT_TEMPLATE_H

#include "koala/Definitions.h"
#include "koala/Registry/ObjectAssociation.h"
#include "koala/Registry/ObjectRegistry.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief Forward declaration of the Koala class.
 */
class Koala;

/**
 * @brief RegisteredObjectTemplate abstract class template.
 */
template <typename TBASE, typename TALIAS = std::string>
class RegisteredObjectTemplate
{
private:
    using TBASE_D = std::decay_t<TBASE>;    ///< Alias for decayed TBASE type.
    using TALIAS_D = std::decay_t<TALIAS>;  ///< Alias for decayed TALIAS type.

public:
    using KoalaBaseType = TBASE_D;                       ///< Alias for the base type.
    using KoalaAliasType = TALIAS_D;                     ///< Alias for the alias type.
    using Registry = ObjectRegistry<TBASE_D, TALIAS_D>;  ///< Alias for registry class.
    using RegisteredObject =
        RegisteredObjectTemplate<TBASE, TALIAS>;  ///< Alias for the RegisteredObject base class.

protected:
    using TBASE_sPtr = std::shared_ptr<TBASE_D>;  ///< Alias for a shared pointer to the base type.
    using TBASE_sPtr_const =
        std::shared_ptr<const TBASE_D>;  ///< Alias for a shared pointer to the const base type.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Functor for organizing a set by IDs.
     */
    template <typename TOBJECT>
    struct IdSetComparisonFunctor
    {
        /**
         * @brief Return whether the ID of the left-hand-side is smaller than the ID of the
         * right-hand-side.
         *
         * @param lhs The left-hand-side object.
         * @param rhs The right-hand-side object.
         *
         * @return Whether the ID of the left-hand-side is smaller than the ID of the
         * right-hand-side.
         */
        auto operator()(const std::decay_t<TOBJECT> &lhs, const std::decay_t<TOBJECT> &rhs) const
            noexcept
        {
            return (lhs.ID() < rhs.ID());
        }
    };

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Functor for a hash function by ID.
     */
    template <typename TOBJECT>
    struct IdHashFunctor
    {
        /**
         * @brief Return the hash key for a given object.
         *
         * @param object The object.
         *
         * @return The hash key.
         */
        auto operator()(const std::decay_t<TOBJECT> &object) const noexcept { return object.ID(); }
    };

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Functor for comparing two objects by their IDs.
     */
    template <typename TOBJECT>
    struct IdCompareFunctor
    {
        /**
         * @brief Compare two object by their IDs.
         *
         * @param lhs The left-hand-side object.
         * @param rhs The right-hand-side object.
         *
         * @return Whether their IDs were the same.
         */
        auto operator()(const std::decay_t<TOBJECT> &lhs, const std::decay_t<TOBJECT> &rhs) const
            noexcept
        {
            return (lhs.ID() == rhs.ID());
        }
    };

    //----------------------------------------------------------------------------------------------

    using TypeInfoRef =
        std::reference_wrapper<const std::type_info>;  ///< Alias for reference
                                                       ///< wrapper of std::type_info.

    /**
     * @brief Functor for a hash function for reference-wrapped std::type_info objects.
     */
    struct TypeInfoHashFunctor
    {
        /**
         * @brief Return the hash key for a given reference-wrapped std::type_info object.
         *
         * @param typeInfoRef The reference-wrapped std::type_info object.
         *
         * @return The hash key.
         */
        std::size_t operator()(TypeInfoRef typeInfoRef) const
        {
            return typeInfoRef.get().hash_code();
        }
    };

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Functor for comparing two reference-wrapped std::type_info objects.
     */
    struct TypeInfoCompareFunctor
    {
        /**
         * @brief Compare two reference-wrapped std::type_info objects.
         *
         * @param lhs The left-hand-side reference-wrapped std::type_info object.
         * @param rhs The right-hand-side reference-wrapped std::type_info object.
         *
         * @return Whether their IDs were the same.
         */
        bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const { return (lhs.get() == rhs.get()); }
    };

    //----------------------------------------------------------------------------------------------

    template <typename TOBJECT>
    using RefWrapperTemplate =
        std::reference_wrapper<std::decay_t<TOBJECT>>;  ///< Alias for reference wrapper of the
                                                        ///< object.

    template <typename TOBJECT>
    using ConstRefWrapperTemplate =
        std::reference_wrapper<const std::decay_t<TOBJECT>>;  ///< Alias for const reference
                                                              ///< wrapper.

    template <typename TOBJECT>
    using UnorderedRefSetTemplate =
        std::unordered_set<std::reference_wrapper<std::decay_t<TOBJECT>>,
                           IdHashFunctor<std::decay_t<TOBJECT>>,
                           IdCompareFunctor<std::decay_t<TOBJECT>>>;  ///< Alias for unordered set
                                                                      ///< of reference wrappers.

    template <typename TOBJECT>
    using UnorderedConstRefSetTemplate =
        std::unordered_set<std::reference_wrapper<const std::decay_t<TOBJECT>>,
                           IdHashFunctor<const std::decay_t<TOBJECT>>,
                           IdCompareFunctor<const std::decay_t<TOBJECT>>>;  ///< Alias for unordered
                                                                            ///< set of const
                                                                            ///< reference wrappers.

    template <typename TOBJECT>
    using RefVectorTemplate =
        std::vector<std::reference_wrapper<std::decay_t<TOBJECT>>>;  ///< Alias for ref wrapper
                                                                     ///< vector of the object.

    template <typename TOBJECT>
    using ConstRefVectorTemplate =
        std::vector<std::reference_wrapper<const std::decay_t<TOBJECT>>>;  ///< Alias for const ref
                                                                           ///< wrapper vector.

    using RefWrapper = RefWrapperTemplate<TBASE_D>;  ///< Alias for reference wrapper.
    using ConstRefWrapper =
        ConstRefWrapperTemplate<TBASE_D>;  ///< Alias for const reference wrapper.
    using UnorderedRefSet =
        UnorderedRefSetTemplate<TBASE_D>;  ///< Alias for unordered set of reference wrappers.
    using UnorderedConstRefSet =
        UnorderedConstRefSetTemplate<TBASE_D>;     ///< Alias for unordered set of const reference
                                                   ///< wrappers.
    using RefVector = RefVectorTemplate<TBASE_D>;  ///< Alias for a vector of reference wrappers.
    using ConstRefVector =
        ConstRefVectorTemplate<TBASE_D>;  ///< Alias for a vector of const reference wrappers.

    using Koala_wPtr = std::weak_ptr<Koala>;        ///< Alias for weak pointer to Koala.
    using Registry_wPtr = std::weak_ptr<Registry>;  ///< Alias for weak pointer to the registry.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Constructor.
     *
     * @param wpRegistry Weak pointer to the associated registry.
     * @param id The unique ID for this object.
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    RegisteredObjectTemplate(Registry_wPtr wpRegistry, const ID_t id, Koala_wPtr wpKoala) noexcept;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Initialize the object (to be overridden).
     */
    virtual void Initialize();

    /**
     * @brief Get the instance of Koala
     *
     * @return An instance of Koala.
     */
    auto &GetKoala() const;

    /**
     * @brief Get a shared pointer to the object.
     *
     * @return A shared pointer to the object.
     */
    virtual TBASE_sPtr GetSharedPointer() = 0;

    /**
     * @brief Get a shared pointer to the object.
     *
     * @return A shared pointer to the object.
     */
    virtual TBASE_sPtr_const GetSharedPointer() const = 0;

    /**
     * @brief Get the is-cereal-serializable boolean.
     *
     * @return The is-cereal-serializable boolean.
     */
    static constexpr auto IsCerealSerializable() noexcept;

    /**
     * @brief Default constructor (for serialization purposes and for constructing base for
     * uninitialized Koala).
     */
    RegisteredObjectTemplate() noexcept;

    /**
     * @brief Set the koala weak pointer.
     *
     * @param wpKoala The koala weak pointer.
     */
    KL_SIMPLE_MOVE_SETTER_LOCK(KoalaWeakPointer, Koala_wPtr, wpKoala, m_wpKoala, WriteLock,
                               m_mutex);

    friend Registry;  ///< Alias for the object registry from the base class.
    friend class Koala;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    // using TypeInfoAssocMultiMap = std::unordered_multimap<TypeInfoRef,
    // ObjectAssociationBase::sPtr, TypeInfoHashFunctor,
    //                                                      TypeInfoCompareFunctor>; ///< Alias for
    //                                                      typeinfo-assoc multimap.
    using TypeInfoAssocMultiMap =
        std::unordered_multimap<std::string,
                                ObjectAssociationBase::sPtr>;  ///< Alias for type-association map.

    mutable Mutex m_mutex;  ///< A mutex for locking this object during concurrent access.

    ID_t m_id;                   ///< The unique ID for this object type.
    Registry_wPtr m_wpRegistry;  ///< Weak pointer to the associated registry.
    Koala_wPtr m_wpKoala;        ///< Weak pointer to the instance of Koala.

    ObjectAssociationBase::sPtrSet
        m_serializableAssociations;  ///< The list of serializable associations to other objects.
    ObjectAssociationBase::sPtrSet
        m_unserializableAssociations;  ///< The list of unserializable associations to other
                                       ///< objects.
    TypeInfoAssocMultiMap m_serializableAssocMultiMap;    ///< Multimap from the typeinfo to the
                                                          ///< serializable associations.
    TypeInfoAssocMultiMap m_unserializableAssocMultiMap;  ///< Multimap from the typeinfo to the
                                                          ///< unserializable associations.

    /**
     * @brief Test the suitability of an object to form an association with and throw an error if
     * unsuitable.
     *
     * @param object The object to test.
     */
    template <typename TOBJECT>
    void TestAssociationObjectSuitability(TOBJECT &&object) const;

    /**
     * @brief Check whether an object type is in an assoc multimap.
     *
     * @param assocMultiMap The map.
     *
     * @return Whether the object type was found.
     */
    template <typename TOBJECT>
    auto IsObjectInAssocMultiMap(const TypeInfoAssocMultiMap &assocMultiMap) const noexcept;

    /**
     * @brief Check whether an object type is in an assoc multimap with a given indicator.
     *
     * @param assocMultiMap The map.
     * @param indicator The indicator.
     *
     * @return Whether the object type was found.
     */
    template <typename TOBJECT, typename TINDICATOR>
    auto IsObjectInAssocMultiMap(const TypeInfoAssocMultiMap &assocMultiMap,
                                 TINDICATOR &&indicator) const noexcept;

    /**
     * @brief Add a new association to the correct list and map.
     *
     * @param spAssociationBase The association to add.
     *
     * @return Success.
     */
    template <typename TOBJECT>
    auto AddAssociation(ObjectAssociationBase::sPtr spAssociationBase) noexcept;

    /**
     * @brief Remove an association to the correct list and map.
     *
     * @param object The object whose association to remove.
     *
     * @return Success.
     */
    template <typename TOBJECT>
    auto RemoveAssociation(TOBJECT &&object) noexcept;

    /**
     * @brief Remove an association with a given indicator to the correct list and map.
     *
     * @param object The object whose association to remove.
     * @param indicator The indicator.
     *
     * @return Success.
     */
    template <typename TOBJECT, typename TINDICATOR>
    auto RemoveAssociation(TOBJECT &&object, TINDICATOR &&indicator) noexcept;

    /**
     * @brief Get the associated objects from a map.
     *
     * @param assocMultiMap The map to use.
     *
     * @return The associated objects.
     */
    template <typename TOBJECT>
    auto GetAssociatedObjectsFromMap(const TypeInfoAssocMultiMap &assocMultiMap) const noexcept;

    /**
     * @brief Get the associated objects with a given indicator from a map.
     *
     * @param assocMultiMap The map to use.
     * @param indicator The indicator.
     *
     * @return The associated objects.
     */
    template <typename TOBJECT, typename TINDICATOR>
    auto GetAssociatedObjectsFromMap(const TypeInfoAssocMultiMap &assocMultiMap,
                                     TINDICATOR &&indicator) const noexcept;

    /**
     * @brief Get an associated object from a map.
     *
     * @param assocMultiMap The map.
     *
     * @return A shared pointer to the associated object.
     */
    template <typename TOBJECT>
    auto GetAssociatedObjectFromMap(const TypeInfoAssocMultiMap &assocMultiMap) const;

    /**
     * @brief Get an associated object with a given indicator from a map.
     *
     * @param assocMultiMap The map.
     * @param indicator The indicator.
     *
     * @return A shared pointer to the associated object.
     */
    template <typename TOBJECT, typename TINDICATOR>
    auto GetAssociatedObjectFromMap(const TypeInfoAssocMultiMap &assocMultiMap,
                                    TINDICATOR &&indicator) const;

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for saving object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void save(TARCHIVE &archive) const;

    /**
     * @brief Method template for loading object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void load(TARCHIVE &archive);
#endif  // #ifdef KOALA_ENABLE_CEREAL

public:
    /**
     * @brief Copy constructor.
     *
     * @param other The object from which to copy-construct.
     */
    RegisteredObjectTemplate(const RegisteredObjectTemplate &other) noexcept;

    /**
     * @brief Move constructor.
     *
     * @param other The object from which to move-construct.
     */
    RegisteredObjectTemplate(RegisteredObjectTemplate &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The object from which to copy-assign.
     */
    auto &operator=(const RegisteredObjectTemplate &other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The object from which to move-assign.
     */
    auto &operator=(RegisteredObjectTemplate &&other) noexcept;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~RegisteredObjectTemplate() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the unique ID.
     *
     * @return The unique ID.
     */
    KL_SIMPLE_GETTER(, ID, m_id, const);

    /**
     * @brief Get the registry object.
     *
     * @return The registry object.
     */
    auto &GetRegistry() const;

    /**
     * @brief Get the object's alias.
     *
     * @return The object's alias.
     */
    KL_SIMPLE_GETTER(, Alias, this->GetRegistry().GetAlias(this->ID()), const);

    /**
     * @brief Get whether the object has an alias.
     *
     * @return Whether the object has an alias.
     */
    KL_SIMPLE_GETTER(, HasAlias, this->GetRegistry().HasAlias(this->ID()), const);

    /**
     * @brief Get a printable name for the object.
     *
     * @return A printable name for the object.
     */
    virtual auto PrintableName() const -> std::string = 0;

    /**
     * @brief Get information about this object's associations.
     *
     * @return A set of AssociationInformation objects.
     */
    auto GetAssociationInformation() const;

    /**
     * @brief A method for getting a string identifying the object.
     *
     * @return A string identifying the object.
     */
    virtual auto GetIdentifierString() const -> std::string = 0;

    /**
     * @brief Get the name of the registry.
     *
     * @return The name of the registry.
     */
    auto GetRegistryName() const;

    /**
     * @brief Find out whether there exists any associations between this object and one of a given
     * type.
     *
     * @return Whether there exist any associations.
     */
    template <typename TOBJECT>
    auto IsAssociated() const noexcept;

    /**
     * @brief Find out whether there exists any associations between this object and one of a given
     * type.
     *
     * @param indicator The indicator flag for the association.
     *
     * @return Whether there exist any associations.
     */
    template <typename TOBJECT, typename TINDICATOR>
    auto IsAssociated(TINDICATOR &&indicator) const noexcept;

    /**
     * @brief Form an association between this object and another object.
     *
     * @param object The other object.
     * @param reciprocate Whether to reciprocate the association.
     *
     * @return Success.
     */
    template <typename TTHIS = TBASE_D, typename TOBJECT>
    auto Associate(TOBJECT &&object, bool reciprocate = true);

    /**
     * @brief Form an association between this object and another object.
     *
     * @param object The other object.
     * @param indicator The indicator.
     * @param reciprocate Whether to reciprocate the association.
     *
     * @return Success.
     */
    template <typename TTHIS = TBASE_D, typename TOBJECT, typename TINDICATOR>
    auto Associate(TOBJECT &&object, TINDICATOR &&indicator, bool reciprocate = true);

    /**
     * @brief Dissolve the association between this object and another object.
     *
     * @param object The other object.
     * @param reciprocate Whether to reciprocate the dissolution.
     *
     * @return Success.
     */
    template <typename TTHIS = TBASE_D, typename TOBJECT>
    auto Dissociate(TOBJECT &&object, bool reciprocate = true);

    /**
     * @brief Dissolve the association between this object and another object.
     *
     * @param object The other object.
     * @param indicator The indicator.
     * @param reciprocate Whether to reciprocate the dissolution.
     *
     * @return Success.
     */
    template <typename TTHIS = TBASE_D, typename TOBJECT, typename TINDICATOR>
    auto Dissociate(TOBJECT &&object, TINDICATOR &&indicator, bool reciprocate = true);

    /**
     * @brief Get a list of the associated objects of a given type.
     *
     * @return A list of the association objects.
     */
    template <typename TOBJECT = TBASE_D>
    auto GetAssociatedObjects() const;

    /**
     * @brief Get a list of the associated objects of a given type.
     *
     * @return A list of the association objects.
     */
    template <typename TOBJECT = TBASE_D, typename TINDICATOR>
    auto GetAssociatedObjects(TINDICATOR &&indicator) const;

    /**
     * @brief Get the associated object of a given type (assuming there is one).
     *
     * @return The associated object.
     */
    template <typename TOBJECT = TBASE>
    auto &GetAssociatedObject() const;

    /**
     * @brief Get the associated object of a given type (assuming there is one).
     *
     * @return The associated object.
     */
    template <typename TOBJECT = TBASE_D, typename TINDICATOR>
    auto &GetAssociatedObject(TINDICATOR &&indicator) const;

    /**
     * @brief Get a range-based for-loop container for looping through the associated objects of a
     * given type by reference.
     *
     * @return A range-based for-loop container.
     */
    template <typename TOBJECT = TBASE_D>
    auto AssociatedObjects() const noexcept;

    /**
     * @brief Get a range-based for-loop container for looping through the associated objects of a
     * given type by reference.
     *
     * @return A range-based for-loop container.
     */
    template <typename TOBJECT = TBASE_D, typename TINDICATOR>
    auto AssociatedObjects(TINDICATOR &&indicator) const noexcept;

    /**
     * @brief Find out whether we can cast this object as a given type.
     *
     * @return Whether the cast can be made.
     */
    template <typename TOBJECT>
    auto CanCastAs() const;

    /**
     * @brief Get this object cast as another type.
     *
     * @return This cast object.
     */
    template <typename TOBJECT>
    auto &CastAs();

    /**
     * @brief Get this object cast as another type.
     *
     * @return This cast object.
     */
    template <typename TOBJECT>
    const auto &CastAs() const;

    /**
     * @brief Less than operator.
     *
     * @param object The compared object.
     *
     * @return Whether this object's ID is less than the given object.
     */
    auto operator<(const TBASE_D &object) const;

    /**
     * @brief Greater than operator.
     *
     * @param object The compared object.
     *
     * @return Whether this object's ID is greater than the given object.
     */
    auto operator>(const TBASE_D &object) const;

    /**
     * @brief Equality operator.
     *
     * @param object The compared object.
     *
     * @return Whether this object's ID is equal to the given object's ID.
     */
    auto operator==(const TBASE_D &object) const;

    /**
     * @brief Equality operator for two weak pointers to the base type.
     *
     * @param lhs The lhs object.
     * @param rhs The rhs object.
     *
     * @return Whether this object's ID is equal to the given object's ID.
     */
    template <typename T>
    friend auto operator==(const std::weak_ptr<T> &lhs, const std::weak_ptr<T> &rhs) -> bool;
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Templates/RegisteredObjectTemplate.txx"

#endif  // #ifndef KL_REGISTERED_OBJECT_TEMPLATE_H
