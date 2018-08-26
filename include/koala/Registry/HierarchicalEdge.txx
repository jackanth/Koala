/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/HierarchicalEdge.txx
 *
 * @brief Template implementation header file for the hierarchical edge base
 * (HierarchicalEdgeBase), hierarchical edge (HierarchicalEdge), and default edge (DefaultEdge)
 * class templates.
 */

#ifndef KL_HIERARCHICAL_EDGE_IMPL_H
#define KL_HIERARCHICAL_EDGE_IMPL_H 1

namespace kl
{
template <typename TBASE>
HierarchicalEdgeBase<TBASE>::HierarchicalEdgeBase(const HierarchicalEdgeBase &other) noexcept
    : std::enable_shared_from_this<HierarchicalEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpParentObject{},
      m_wpDaughterObject{},
      m_pseudoEdges{}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = ReadLock{other.m_mutex};

    m_wpParentObject = other.m_wpParentObject;
    m_wpDaughterObject = other.m_wpDaughterObject;
    m_pseudoEdges = other.m_pseudoEdges;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalEdgeBase<TBASE>::HierarchicalEdgeBase(HierarchicalEdgeBase &&other) noexcept
    : std::enable_shared_from_this<HierarchicalEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpParentObject{},
      m_wpDaughterObject{},
      m_pseudoEdges{}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = WriteLock{other.m_mutex};

    m_wpParentObject = std::move_if_noexcept(other.m_wpParentObject);
    m_wpDaughterObject = std::move_if_noexcept(other.m_wpDaughterObject);
    m_pseudoEdges = std::move_if_noexcept(other.m_pseudoEdges);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalEdgeBase<TBASE> &HierarchicalEdgeBase<TBASE>::operator=(
    const HierarchicalEdgeBase &other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = ReadLock{other.m_mutex, std::defer_lock};
        kl::Lock<>::DoLock(thisLock, otherLock);

        std::enable_shared_from_this<HierarchicalEdgeBase<TBASE>>::operator=(other);

        m_wpParentObject = other.m_wpParentObject;
        m_wpDaughterObject = other.m_wpDaughterObject;
        m_pseudoEdges = other.m_pseudoEdges;
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalEdgeBase<TBASE> &HierarchicalEdgeBase<TBASE>::operator=(
    HierarchicalEdgeBase &&other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = WriteLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        std::enable_shared_from_this<HierarchicalEdgeBase<TBASE>>::operator=(other);

        m_wpParentObject = std::move_if_noexcept(other.m_wpParentObject);
        m_wpDaughterObject = std::move_if_noexcept(other.m_wpDaughterObject);
        m_pseudoEdges = std::move_if_noexcept(other.m_pseudoEdges);
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
inline auto HierarchicalEdgeBase<TBASE>::ParentIsA() const
{
    if (const auto spParentObject = m_wpParentObject.lock())
    {
        if (std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spParentObject)) return true;

        return false;
    }

    KL_THROW("Could not get parent object of hierarchical edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
inline auto HierarchicalEdgeBase<TBASE>::DaughterIsA() const
{
    if (const auto spDaughterObject = m_wpDaughterObject.lock())
    {
        if (std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spDaughterObject)) return true;

        return false;
    }

    KL_THROW("Could not get daughter object of hierarchical edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
inline auto &HierarchicalEdgeBase<TBASE>::GetParent() const
{
    if (const auto spParentObject = m_wpParentObject.lock())
    {
        if (const auto spCastParentObject =
                std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spParentObject))
            return *spCastParentObject;

        KL_THROW(
            "Could not get parent object of hierarchical edge because it was not of the correct "
            "type");
    }

    KL_THROW("Could not get parent object of hierarchical edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
inline auto &HierarchicalEdgeBase<TBASE>::GetDaughter() const
{
    if (const auto spDaughterObject = m_wpDaughterObject.lock())
    {
        if (const auto spCastDaughterObject =
                std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(spDaughterObject))
            return *spCastDaughterObject;

        KL_THROW(
            "Could not get parent object of hierarchical edge because it was not of the correct "
            "type");
    }

    KL_THROW("Could not get parent object of hierarchical edge because it was no longer alive");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
auto HierarchicalEdgeBase<TBASE>::CanCastAs() const
{
    try
    {
        return (std::dynamic_pointer_cast<const std::decay_t<TOBJECT>>(this->shared_from_this()) !=
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

template <typename TBASE>
template <typename TOBJECT>
auto &HierarchicalEdgeBase<TBASE>::CastAs()
{
    try
    {
        if (const auto spCastObject =
                std::dynamic_pointer_cast<std::decay_t<TOBJECT>>(this->shared_from_this()))
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

    KL_THROW("Could not perform cast for hierarchical edge");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT>
const auto &HierarchicalEdgeBase<TBASE>::CastAs() const
{
    try
    {
        if (const auto spCastObject =
                std::dynamic_pointer_cast<const std::decay_t<TOBJECT>>(this->shared_from_this()))
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

    KL_THROW("Could not perform cast for hierarchical edge");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
HierarchicalEdgeBase<TBASE>::HierarchicalEdgeBase(TBASE_D_wPtr wpParentObject,
                                                  TBASE_D_wPtr wpDaughterObject) noexcept
    : std::enable_shared_from_this<HierarchicalEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpParentObject{std::move_if_noexcept(wpParentObject)},
      m_wpDaughterObject{std::move_if_noexcept(wpDaughterObject)},
      m_pseudoEdges{}
{
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE>
template <typename TARCHIVE>
inline void HierarchicalEdgeBase<TBASE>::serialize(TARCHIVE &archive)
{
    archive(m_wpParentObject, m_wpDaughterObject, m_pseudoEdges);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE>
HierarchicalEdgeBase<TBASE>::HierarchicalEdgeBase() noexcept
    : std::enable_shared_from_this<HierarchicalEdgeBase<TBASE>>{},
      m_mutex{},
      m_wpParentObject{},
      m_wpDaughterObject{},
      m_pseudoEdges{}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline auto HierarchicalEdgeBase<TBASE>::ParentSharedPointer() const
{
    if (const auto spParent = m_wpParentObject.lock()) return spParent;

    KL_THROW("Could not lock edge parent object");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline auto HierarchicalEdgeBase<TBASE>::DaughterSharedPointer() const
{
    if (const auto spDaughter = m_wpDaughterObject.lock()) return spDaughter;

    KL_THROW("Could not lock edge daughter object");
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline std::string HierarchicalEdgeBase<TBASE>::GetGraphEdgeLabel() const
{
    return {};
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline bool HierarchicalEdgeBase<TBASE>::IsEquivalent(const HierarchicalEdgeBase &) const
{
    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline void HierarchicalEdgeBase<TBASE>::ClearPseudoEdges() noexcept
{
    m_pseudoEdges.clear();
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline void HierarchicalEdgeBase<TBASE>::AddPseudoEdgeWeakPointer(
    typename PseudoEdgeBase::wPtr wpPseudoEdge) noexcept
{
    m_pseudoEdges.emplace(std::move_if_noexcept(wpPseudoEdge));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline typename HierarchicalEdgeBase<TBASE>::STYLE HierarchicalEdgeBase<TBASE>::GetGraphEdgeStyle()
    const
{
    return STYLE::SOLID;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TEDGE, typename TBASE, bool TINHERITABLE>
inline constexpr bool HierarchicalEdge<TEDGE, TBASE, TINHERITABLE>::IsInheritable() const
{
    return TINHERITABLE;
}

//--------------------------------------------------------------------------------------------------

template <typename TEDGE, typename TBASE, bool TINHERITABLE>
inline auto HierarchicalEdge<TEDGE, TBASE, TINHERITABLE>::CreatePseudoEdge(
    typename HierarchicalEdgeBase<TBASE>::TBASE_D_wPtr wpObject,
    typename HierarchicalEdgeBase<TBASE>::TBASE_D_wPtr wpOwningObject) ->
    typename HierarchicalEdgeBase<TBASE>::PseudoEdgeBase::wPtr
{
    const auto spPseudoEdge = typename PseudoEdge::sPtr{new PseudoEdge{
        wpObject, wpOwningObject,
        static_cast<typename HierarchicalEdgeBase<TBASE>::wPtr>(this->shared_from_this())}};

    this->AddPseudoEdgeWeakPointer(spPseudoEdge);
    return static_cast<typename PseudoEdge::wPtr>(spPseudoEdge);
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TEDGE, typename TBASE, bool TINHERITABLE>
template <typename TARCHIVE>
inline void HierarchicalEdge<TEDGE, TBASE, TINHERITABLE>::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<HierarchicalEdgeBase<TBASE>>(this));
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE>
template <typename TARCHIVE>
inline void DefaultEdge<TBASE>::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<HierarchicalEdge<DefaultEdge, std::decay_t<TBASE>, true>>(this));
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_HIERARCHICAL_EDGE_IMPL_H