/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/ObjectAssociation.txx
 *
 * @brief Template implementation header file for the object association (ObjectAssociation) class
 * template, the object association base (ObjectAssociationBase) abstract class and the association
 * information (AssociationInformation) class.
 */

#ifndef KL_OBJECT_ASSOCIATION_IMPL_H
#define KL_OBJECT_ASSOCIATION_IMPL_H 1

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/types/base_class.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void ObjectAssociationBase::serialize(TARCHIVE &archive)
{
    archive(m_id, m_isCerealSerializable);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto &ObjectAssociation<TOBJECT, TINDICATOR>::Get() const
{
    if (const auto spObject = m_wpObject.lock()) return *spObject;

    KL_THROW("Could not get associated object because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetPrintableTypeName() const -> std::string
{
    if (const auto spObject = m_wpObject.lock()) return spObject->PrintableName();

    KL_THROW("Could not get associated object's type name because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetRegistryName() const -> std::string
{
    if (const auto spObject = m_wpObject.lock()) return spObject->GetRegistry().PrintableBaseName();

    KL_THROW("Could not get associated object's registry's name because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetIdentifierString() const -> std::string
{
    if (const auto spObject = m_wpObject.lock()) return spObject->GetIdentifierString();

    KL_THROW("Could not get associated object's identifier string because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::IsAlive() const noexcept -> bool
{
    return (m_wpObject.lock() != nullptr);
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::HasIndicator() const noexcept -> bool
{
    return m_hasIndicator;
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetIndicatorString() const -> std::string
{
    if (!m_hasIndicator) return std::string{};

    return this->GetIndicatorStringImpl(
        typename std::is_arithmetic<TINDICATOR_D>::type(),
        typename std::is_constructible<std::string, TINDICATOR_D>::type());
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
template <typename TTHIS>
ObjectAssociation<TOBJECT, TINDICATOR>::ObjectAssociation(TTHIS &&thisObject, TOBJECT &&assocObject,
                                                          const bool reciprocate)
    : ObjectAssociationBase{assocObject.ID(), (thisObject.IsCerealSerializable() &&
                                               assocObject.IsCerealSerializable())},
      m_wpObject{static_cast<TOBJECT_D_wPtr>(
          std::dynamic_pointer_cast<TOBJECT_D>(assocObject.GetSharedPointer()))},
      m_indicator{},
      m_hasIndicator{false}
{
    // Note that checking the objects are alive is to be done by `RegisteredObjectTemplate<TBASE,
    // TALIAS>::Associate` before instantiating an association object, so we can consider the
    // dynamic casting and getting shared pointer to be guaranteed to succeed.
    if (reciprocate)
        assocObject.template Associate<TOBJECT_D>(std::forward<TTHIS>(thisObject), false);
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
template <typename TTHIS>
ObjectAssociation<TOBJECT, TINDICATOR>::ObjectAssociation(TTHIS &&thisObject, TOBJECT &&assocObject,
                                                          const bool reciprocate,
                                                          TINDICATOR &&indicator)
    : ObjectAssociationBase{assocObject.ID(), (thisObject.IsCerealSerializable() &&
                                               assocObject.IsCerealSerializable())},
      m_wpObject{static_cast<TOBJECT_D_wPtr>(
          std::dynamic_pointer_cast<TOBJECT_D>(assocObject.GetSharedPointer()))},
      m_indicator{indicator},
      m_hasIndicator{true}
{
    // Note that checking the objects are alive is to be done by `RegisteredObjectTemplate<TBASE,
    // TALIAS>::Associate` before instantiating an association object, so we can consider the
    // dynamic casting and getting shared pointer to be guaranteed to succeed.
    if (reciprocate)
        assocObject.template Associate<TOBJECT_D>(std::forward<TTHIS>(thisObject),
                                                  std::forward<TINDICATOR>(indicator), false);
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetIndicatorStringImpl(std::true_type,
                                                                           std::true_type) const
    -> std::string
{
    return std::string{m_indicator};
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetIndicatorStringImpl(std::false_type,
                                                                           std::true_type) const
    -> std::string
{
    return std::string{m_indicator};
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetIndicatorStringImpl(std::true_type,
                                                                           std::false_type) const
    noexcept -> std::string
{
    return std::to_string(m_indicator);
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT, typename TINDICATOR>
inline auto ObjectAssociation<TOBJECT, TINDICATOR>::GetIndicatorStringImpl(std::false_type,
                                                                           std::false_type) const
    noexcept -> std::string
{
    return std::string{};
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TOBJECT, typename TINDICATOR>
template <typename TARCHIVE>
inline void ObjectAssociation<TOBJECT, TINDICATOR>::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<ObjectAssociationBase>(this), m_wpObject, m_indicator,
            m_hasIndicator);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TOBJECT, typename TINDICATOR>
ObjectAssociation<TOBJECT, TINDICATOR>::ObjectAssociation() noexcept
    : m_wpObject{}, m_indicator{}, m_hasIndicator{false}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void AssociationInformation::serialize(TARCHIVE &archive)
{
    archive(m_id, m_isAlive, m_isCerealSerializable, m_hasIndicator, m_identifierString, m_typeName,
            m_registryName, m_indicatorString);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_OBJECT_ASSOCIATION_IMPL_H