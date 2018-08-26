/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/HierarchicalPseudoEdge.txx
 *
 * @brief Template implementation header file for the hierarchical pseudo-edge base
 * (HierarchicalPseudoEdgeBase) class template.
 */

#ifndef KL_HIERARCHICAL_PSEUDO_EDGE_IMPL_H
#define KL_HIERARCHICAL_PSEUDO_EDGE_IMPL_H 1

namespace kl
{
template <typename TBASE>
HierarchicalPseudoEdgeBase<TBASE>::HierarchicalPseudoEdgeBase(
    const HierarchicalPseudoEdgeBase &other) noexcept
    : std::enable_shared_from_this<HierarchicalPseudoEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpObject{},
      m_wpOwningObject{},
      m_wpUnderlyingEdge{}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = ReadLock{other.m_mutex};

    m_wpObject = other.m_wpObject;
    m_wpOwningObject = other.m_wpOwningObject;
    m_wpUnderlyingEdge = other.m_wpUnderlyingEdge;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalPseudoEdgeBase<TBASE>::HierarchicalPseudoEdgeBase(
    HierarchicalPseudoEdgeBase &&other) noexcept
    : std::enable_shared_from_this<HierarchicalPseudoEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpObject{},
      m_wpOwningObject{},
      m_wpUnderlyingEdge{}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = WriteLock{other.m_mutex};

    m_wpObject = std::move_if_noexcept(other.m_wpObject);
    m_wpOwningObject = std::move_if_noexcept(other.m_wpOwningObject);
    m_wpUnderlyingEdge = std::move_if_noexcept(other.m_wpUnderlyingEdge);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalPseudoEdgeBase<TBASE> &HierarchicalPseudoEdgeBase<TBASE>::operator=(
    const HierarchicalPseudoEdgeBase &other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = ReadLock{other.m_mutex, std::defer_lock};
        kl::Lock<>::DoLock(thisLock, otherLock);

        std::enable_shared_from_this<HierarchicalPseudoEdgeBase<TBASE>>::operator=(other);

        auto tmp = HierarchicalPseudoEdgeBase{other};
        *this = std::move_if_noexcept(tmp);
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalPseudoEdgeBase<TBASE> &HierarchicalPseudoEdgeBase<TBASE>::operator=(
    HierarchicalPseudoEdgeBase &&other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = WriteLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        std::enable_shared_from_this<HierarchicalPseudoEdgeBase<TBASE>>::operator=(other);

        m_wpObject = std::move_if_noexcept(other.m_wpObject);
        m_wpOwningObject = std::move_if_noexcept(other.m_wpOwningObject);
        m_wpUnderlyingEdge = std::move_if_noexcept(other.m_wpUnderlyingEdge);
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
inline auto HierarchicalPseudoEdgeBase<TBASE>::ObjectIsA() const
{
    if (const auto spObject = m_wpObject.lock())
    {
        if (std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spObject)) return true;

        return false;
    }

    KL_THROW("Could not get object of hierarchical pseudo-edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
inline auto &HierarchicalPseudoEdgeBase<TBASE>::GetObject() const
{
    if (const auto spObject = m_wpObject.lock())
    {
        if (const auto spCastObject = std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spObject))
            return *spCastObject;

        KL_THROW(
            "Could not get object of hierarchical pseudo-edge because it was not of the correct "
            "type");
    }

    KL_THROW("Could not get object of hierarchical pseudo-edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TEDGE>
inline auto HierarchicalPseudoEdgeBase<TBASE>::UnderlyingEdgeIsA() const
{
    if (const auto spEdge = m_wpUnderlyingEdge.lock())
    {
        if (std::dynamic_pointer_cast<std::decay_t<TEDGE>>(spEdge)) return true;

        return false;
    }

    KL_THROW(
        "Could not get underlying edge of hierarchical pseudo-edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TEDGE>
inline auto &HierarchicalPseudoEdgeBase<TBASE>::GetUnderlyingEdge() const
{
    if (const auto spEdge = m_wpUnderlyingEdge.lock())
    {
        if (const auto spCastEdge = std::dynamic_pointer_cast<std::decay_t<TEDGE>>(spEdge))
            return *spCastEdge;

        KL_THROW(
            "Could not get underlying edge of hierarchical pseudo-edge because it was not of the "
            "correct type");
    }

    KL_THROW(
        "Could not get underlying edge of hierarchical pseudo-edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalPseudoEdgeBase<TBASE>::HierarchicalPseudoEdgeBase(
    TBASE_D_wPtr wpObject, TBASE_D_wPtr wpOwningObject,
    typename Edge::wPtr wpUnderlyingEdge) noexcept
    : std::enable_shared_from_this<HierarchicalPseudoEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpObject{std::move_if_noexcept(wpObject)},
      m_wpOwningObject{std::move_if_noexcept(wpOwningObject)},
      m_wpUnderlyingEdge{std::move_if_noexcept(wpUnderlyingEdge)}
{
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline auto HierarchicalPseudoEdgeBase<TBASE>::ObjectSharedPointer() const
{
    if (const auto spObject = m_wpObject.lock()) return spObject;

    KL_THROW("Could not lock pseudo-edge object");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline auto HierarchicalPseudoEdgeBase<TBASE>::OwningObjectSharedPointer() const
{
    if (const auto spOwningObject = m_wpOwningObject.lock()) return spOwningObject;

    KL_THROW("Could not lock pseudo-edge owning object");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline auto HierarchicalPseudoEdgeBase<TBASE>::UnderlyingEdgeSharedPointer() const
{
    if (const auto spUnderlyingEdge = m_wpUnderlyingEdge.lock()) return spUnderlyingEdge;

    KL_THROW("Could not lock pseudo-edge underlying edge");
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE>
template <typename TARCHIVE>
inline void HierarchicalPseudoEdgeBase<TBASE>::serialize(TARCHIVE &archive)
{
    archive(m_wpObject, m_wpOwningObject, m_wpUnderlyingEdge);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE>
HierarchicalPseudoEdgeBase<TBASE>::HierarchicalPseudoEdgeBase() noexcept
    : std::enable_shared_from_this<HierarchicalPseudoEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpObject{},
      m_wpOwningObject{},
      m_wpUnderlyingEdge{}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TEDGE>
inline auto &HierarchicalPseudoEdge<TBASE, TEDGE>::GetUnderlyingEdge() const
{
    return HierarchicalPseudoEdgeBase<TBASE>::template GetUnderlyingEdge<std::decay_t<TEDGE>>();
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TEDGE>
template <typename TARCHIVE>
inline void HierarchicalPseudoEdge<TBASE, TEDGE>::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<HierarchicalPseudoEdgeBase<TBASE>>(this));
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_HIERARCHICAL_PSEUDO_EDGE_IMPL_H