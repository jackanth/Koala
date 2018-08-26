/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Templates/RegisteredObjectTemplate.txx
 *
 * @brief Template implementation header file for the registered object template
 * (RegisteredObjectTemplate) abstract class template.
 */

#ifndef KL_REGISTERED_OBJECT_TEMPLATE_IMPL_H
#define KL_REGISTERED_OBJECT_TEMPLATE_IMPL_H

#include "koala/Definitions.h"
#include "koala/RangeBasedContainer.h"
#include "koala/Registry/ObjectAssociation.h"

namespace kl
{
template <typename TBASE, typename TALIAS>
RegisteredObjectTemplate<TBASE, TALIAS>::RegisteredObjectTemplate(Registry_wPtr wpRegistry,
                                                                  const kl::ID_t id,
                                                                  Koala_wPtr wpKoala) noexcept
    : m_mutex{},
      m_id{id},
      m_wpRegistry{std::move_if_noexcept(wpRegistry)},
      m_wpKoala{std::move_if_noexcept(wpKoala)},
      m_serializableAssociations{},
      m_unserializableAssociations{},
      m_serializableAssocMultiMap{},
      m_unserializableAssocMultiMap{}
{
    static_assert(!std::is_same<TBASE_D, kl::ID_t>::value,
                  "Cannot instantiate a registered object if the base type is the same as the ID "
                  "type (std::size_t)");

    static_assert(!std::is_same<TALIAS_D, kl::ID_t>::value,
                  "Cannot instantiate a registered object if the base type is the same as the ID "
                  "type (std::size_t)");

    static_assert(
        !std::is_base_of<TBASE_D, TALIAS_D>::value,
        "Cannot instantiate a registered object if the base type is a base of the alias type");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline void RegisteredObjectTemplate<TBASE, TALIAS>::Initialize()
{
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto &RegisteredObjectTemplate<TBASE, TALIAS>::GetKoala() const
{
    if (const auto spKoala = m_wpKoala.lock()) return *spKoala;

    KL_THROW("Could not get Koala because the instance of Koala has been deleted");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline constexpr auto RegisteredObjectTemplate<TBASE, TALIAS>::IsCerealSerializable() noexcept
{
    return false;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
RegisteredObjectTemplate<TBASE, TALIAS>::RegisteredObjectTemplate() noexcept
    : m_mutex{},
      m_id{SIZE_T(0UL)},
      m_wpRegistry{},
      m_wpKoala{},
      m_serializableAssociations{},
      m_unserializableAssociations{},
      m_serializableAssocMultiMap{},
      m_unserializableAssocMultiMap{}
{
    static_assert(!std::is_same<TBASE_D, kl::ID_t>::value,
                  "Cannot instantiate a registered object if the base type is the same as the ID "
                  "type (std::size_t)");

    static_assert(!std::is_same<TALIAS_D, kl::ID_t>::value,
                  "Cannot instantiate a registered object if the base type is the same as the ID "
                  "type (std::size_t)");

    static_assert(
        !std::is_base_of<TBASE_D, TALIAS_D>::value,
        "Cannot instantiate a registered object if the base type is a base of the alias type");
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
void RegisteredObjectTemplate<TBASE, TALIAS>::TestAssociationObjectSuitability(
    TOBJECT &&object) const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    static_assert(
        std::is_base_of<
            RegisteredObjectTemplate<typename TOBJECT_D::RegisteredObject::KoalaBaseType,
                                     typename TOBJECT_D::RegisteredObject::KoalaAliasType>,
            TOBJECT_D>::value,
        "Could not form association because chosen object was not a public registered object");

    if (!this->GetKoala().template HasRegistry<TOBJECT_D>())
        KL_THROW(
            "Could not form association because chosen object's registry was not registered with "
            "koala");

    if (!object.GetRegistry().template DoesObjectExist<TOBJECT_D>(object))
        KL_THROW(
            "Could not form association because chosen object did not exist in its own registry");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::IsObjectInAssocMultiMap(
    const TypeInfoAssocMultiMap &assocMultiMap) const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    // Check if there is at least one live associated object of the correct type.
    const auto serRet = assocMultiMap.equal_range(typeid(TOBJECT_D).name());
    for (auto iter = serRet.first; iter != serRet.second; ++iter)
    {
        if (iter->second->IsAlive()) return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
auto RegisteredObjectTemplate<TBASE, TALIAS>::IsObjectInAssocMultiMap(
    const TypeInfoAssocMultiMap &assocMultiMap, TINDICATOR &&indicator) const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    // Check if there is at least one live associated object of the correct type with the provided
    // indicator.
    const auto ret = assocMultiMap.equal_range(typeid(TOBJECT_D).name());
    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (const auto spCastAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D, std::decay_t<TINDICATOR>>>(
                    iter->second))
        {
            if (spCastAssociation->Indicator() == indicator && spCastAssociation->IsAlive())
                return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::AddAssociation(
    ObjectAssociationBase::sPtr spAssociationBase) noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    if (spAssociationBase->IsCerealSerializable())
    {
        // Serializable association.
        if (m_serializableAssociations.insert(spAssociationBase).second)
        {
            m_serializableAssocMultiMap.emplace(typeid(TOBJECT_D).name(),
                                                std::move(spAssociationBase));
            return true;
        }

        return false;
    }

    // Unserializable association.
    if (m_unserializableAssociations.insert(spAssociationBase).second)
    {
        m_unserializableAssocMultiMap.emplace(typeid(TOBJECT_D).name(),
                                              std::move(spAssociationBase));
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::RemoveAssociation(TOBJECT &&object) noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    if (object.IsCerealSerializable())
    {
        // Serializable association.
        const auto ret = m_serializableAssocMultiMap.equal_range(typeid(TOBJECT_D).name());

        for (auto iter = ret.first; iter != ret.second; ++iter)
        {
            if (iter->second->ID() != object.ID() &&
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D>>(iter->second))
            {
                m_serializableAssocMultiMap.erase(iter);

                const auto findIter =
                    m_serializableAssociations.find(iter->second);  // guaranteed to be found
                KL_ASSERT(findIter != m_serializableAssociations.end(),
                          "Association information is corrupted");

                m_serializableAssociations.erase(findIter);
                return true;
            }
        }
    }

    else
    {
        // Unserializable association.
        const auto ret = m_unserializableAssocMultiMap.equal_range(typeid(TOBJECT_D).name());

        for (auto iter = ret.first; iter != ret.second; ++iter)
        {
            if (iter->second->ID() != object.ID() &&
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D>>(iter->second))
            {
                m_unserializableAssocMultiMap.erase(iter);

                const auto findIter =
                    m_unserializableAssociations.find(iter->second);  // guaranteed to be found
                KL_ASSERT(findIter != m_unserializableAssociations.end(),
                          "Association information is corrupted");

                m_unserializableAssociations.erase(findIter);
                return true;
            }
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
auto RegisteredObjectTemplate<TBASE, TALIAS>::RemoveAssociation(TOBJECT &&object,
                                                                TINDICATOR &&indicator) noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    using TINDICATOR_D = std::decay_t<TINDICATOR>;

    if (object.IsCerealSerializable())
    {
        // Serializable association.
        const auto ret = m_serializableAssocMultiMap.equal_range(typeid(TOBJECT_D).name());

        for (auto iter = ret.first; iter != ret.second; ++iter)
        {
            if (iter->second->ID() != object.ID()) continue;

            const auto spCastAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D, TINDICATOR_D>>(iter->second);
            if (spCastAssociation && spCastAssociation->Indicator() == indicator)
            {
                m_serializableAssocMultiMap.erase(iter);

                const auto findIter =
                    m_serializableAssociations.find(iter->second);  // guaranteed to be found
                KL_ASSERT(findIter != m_serializableAssociations.end(),
                          "Association information is corrupted");

                m_serializableAssociations.erase(findIter);
                return true;
            }
        }
    }

    else
    {
        // Unserializable association.
        const auto ret = m_unserializableAssocMultiMap.equal_range(typeid(TOBJECT_D).name());

        for (auto iter = ret.first; iter != ret.second; ++iter)
        {
            if (iter->second->ID() != object.ID()) continue;

            const auto spCastAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D, TINDICATOR_D>>(iter->second);
            if (spCastAssociation && spCastAssociation->Indicator() == indicator)
            {
                m_unserializableAssocMultiMap.erase(iter);

                const auto findIter =
                    m_unserializableAssociations.find(iter->second);  // guaranteed to be found
                KL_ASSERT(findIter != m_unserializableAssociations.end(),
                          "Association information is corrupted");

                m_unserializableAssociations.erase(findIter);
                return true;
            }
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObjectsFromMap(
    const TypeInfoAssocMultiMap &assocMultiMap) const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    // Add all live associated objects to the list.
    auto associatedObjects = typename TOBJECT_D::UnorderedRefSet{};
    const auto ret = assocMultiMap.equal_range(typeid(TOBJECT_D).name());

    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (const auto spCastAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D>>(iter->second))
        {
            if (spCastAssociation->IsAlive()) associatedObjects.insert(spCastAssociation->Get());
        }
    }

    return associatedObjects;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
auto RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObjectsFromMap(
    const TypeInfoAssocMultiMap &assocMultiMap, TINDICATOR &&indicator) const noexcept
{
    using TOBJECT_D = std::decay_t<TOBJECT>;

    // Add all live associated objects to the list.
    auto associatedObjects = typename TOBJECT_D::UnorderedRefSet{};
    const auto ret = assocMultiMap.equal_range(typeid(TOBJECT_D).name());

    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (const auto spCastAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D, std::decay_t<TINDICATOR>>>(
                    iter->second))
        {
            if (spCastAssociation->Indicator() == indicator && spCastAssociation->IsAlive())
                associatedObjects.insert(spCastAssociation->Get());
        }
    }

    return associatedObjects;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObjectFromMap(
    const TypeInfoAssocMultiMap &assocMultiMap) const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    auto spReturnAssociation = typename ObjectAssociation<TOBJECT_D>::sPtr{nullptr};

    const auto ret = assocMultiMap.equal_range(typeid(TOBJECT_D).name());
    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (const auto spCastAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D>>(iter->second))
        {
            if (spCastAssociation->IsAlive())
            {
                if (spReturnAssociation)
                {
                    KL_THROW(
                        "Used 'GetAssociatedObject' but there was more than one association for "
                        "object of "
                        "base type "
                        << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
                }

                spReturnAssociation = spCastAssociation;
            }
        }
    }

    return spReturnAssociation;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
auto RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObjectFromMap(
    const TypeInfoAssocMultiMap &assocMultiMap, TINDICATOR &&indicator) const
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    auto spReturnAssociation = typename ObjectAssociation<TOBJECT_D>::sPtr{nullptr};

    const auto ret = assocMultiMap.equal_range(typeid(TOBJECT_D).name());
    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (const auto spCastAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D, std::decay_t<TINDICATOR>>>(
                    iter->second))
        {
            if (spCastAssociation.Indicator() == indicator && spCastAssociation->IsAlive())
            {
                if (spReturnAssociation)
                {
                    KL_THROW(
                        "Used 'GetAssociatedObject' but there was more than one association for "
                        "object of "
                        "base type "
                        << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
                }

                spReturnAssociation = spCastAssociation;
            }
        }
    }

    return spReturnAssociation;
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TALIAS>
template <typename TARCHIVE>
inline void RegisteredObjectTemplate<TBASE, TALIAS>::save(TARCHIVE &archive) const
{
    archive(m_wpRegistry, m_id, m_wpKoala, m_serializableAssociations, m_serializableAssocMultiMap);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TALIAS>
template <typename TARCHIVE>
inline void RegisteredObjectTemplate<TBASE, TALIAS>::load(TARCHIVE &archive)
{
    archive(m_wpRegistry, m_id, m_wpKoala, m_serializableAssociations, m_serializableAssocMultiMap);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
RegisteredObjectTemplate<TBASE, TALIAS>::RegisteredObjectTemplate(
    const RegisteredObjectTemplate<TBASE, TALIAS> &other) noexcept
    : m_mutex{},
      m_id{SIZE_T(0UL)},
      m_wpRegistry{},
      m_wpKoala{},
      m_serializableAssociations{},
      m_unserializableAssociations{},
      m_serializableAssocMultiMap{},
      m_unserializableAssocMultiMap{}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = ReadLock{other.m_mutex};

    m_id = other.m_id;
    m_wpRegistry = other.m_wpRegistry;
    m_wpKoala = other.m_wpKoala;
    m_serializableAssociations = other.m_serializableAssociations;
    m_unserializableAssociations = other.m_unserializableAssociations;
    m_serializableAssocMultiMap = other.m_serializableAssocMultiMap;
    m_unserializableAssocMultiMap = other.m_unserializableAssocMultiMap;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
RegisteredObjectTemplate<TBASE, TALIAS>::RegisteredObjectTemplate(
    RegisteredObjectTemplate<TBASE, TALIAS> &&other) noexcept
    : m_mutex{},
      m_id{SIZE_T(0UL)},
      m_wpRegistry{},
      m_wpKoala{},
      m_serializableAssociations{},
      m_unserializableAssociations{},
      m_serializableAssocMultiMap{},
      m_unserializableAssocMultiMap{}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = WriteLock{other.m_mutex};

    m_id = other.m_id;
    m_wpRegistry = std::move_if_noexcept(other.m_wpRegistry);
    m_wpKoala = std::move_if_noexcept(other.m_wpKoala);
    m_serializableAssociations = std::move_if_noexcept(other.m_serializableAssociations);
    m_unserializableAssociations = std::move_if_noexcept(other.m_unserializableAssociations);
    m_serializableAssocMultiMap = std::move_if_noexcept(other.m_serializableAssocMultiMap);
    m_unserializableAssocMultiMap = std::move_if_noexcept(other.m_unserializableAssocMultiMap);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto &RegisteredObjectTemplate<TBASE, TALIAS>::operator=(
    const RegisteredObjectTemplate<TBASE, TALIAS> &other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = ReadLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        m_id = other.m_id;
        m_wpRegistry = other.m_wpRegistry;
        m_wpKoala = other.m_wpKoala;
        m_serializableAssociations = other.m_serializableAssociations;
        m_unserializableAssociations = other.m_unserializableAssociations;
        m_serializableAssocMultiMap = other.m_serializableAssocMultiMap;
        m_unserializableAssocMultiMap = other.m_unserializableAssocMultiMap;
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto &RegisteredObjectTemplate<TBASE, TALIAS>::operator=(
    RegisteredObjectTemplate<TBASE, TALIAS> &&other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = WriteLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        m_id = other.m_id;
        m_wpRegistry = std::move_if_noexcept(other.m_wpRegistry);
        m_wpKoala = std::move_if_noexcept(other.m_wpKoala);
        m_serializableAssociations = std::move_if_noexcept(other.m_serializableAssociations);
        m_unserializableAssociations = std::move_if_noexcept(other.m_unserializableAssociations);
        m_serializableAssocMultiMap = std::move_if_noexcept(other.m_serializableAssocMultiMap);
        m_unserializableAssocMultiMap = std::move_if_noexcept(other.m_unserializableAssocMultiMap);
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto &RegisteredObjectTemplate<TBASE, TALIAS>::GetRegistry() const
{
    if (const auto spRegistry = m_wpRegistry.lock()) return *spRegistry;

    KL_THROW("Could not get registry because the registry for this object has been deleted");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociationInformation() const
{
    const auto lock = ReadLock{m_mutex};

    auto associationInformationList = AssociationInformation::Vector{};
    for (const auto &spAssociationBase : m_serializableAssociations)
        associationInformationList.push_back(AssociationInformation{spAssociationBase});

    for (const auto &spAssociationBase : m_unserializableAssociations)
        associationInformationList.push_back(AssociationInformation{spAssociationBase});

    return associationInformationList;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::GetRegistryName() const
{
    return this->GetRegistry().PrintableBaseName();
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::IsAssociated() const noexcept
{
    const auto lock = ReadLock{m_mutex};
    return (this->IsObjectInAssocMultiMap<TOBJECT>(m_serializableAssocMultiMap) ||
            this->IsObjectInAssocMultiMap<TOBJECT>(m_unserializableAssocMultiMap));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::IsAssociated(TINDICATOR &&indicator) const
    noexcept
{
    const auto lock = ReadLock{m_mutex};

    using TOBJECT_D = std::decay_t<TOBJECT>;
    using TINDICATOR_D = std::decay_t<TINDICATOR>;

    return (this->IsObjectInAssocMultiMap<TOBJECT_D>(m_serializableAssocMultiMap,
                                                     std::forward<TINDICATOR_D>(indicator)) ||
            this->IsObjectInAssocMultiMap<TOBJECT_D>(m_unserializableAssocMultiMap,
                                                     std::forward<TINDICATOR_D>(indicator)));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHIS, typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::Associate(TOBJECT &&object, const bool reciprocate)
{
    this->TestAssociationObjectSuitability(
        object);  // this will throw a descriptive error if it fails

    using TOBJECT_D = std::decay_t<TOBJECT>;
    using TTHIS_D = std::decay_t<TTHIS>;

    // Instantiating the association automatically forms a reciprocal association with the other
    // object if reciprocate is true.
    auto spThisObject = std::dynamic_pointer_cast<TTHIS_D>(this->GetSharedPointer());
    if (!spThisObject) KL_THROW("Object could not be cast to desired type for association");

    auto spAssociationBase = ObjectAssociationBase::sPtr{new ObjectAssociation<TOBJECT_D>{
        *spThisObject, std::forward<TOBJECT_D>(object), reciprocate}};

    const auto lock = WriteLock{m_mutex};
    return this->AddAssociation<TOBJECT_D>(std::move(spAssociationBase));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHIS, typename TOBJECT, typename TINDICATOR>
auto RegisteredObjectTemplate<TBASE, TALIAS>::Associate(TOBJECT &&object, TINDICATOR &&indicator,
                                                        const bool reciprocate)
{
    this->TestAssociationObjectSuitability(
        object);  // this will throw a descriptive error if it fails

    using TOBJECT_D = std::decay_t<TOBJECT>;
    using TINDICATOR_D = std::decay_t<TINDICATOR>;

    static_assert(std::is_default_constructible<TINDICATOR_D>::value,
                  "Association indicators must be default-constructible");
    static_assert(std::is_move_constructible<TINDICATOR_D>::value,
                  "Association indicators must be move-constructible");
    static_assert(std::is_move_assignable<TINDICATOR_D>::value,
                  "Association indicators must be move-assignable");
    static_assert(std::is_copy_constructible<TINDICATOR_D>::value,
                  "Association indicators must be copy-constructible");
    static_assert(std::is_copy_assignable<TINDICATOR_D>::value,
                  "Association indicators must be copy-assignable");

    // Instantiating the association automatically forms a reciprocal association with the other
    // object if reciprocate is true.
    auto &thisObject = *std::dynamic_pointer_cast<std::decay_t<TTHIS>>(this->GetSharedPointer());
    auto spAssociationBase =
        ObjectAssociationBase::sPtr{new ObjectAssociation<TOBJECT_D, TINDICATOR_D>{
            thisObject, std::forward<TOBJECT_D>(object), reciprocate,
            std::forward<TINDICATOR_D>(indicator)}};
    const auto lock = WriteLock{m_mutex};
    return this->AddAssociation<TOBJECT_D>(std::move(spAssociationBase));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHIS, typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::Dissociate(TOBJECT &&object, const bool reciprocate)
{
    const auto lock = WriteLock{m_mutex};
    this->TestAssociationObjectSuitability(
        object);  // this will throw a descriptive error if it fails

    if (!this->RemoveAssociation(object)) return false;

    // If we can't make the dissolution, then either the user made a one-way association in the
    // first place or something bad has happened.
    using TTHIS_D = std::decay_t<TTHIS>;
    if (reciprocate && !object.template Dissociate<std::decay_t<TOBJECT>, TTHIS_D>(
                           *std::dynamic_pointer_cast<TTHIS_D>(this->GetSharedPointer()), false))
    {
        KL_ASSERT(false, "Could not reciprocate association dissolution for object of base type "
                             << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName()
                             << KL_NORMAL << ": association integrity has been broken");
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHIS, typename TOBJECT, typename TINDICATOR>
auto RegisteredObjectTemplate<TBASE, TALIAS>::Dissociate(TOBJECT &&object, TINDICATOR &&indicator,
                                                         const bool reciprocate)
{
    const auto lock = WriteLock{m_mutex};
    this->TestAssociationObjectSuitability(
        object);  // this will throw a descriptive error if it fails

    if (!this->RemoveAssociation(object, indicator)) return false;

    // If we can't make the dissolution, then either the user made a one-way association in the
    // first place or something bad has happened.
    using TTHIS_D = std::decay_t<TTHIS>;
    using TINDICATOR_D = std::decay_t<TINDICATOR>;

    if (reciprocate && !object.template Dissociate<std::decay_t<TOBJECT>>(
                           *std::dynamic_pointer_cast<TTHIS_D>(this->GetSharedPointer()),
                           std::forward<TINDICATOR_D>(indicator), false))
    {
        KL_ASSERT(false, "Could not reciprocate association dissolution for object of base type "
                             << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName()
                             << KL_NORMAL << ": association integrity has been broken");
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObjects() const
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;

    auto associatedObjects =
        this->GetAssociatedObjectsFromMap<TOBJECT_D>(m_serializableAssocMultiMap);
    auto unserAssocObjects =
        this->GetAssociatedObjectsFromMap<TOBJECT_D>(m_unserializableAssocMultiMap);
    associatedObjects.insert(std::make_move_iterator(unserAssocObjects.begin()),
                             std::make_move_iterator(unserAssocObjects.end()));

    return associatedObjects;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
auto RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObjects(TINDICATOR &&indicator) const
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;

    auto associatedObjects =
        this->GetAssociatedObjectsFromMap<TOBJECT_D>(m_serializableAssocMultiMap, indicator);
    auto unserAssocObjects =
        this->GetAssociatedObjectsFromMap<TOBJECT_D>(m_unserializableAssocMultiMap, indicator);
    associatedObjects.insert(std::make_move_iterator(unserAssocObjects.begin()),
                             std::make_move_iterator(unserAssocObjects.end()));

    return associatedObjects;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto &RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObject() const
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;

    auto spSerReturnAssoc =
        this->GetAssociatedObjectFromMap<TOBJECT_D>(m_serializableAssocMultiMap);
    auto spUnserReturnAssoc =
        this->GetAssociatedObjectFromMap<TOBJECT_D>(m_unserializableAssocMultiMap);

    if (spSerReturnAssoc && spUnserReturnAssoc)
    {
        KL_THROW(
            "Used 'GetAssociatedObject' but there was more than one association for object of base "
            "type "
            << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
    }

    if (spSerReturnAssoc || spUnserReturnAssoc)
        return spSerReturnAssoc ? spSerReturnAssoc->Get() : spUnserReturnAssoc->Get();

    KL_THROW("There were no live objects of this type found for object of base type "
             << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
auto &RegisteredObjectTemplate<TBASE, TALIAS>::GetAssociatedObject(TINDICATOR &&indicator) const
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;

    auto spSerReturnAssoc =
        this->GetAssociatedObjectFromMap<TOBJECT_D>(m_serializableAssocMultiMap, indicator);
    auto spUnserReturnAssoc =
        this->GetAssociatedObjectFromMap<TOBJECT_D>(m_unserializableAssocMultiMap, indicator);

    if (spSerReturnAssoc && spUnserReturnAssoc)
    {
        KL_THROW(
            "Used 'GetAssociatedObject' but there was more than one association for object of base "
            "type "
            << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
    }

    if (spSerReturnAssoc || spUnserReturnAssoc)
        return spSerReturnAssoc ? spSerReturnAssoc->Get() : spUnserReturnAssoc->Get();

    KL_THROW("There were no live objects of this type found for object of base type "
             << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::AssociatedObjects() const noexcept
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return RangeBasedContainer<ObjectAssociationBase::sPtrSet, ObjectAssociationBase,
                               ObjectAssociation<TOBJECT_D>, TOBJECT_D>{
        [](const ObjectAssociationBase::sPtr &spAssocBase) { return spAssocBase->IsAlive(); },
        [](const ObjectAssociationBase::sPtr &spAssocBase) { return spAssocBase; },
        [](const typename ObjectAssociation<std::decay_t<TOBJECT>>::sPtr &spAssoc)
            -> auto &{return spAssoc->Get();
}
, ReadLock{m_mutex}, ReadLock{}, m_serializableAssociations, m_unserializableAssociations
};  // namespace kl
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TINDICATOR>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::AssociatedObjects(TINDICATOR &&indicator) const
    noexcept
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;
    using TINDICATOR_D = std::decay_t<TINDICATOR>;
    return RangeBasedContainer<ObjectAssociationBase::sPtrSet, ObjectAssociationBase,
                               ObjectAssociation<TOBJECT_D, TINDICATOR_D>, TOBJECT_D>{
        [&indicator](const ObjectAssociationBase::sPtr &spAssocBase) {
            const auto spAssociation =
                std::dynamic_pointer_cast<ObjectAssociation<TOBJECT_D, TINDICATOR_D>>(spAssocBase);
            return (spAssociation && spAssociation->IsAlive() &&
                    (spAssociation->Indicator() == indicator));
        },
        [](const ObjectAssociationBase::sPtr &spAssocBase) { return spAssocBase; },
        [](const typename ObjectAssociation<TOBJECT_D, TINDICATOR_D>::sPtr &spAssoc)
            -> auto &{return spAssoc->Get();
}
, ReadLock{m_mutex}, ReadLock{}, m_serializableAssociations, m_unserializableAssociations
}
;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto RegisteredObjectTemplate<TBASE, TALIAS>::CanCastAs() const
{
    try
    {
        return (std::dynamic_pointer_cast<const std::decay_t<TOBJECT>>(this->GetSharedPointer()) !=
                nullptr);
    }

    catch (const std::bad_weak_ptr &error)
    {
        KL_THROW("Could not cast object because shared_from_this method failed - bad weak pointer: "
                 << error.what());
    }

    catch (...)
    {
        KL_THROW("Could not cast object because shared_from_this method failed");
    }

    throw;  // unreachable
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto &RegisteredObjectTemplate<TBASE, TALIAS>::CastAs()
{
    try
    {
        if (const auto spCastObject =
                std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(this->GetSharedPointer()))
            return *spCastObject;
    }

    catch (const std::bad_weak_ptr &error)
    {
        KL_THROW("Could not cast object because shared_from_this method failed - bad weak pointer: "
                 << error.what());
    }

    catch (...)
    {
        KL_THROW("Could not cast object because shared_from_this method failed");
    }

    KL_THROW("Could not perform cast for object of base type "
             << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
const auto &RegisteredObjectTemplate<TBASE, TALIAS>::CastAs() const
{
    try
    {
        if (const auto spCastObject =
                std::dynamic_pointer_cast<const std::decay_t<TOBJECT>>(this->GetSharedPointer()))
            return *spCastObject;
    }

    catch (const std::bad_weak_ptr &error)
    {
        KL_THROW("Could not cast object because shared_from_this method failed - bad weak pointer: "
                 << error.what());
    }

    catch (...)
    {
        KL_THROW("Could not cast object because shared_from_this method failed");
    }

    KL_THROW("Could not perform cast for object of base type "
             << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::operator<(const TBASE_D &object) const
{
    return (this->ID() < object.ID());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::operator>(const TBASE_D &object) const
{
    return (this->ID() > object.ID());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto RegisteredObjectTemplate<TBASE, TALIAS>::operator==(const TBASE_D &object) const
{
    return (this->ID() == object.ID());
}

//--------------------------------------------------------------------------------------------------

template <typename T>
inline auto operator==(const std::weak_ptr<T> &lhs, const std::weak_ptr<T> &rhs) -> bool
{
    return lhs.lock()->ID() == rhs.lock()->ID();
}
}  // namespace kl

#endif  // #ifndef KL_REGISTERED_OBJECT_TEMPLATE_IMPL_H