/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/ObjectRegistry.txx
 *
 * @brief Template implementation header file for the object registry (ObjectRegistry) class
 * template.
 */

#ifndef KL_OBJECT_REGISTRY_IMPL_H
#define KL_OBJECT_REGISTRY_IMPL_H 1

#include "koala/Definitions.h"
#include "koala/RangeBasedContainer.h"

namespace kl
{
#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void ObjectRegistryBase::serialize(TARCHIVE &)
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
ObjectRegistry<TBASE, TALIAS>::ObjectRegistry(Koala_wPtr wpKoala,
                                              std::string printableBaseName) noexcept
    : ObjectRegistryBase{},
      std::enable_shared_from_this<ObjectRegistry<TBASE, TALIAS>>{},
      m_mutex{},
      m_wpKoala{std::move_if_noexcept(wpKoala)},
      m_printableBaseName{std::move_if_noexcept(printableBaseName)},
      m_idCount{SIZE_T(0UL)},
      m_objectIdMap{},
      m_objectTypeMultiMap{},
      m_objectAliasToIdMap{},
      m_objectIdToAliasMap{}
{
    static_assert(
        !std::is_same<TBASE_D, ID_t>::value,
        "Cannot instantiate an object registry if the base type is the same as the ID type "
        "(std::size_t)");

    static_assert(!std::is_same<TALIAS_D, ID_t>::value,
                  "Cannot instantiate an object registry if the alias type is the same as the ID "
                  "type (std::size_t)");

    static_assert(
        !std::is_base_of<TBASE_D, TALIAS_D>::value,
        "Cannot instantiate an object registry if the base type is a base of the alias type");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto ObjectRegistry<TBASE, TALIAS>::GetSharedPointer() noexcept
{
    return this->shared_from_this();
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto &ObjectRegistry<TBASE, TALIAS>::GetKoala() const
{
    if (const auto spKoala = m_wpKoala.lock()) return *spKoala;

    KL_THROW("Failed to get instance of Koala");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHISALIAS, typename TOBJECT>
auto ObjectRegistry<TBASE, TALIAS>::AddSharedPointerByAlias(
    TTHISALIAS &&objectAlias, const std::shared_ptr<TOBJECT> &spObject)
{
    const auto spCastObject = TBASE_sPtr{spObject};
    const auto objectId = m_idCount++;

    // Atomicity of ID requires that the map insertion be successful.
    KL_ASSERT(m_objectIdMap.emplace(objectId, spCastObject).second,
              "Failed to append to object ID map");
    m_objectTypeMultiMap.emplace(typeid(std::decay_t<TOBJECT>).name(), spCastObject);

    if (!m_objectAliasToIdMap.emplace(objectAlias, objectId).second)
    {
        KL_THROW("Could not add object of type " << KL_WHITE_BOLD << m_printableBaseName
                                                 << KL_NORMAL << " by given alias (alias "
                                                 << "already exists)");
    }

    // Atomicity of ID requires that the map insertion be successful.
    KL_ASSERT(m_objectIdToAliasMap.emplace(objectId, std::forward<TTHISALIAS>(objectAlias)).second,
              "Failed to append to ID-to-alias map");
    return objectId;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TTHISALIAS, typename... TPARAMETERS>
auto ObjectRegistry<TBASE, TALIAS>::CreateSharedPointerByAlias(TTHISALIAS &&objectAlias,
                                                               TPARAMETERS &&... parameters)
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    // static_assert(std::is_constructible<TOBJECT_D,
    // decltype(static_cast<ObjectRegistry::wPtr>(this->shared_from_this())),
    //                                    decltype(m_idCount.load()), decltype(m_wpKoala),
    //                                    decltype(std::forward<TPARAMETERS>(parameters))...>::value,
    //              "Incorrect constructor arguments passed to object registry");

    const auto spObject = TBASE_sPtr{
        new TOBJECT_D{static_cast<ObjectRegistry::wPtr>(this->shared_from_this()), m_idCount.load(),
                      m_wpKoala, std::forward<TPARAMETERS>(parameters)...}};

    spObject->Initialize();
    ++m_idCount;

    // Atomicity of ID requires that the map insertion be successful.
    KL_ASSERT(m_objectIdMap.emplace(spObject->ID(), spObject).second,
              "Failed to append to object ID map");
    m_objectTypeMultiMap.emplace(typeid(TOBJECT_D).name(), spObject);

    if (!m_objectAliasToIdMap.emplace(objectAlias, spObject->ID()).second)
    {
        KL_THROW("Could not create object of type " << KL_WHITE_BOLD << m_printableBaseName
                                                    << KL_NORMAL << " by given alias (alias "
                                                    << "already exists)");
    }

    // Atomicity of ID requires that the map insertion be successful.
    KL_ASSERT(
        m_objectIdToAliasMap.emplace(spObject->ID(), std::forward<TTHISALIAS>(objectAlias)).second,
        "Failed to append to ID-to-alias map");

    return spObject;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename... TPARAMETERS>
auto ObjectRegistry<TBASE, TALIAS>::CreateSharedPointer(TPARAMETERS &&... parameters) noexcept(
    std::is_nothrow_constructible<std::decay_t<TOBJECT>, ObjectRegistry::wPtr, const ID_t, Koala_wPtr,
                                  TPARAMETERS...>::value)
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    const auto spObject = TBASE_sPtr{
        new TOBJECT_D{static_cast<ObjectRegistry::wPtr>(this->shared_from_this()), m_idCount.load(),
                      m_wpKoala, std::forward<TPARAMETERS>(parameters)...}};

    spObject->Initialize();
    ++m_idCount;

    // Atomicity of ID requires that the map insertion be successful.
    KL_ASSERT(m_objectIdMap.emplace(spObject->ID(), spObject).second,
              "Failed to append to object ID map");
    m_objectTypeMultiMap.emplace(typeid(TOBJECT_D).name(), spObject);

    return spObject;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename T, typename>
inline auto ObjectRegistry<TBASE, TALIAS>::GetSharedPointer(T &&arg) const
{
    return this->GetSharedPointerImpl(
        std::forward<T>(arg),
        typename std::is_same<TALIAS_D, std::decay_t<T>>::type());  // tag dispatch
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::GetSharedPointer(const ID_t objectId) const
{
    const auto findIter = m_objectIdMap.find(objectId);
    if (findIter != m_objectIdMap.cend()) return findIter->second;

    KL_THROW("Could not find object of base type " << KL_WHITE_BOLD << m_printableBaseName
                                                   << KL_NORMAL
                                                   << " by the given ID: " << objectId);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto ObjectRegistry<TBASE, TALIAS>::DeleteSharedPointer(
    ObjectRegistry::TBASE_sPtr &&spObject) noexcept
{
    // Delete from ID map.
    const auto idFindIter = m_objectIdMap.find(spObject->ID());
    if (idFindIter == m_objectIdMap.cend()) return false;  // not found

    m_objectIdMap.erase(idFindIter);

    // Delete from the type map (the mechanics guarantees it be found singly).
    for (auto iter = m_objectTypeMultiMap.cbegin(); iter != m_objectTypeMultiMap.cend();
         /* no iterator */)
    {
        if (iter->second == spObject)
        {
            m_objectTypeMultiMap.erase(iter);
            break;
        }

        else
            ++iter;
    }

    // If it has an alias, delete it from the other maps.
    const auto idToAliasFindIter = m_objectIdToAliasMap.find(spObject->ID());
    if (idToAliasFindIter == m_objectIdToAliasMap.cend()) return true;

    const auto aliasToIdFindIter =
        m_objectAliasToIdMap.find(idToAliasFindIter->second);  // mechanics guarantees this be found
    KL_ASSERT(aliasToIdFindIter != m_objectAliasToIdMap, "Failed to find entry in alias-to-ID map");

    m_objectAliasToIdMap.erase(aliasToIdFindIter);
    m_objectIdToAliasMap.erase(idToAliasFindIter);

    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHISALIAS>
auto ObjectRegistry<TBASE, TALIAS>::DeleteImpl(TTHISALIAS &&objectAlias, std::true_type) noexcept
{
    // Try to find it in the alias-to-ID map: return false if it can't be found.
    const auto aliasToIdFindIter = m_objectAliasToIdMap.find(objectAlias);
    if (aliasToIdFindIter == m_objectAliasToIdMap.cend()) return false;

    // We need to get the object ID so we can delete it from the ID map too.
    const auto objectId = aliasToIdFindIter->second;

    // Delete from the type map (the mechanics guarantees it be found singly).
    for (auto iter = m_objectTypeMultiMap.cbegin(); iter != m_objectTypeMultiMap.cend();
         /* no iterator */)
    {
        if (iter->second->ID() == objectId)
        {
            m_objectTypeMultiMap.erase(iter);
            break;
        }

        else
            ++iter;
    }

    // Object must have an alias.
    const auto idMapFindIter =
        m_objectIdMap.find(objectId);  // the mechanics guarantees this be found
    KL_ASSERT(idMapFindIter != m_objectIdMap.end(), "Failed to find entry in object ID map");

    m_objectIdMap.erase(idMapFindIter);
    m_objectAliasToIdMap.erase(aliasToIdFindIter);

    const auto idToAliasFindIter =
        m_objectIdToAliasMap.find(objectId);  // the mechanics guarantees this be found
    KL_ASSERT(idToAliasFindIter != m_objectIdToAliasMap.end(),
              "Failed to find entry in ID-to-alias map");
    m_objectIdToAliasMap.erase(idToAliasFindIter);

    // KL_IF_DEBUG_MESSAGE(KL_LIGHT_GREY << m_printableBaseName << KL_NORMAL
    //                                   << " registry deleted object with ID " << KL_WHITE_BOLD
    //                                   << objectId);
    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto ObjectRegistry<TBASE, TALIAS>::DeleteImpl(TOBJECT &&object, std::false_type) noexcept
{
    const auto idFindIter = m_objectIdMap.find(object.ID());
    if (idFindIter == m_objectIdMap.cend()) return false;

    m_objectIdMap.erase(idFindIter);
    const auto objectId = object.ID();

    // Delete from the type map (the mechanics guarantees it be found singly).
    const auto ret = m_objectTypeMultiMap.equal_range(typeid(TOBJECT).name());
    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (iter->second->ID() == objectId)
        {
            m_objectTypeMultiMap.erase(iter);
            break;
        }
    }

    // If it has an alias, delete it from the other maps.
    const auto idToAliasFindIter = m_objectIdToAliasMap.find(objectId);
    if (idToAliasFindIter == m_objectIdToAliasMap.cend()) return true;

    const auto alias = idToAliasFindIter->second;
    m_objectIdToAliasMap.erase(idToAliasFindIter);

    const auto aliasToIdFindIter =
        m_objectAliasToIdMap.find(alias);  // mechanics guarantees this be successful
    KL_ASSERT(aliasToIdFindIter != m_objectAliasToIdMap.end(),
              "Failed to find entry in alias-to-ID map");
    m_objectAliasToIdMap.erase(aliasToIdFindIter);

    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto ObjectRegistry<TBASE, TALIAS>::GetSharedPointerImpl(TOBJECT &&object,
                                                                std::false_type) const
{
    const auto findIter = m_objectIdMap.find(object.ID());
    if (findIter != m_objectIdMap.cend()) return findIter->second;

    KL_THROW("Could not find object by the given ID for object of base type "
             << KL_WHITE_BOLD << m_printableBaseName << KL_NORMAL << ": " << object.ID());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHISALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::GetSharedPointerImpl(TTHISALIAS &&objectAlias,
                                                                std::true_type) const
{
    const auto aliasFindIter = m_objectAliasToIdMap.find(objectAlias);
    if (aliasFindIter != m_objectAliasToIdMap.cend())
    {
        const auto idFindIter =
            m_objectIdMap.find(aliasFindIter->second);  // the mechanics guarantees this be found
        KL_ASSERT(idFindIter != m_objectIdMap.end(), "Failed to find entry");

        return idFindIter->second;  // return the pointer
    }

    KL_THROW("Could not find object by the given alias for object of base type "
             << KL_WHITE_BOLD << m_printableBaseName);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TDESIRED, typename TOBJECT>
inline auto ObjectRegistry<TBASE, TALIAS>::DoesObjectExistImpl(TOBJECT &&object,
                                                               std::false_type) const noexcept
{
    const auto findIter = m_objectIdMap.find(object.ID());
    if (findIter == m_objectIdMap.cend()) return false;

    return (std::dynamic_pointer_cast<std::decay_t<TDESIRED>>(findIter->second) != nullptr);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TDESIRED, typename TTHISALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::DoesObjectExistImpl(TTHISALIAS &&objectAlias,
                                                               std::true_type) const noexcept
{
    const auto aliasFindIter = m_objectAliasToIdMap.find(objectAlias);
    if (aliasFindIter == m_objectAliasToIdMap.cend()) return false;

    return (std::dynamic_pointer_cast<std::decay_t<TDESIRED>>(
                m_objectIdMap.find(aliasFindIter->second)->second) != nullptr);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::HasAliasImpl(const ID_t objectId) const noexcept
{
    return (m_objectIdToAliasMap.find(objectId) != m_objectIdToAliasMap.cend());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHISALIAS>
inline void ObjectRegistry<TBASE, TALIAS>::AddAliasImpl(const ID_t objectId, TTHISALIAS &&alias)
{
    if (this->HasAliasImpl(objectId))
    {
        KL_THROW("Could not add alias for object of base type "
                 << KL_WHITE_BOLD << m_printableBaseName << KL_NORMAL << " because "
                 << "object already had an alias");
    }

    if (!m_objectAliasToIdMap.emplace(alias, objectId).second)
    {
        KL_THROW("Could not add given alias for object of base type "
                 << KL_WHITE_BOLD << m_printableBaseName << KL_NORMAL << " (alias already exists)");
    }

    // Atomicity of ID requires that the map insertion be successful.
    KL_ASSERT(m_objectIdToAliasMap.emplace(objectId, std::forward<TTHISALIAS>(alias)).second,
              "Failed to append to ID-to-alias map");
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TALIAS>
template <typename TARCHIVE>
inline void ObjectRegistry<TBASE, TALIAS>::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<ObjectRegistryBase>(this), m_wpKoala, m_printableBaseName,
            m_idCount.load(), m_objectIdMap, m_objectTypeMultiMap, m_objectAliasToIdMap,
            m_objectIdToAliasMap);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TALIAS>
inline ObjectRegistry<TBASE, TALIAS>::ObjectRegistry() noexcept
    : ObjectRegistryBase{},
      std::enable_shared_from_this<ObjectRegistry<TBASE, TALIAS>>{},
      m_mutex{},
      m_wpKoala{},
      m_printableBaseName{},
      m_idCount{SIZE_T(0UL)},
      m_objectIdMap{},
      m_objectTypeMultiMap{},
      m_objectAliasToIdMap{},
      m_objectIdToAliasMap{}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TALIAS>
template <typename TARCHIVE>
void ObjectRegistry<TBASE, TALIAS>::load_and_construct(
    TARCHIVE &archive, cereal::construct<ObjectRegistry<TBASE, TALIAS>> &construct)
{
    auto wpKoala = Koala_wPtr{};
    auto printableBaseName = std::string{};
    auto idCount = SIZE_T(0UL);

    auto objectIdMap = ObjectIdMap{};
    auto objectTypeMultiMap = ObjectTypeMultiMap{};
    auto objectAliasToIdMap = ObjectAliasToIdMap{};
    auto objectIdToAliasMap = ObjectIdToAliasMap{};

    // Load archived variables and construct the object.
    construct();
    archive(cereal::base_class<ObjectRegistryBase>(construct->GetSharedPointer().get()), wpKoala,
            printableBaseName, idCount, objectIdMap, objectTypeMultiMap, objectAliasToIdMap,
            objectIdToAliasMap);

    construct->m_wpKoala = std::move(wpKoala);
    construct->m_printableBaseName = std::move(printableBaseName);
    construct->m_objectIdMap = std::move(objectIdMap);
    construct->m_objectTypeMultiMap = std::move(objectTypeMultiMap);
    construct->m_objectAliasToIdMap = std::move(objectAliasToIdMap);
    construct->m_objectIdToAliasMap = std::move(objectIdToAliasMap);
    construct->m_idCount.store(idCount);  // atomicity not guaranteed
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::PrintableBaseName() const noexcept -> std::string
{
    return m_printableBaseName;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename... TPARAMETERS>
inline auto &ObjectRegistry<TBASE, TALIAS>::Create(TPARAMETERS &&... parameters)
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    auto spObject = TBASE_sPtr{};

    {
        const auto lock = WriteLock{m_mutex};
        spObject = this->CreateSharedPointer<TOBJECT_D>(std::forward<TPARAMETERS>(parameters)...);
    }

    const auto spCastObject =
        std::dynamic_pointer_cast<TOBJECT_D>(spObject);  // this is guaranteed to be possible
    KL_ASSERT(spCastObject, "Failed to cast object");

    // KL_IF_DEBUG_MESSAGE(KL_LIGHT_GREY
    //                     << m_printableBaseName << KL_NORMAL << " registry created object '"
    //                     << KL_LIGHT_GREY << spCastObject->GetIdentifierString() << KL_NORMAL
    //                     << "' of type " << KL_LIGHT_GREY << TOBJECT_D::StaticPrintableName()
    //                     << KL_NORMAL << " with ID " << KL_WHITE_BOLD << spCastObject->ID());

    return *spCastObject;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TTHISALIAS, typename... TPARAMETERS>
inline auto &ObjectRegistry<TBASE, TALIAS>::CreateByAlias(TTHISALIAS &&objectAlias,
                                                          TPARAMETERS &&... parameters)
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    auto spObject = TBASE_sPtr{};

    {
        const auto lock = WriteLock{m_mutex};
        spObject = this->CreateSharedPointerByAlias<TOBJECT_D>(
            std::forward<TTHISALIAS>(objectAlias), std::forward<TPARAMETERS>(parameters)...);
    }

    const auto spCastObject =
        std::dynamic_pointer_cast<TOBJECT_D>(spObject);  // this is guaranteed to be possible
    KL_ASSERT(spCastObject, "Failed to cast object");

    // KL_IF_DEBUG_MESSAGE(
    //     KL_LIGHT_GREY << m_printableBaseName << KL_NORMAL << " registry created object '"
    //                   << KL_LIGHT_GREY << spCastObject->GetIdentifierString() << KL_NORMAL
    //                   << "' of type " << KL_LIGHT_GREY << TOBJECT_D::StaticPrintableName()
    //                   << KL_NORMAL << " by alias with ID " << KL_WHITE_BOLD << spCastObject->ID());

    return *spCastObject;  // this is guaranteed to be possible
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TTHISALIAS, typename>
inline auto &ObjectRegistry<TBASE, TALIAS>::Get(TTHISALIAS &&objectAlias) const
{
    const auto lock = ReadLock{m_mutex};
    const auto spObject = this->GetSharedPointer(std::forward<TTHISALIAS>(objectAlias));

    if (const auto spCastObject = std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spObject))
        return *spCastObject;

    KL_THROW("The registry could not dynamically cast the called "
             << KL_WHITE_BOLD << m_printableBaseName << KL_NORMAL << " object "
             << "into the desired type");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename>
inline auto &ObjectRegistry<TBASE, TALIAS>::Get(TOBJECT &&object) const
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;
    const auto spObject = this->GetSharedPointer(std::forward<TOBJECT>(object));

    if (const auto spCastObject = std::dynamic_pointer_cast<TOBJECT_D>(spObject))
        return *spCastObject;

    KL_THROW("The registry could not dynamically cast the called "
             << KL_WHITE_BOLD << m_printableBaseName << KL_NORMAL << " object "
             << "into the desired type");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto &ObjectRegistry<TBASE, TALIAS>::Get(const ID_t objectId) const
{
    const auto lock = ReadLock{m_mutex};
    const auto spObject = this->GetSharedPointer(objectId);

    if (const auto spCastObject = std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spObject))
        return *spCastObject;

    KL_THROW("The registry could not dynamically cast the called "
             << KL_WHITE_BOLD << m_printableBaseName << KL_NORMAL << " object "
             << "into the desired type");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto ObjectRegistry<TBASE, TALIAS>::GetAllList() const noexcept
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;

    auto objectList = typename TOBJECT_D::UnorderedRefSet{};
    const auto ret = m_objectTypeMultiMap.equal_range(typeid(TOBJECT_D).name());

    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (const auto spCastObject = std::dynamic_pointer_cast<TOBJECT_D>(iter->second))
            objectList.insert(*spCastObject);
    }

    return objectList;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto ObjectRegistry<TBASE, TALIAS>::GetAll() const
{
    const auto lock = ReadLock{m_mutex};
    using TOBJECT_D = std::decay_t<TOBJECT>;
    return RangeBasedContainer<ObjectIdMap, TBASE_D, TOBJECT_D, TOBJECT_D>{
        [](const typename ObjectIdMap::value_type &) { return true; },
        [](const typename ObjectIdMap::value_type &mapElement) { return mapElement.second; },
        [](const std::shared_ptr<TOBJECT_D> &spObject) -> auto &{return *spObject;
}
, ReadLock{m_mutex}, ReadLock{}, m_objectIdMap
};  // namespace kl
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto ObjectRegistry<TBASE, TALIAS>::Count() const noexcept
{
    const auto lock = ReadLock{m_mutex};

    using TOBJECT_D = std::decay_t<TOBJECT>;
    const auto ret = m_objectTypeMultiMap.equal_range(typeid(TOBJECT_D).name());
    auto count = SIZE_T(0UL);

    for (auto iter = ret.first; iter != ret.second; ++iter)
    {
        if (std::dynamic_pointer_cast<TOBJECT_D>(iter->second)) ++count;
    }

    return count;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::CountAll() const noexcept
{
    const auto lock = ReadLock{m_mutex};
    return m_objectIdMap.size();
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename T, typename>
inline auto ObjectRegistry<TBASE, TALIAS>::Delete(T &&arg) noexcept
{
    const auto lock = WriteLock{m_mutex};
    return this->DeleteImpl(
        std::forward<T>(arg),
        typename std::is_same<TALIAS_D, std::decay_t<T>>::type());  // tag dispatch
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto ObjectRegistry<TBASE, TALIAS>::Delete(const ID_t objectId) noexcept
{
    const auto lock = WriteLock{m_mutex};

    // Delete from ID map.
    const auto idFindIter = m_objectIdMap.find(objectId);
    if (idFindIter == m_objectIdMap.cend()) return false;  // not found

    m_objectIdMap.erase(idFindIter);

    // Delete from the type map (the mechanics guarantees it be found singly).
    for (auto iter = m_objectTypeMultiMap.cbegin(); iter != m_objectTypeMultiMap.cend();
         /* no iterator */)
    {
        if (iter->second->ID() == objectId)
        {
            m_objectTypeMultiMap.erase(iter);
            break;
        }

        else
            ++iter;
    }

    // If it has an alias, delete it from the other maps.
    const auto idToAliasFindIter = m_objectIdToAliasMap.find(objectId);
    if (idToAliasFindIter == m_objectIdToAliasMap.cend()) return true;

    const auto aliasToIdFindIter =
        m_objectAliasToIdMap.find(idToAliasFindIter->second);  // mechanics guarantees this be found
    KL_ASSERT(aliasToIdFindIter != m_objectAliasToIdMap.end(),
              "Failed to find entry in alias-to-ID map");

    m_objectAliasToIdMap.erase(aliasToIdFindIter);
    m_objectIdToAliasMap.erase(idToAliasFindIter);

    // KL_IF_DEBUG_MESSAGE(KL_LIGHT_GREY << m_printableBaseName << KL_NORMAL
    //                                   << " registry deleted object with ID " << KL_WHITE_BOLD
    //                                   << objectId);
    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline void ObjectRegistry<TBASE, TALIAS>::DeleteAll() noexcept
{
    const auto lock = WriteLock{m_mutex};
    m_objectIdMap.clear();
    m_objectTypeMultiMap.clear();
    m_objectAliasToIdMap.clear();
    m_objectIdToAliasMap.clear();

    // KL_IF_DEBUG_MESSAGE(KL_LIGHT_GREY << m_printableBaseName << KL_NORMAL
    //                                   << " registry deleted all objects");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename>
inline auto ObjectRegistry<TBASE, TALIAS>::GetAlias(TOBJECT &&object) const
{
    const auto lock = ReadLock{m_mutex};
    return this->GetAlias(object.ID());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::GetAlias(const ID_t objectId) const
{
    const auto lock = ReadLock{m_mutex};

    const auto findIter = m_objectIdToAliasMap.find(objectId);
    if (findIter != m_objectIdToAliasMap.cend()) return findIter->second;

    KL_THROW("Could not return object alias because the " << KL_WHITE_BOLD << m_printableBaseName
                                                          << KL_NORMAL << " object did not "
                                                          << "have one");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename>
inline auto ObjectRegistry<TBASE, TALIAS>::HasAlias(TOBJECT &&object) const noexcept
{
    const auto lock = ReadLock{m_mutex};
    return this->HasAliasImpl(object.ID());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto ObjectRegistry<TBASE, TALIAS>::HasAlias(const ID_t objectId) const noexcept
{
    const auto lock = ReadLock{m_mutex};
    return this->HasAliasImpl(objectId);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT, typename TTHISALIAS, typename>
inline void ObjectRegistry<TBASE, TALIAS>::AddAlias(TOBJECT &&object, TTHISALIAS &&alias)
{
    const auto lock = WriteLock{m_mutex};
    this->AddAliasImpl(object.ID(), std::forward<TALIAS>(alias));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TTHISALIAS>
inline void ObjectRegistry<TBASE, TALIAS>::AddAlias(const ID_t objectId, TTHISALIAS &&alias)
{
    const auto lock = WriteLock{m_mutex};
    this->AddAliasImpl(objectId, std::forward<TALIAS>(alias));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TDESIRED, typename T, typename>
inline auto ObjectRegistry<TBASE, TALIAS>::DoesObjectExist(T &&arg) const noexcept
{
    const auto lock = ReadLock{m_mutex};
    return this->DoesObjectExistImpl<std::decay_t<TDESIRED>>(
        std::forward<T>(arg),
        typename std::is_same<TALIAS_D, std::decay_t<T>>::type());  // tag dispatch
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TDESIRED>
inline auto ObjectRegistry<TBASE, TALIAS>::DoesObjectExist(const ID_t objectId) const noexcept
{
    const auto lock = ReadLock{m_mutex};

    const auto findIter = m_objectIdMap.find(objectId);
    if (findIter == m_objectIdMap.cend()) return false;

    return (std::dynamic_pointer_cast<std::decay_t<TDESIRED>>(findIter->second) != nullptr);
}
}  // namespace kl

#endif  // #ifndef KL_OBJECT_REGISTRY_IMPL_H