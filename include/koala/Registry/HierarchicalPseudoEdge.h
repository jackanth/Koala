/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/HierarchicalPseudoEdge.h
 *
 * @brief Header file for the hierarchical pseudo-edge base (HierarchicalPseudoEdgeBase) class
 * template.
 */

#ifndef KL_HIERARCHICAL_PSEUDO_EDGE_H
#define KL_HIERARCHICAL_PSEUDO_EDGE_H 1

#include "koala/Definitions.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/polymorphic.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief Forward declaration of HierarchicalEdgeBase class template.
 */
template <typename T>
class HierarchicalEdgeBase;

/**
 * @brief HierarchicalPseudoEdgeBase class template.
 */
template <typename TBASE>
class HierarchicalPseudoEdgeBase
    : public std::enable_shared_from_this<HierarchicalPseudoEdgeBase<TBASE>>
{
protected:
    using TBASE_D = std::decay_t<TBASE>;  ///< Alias for the decayed base type.
    using TBASE_D_wPtr =
        std::weak_ptr<TBASE_D>;  ///< Alias for a weak pointer to the decayed base type.
    using sPtr = std::shared_ptr<HierarchicalPseudoEdgeBase>;  ///< Alias for a shared pointer.
    using sPtrSet = std::unordered_set<sPtr>;  ///< Alias for an unordered set of shared pointers.
    using wPtr = std::weak_ptr<HierarchicalPseudoEdgeBase>;  ///< Alias for a weak pointer.
    using wPtrSet = std::set<wPtr, std::owner_less<wPtr>>;   ///< Alias for a set of weak pointers.
    using Edge = HierarchicalEdgeBase<TBASE_D>;              ///< Alias for a hierarchical edge.

public:
    /**
     * @brief Copy constructor.
     *
     * @param other The object from which to copy-construct.
     */
    HierarchicalPseudoEdgeBase(const HierarchicalPseudoEdgeBase &other) noexcept;

    /**
     * @brief Move constructor.
     *
     * @param other The object from which to move-construct.
     */
    HierarchicalPseudoEdgeBase(HierarchicalPseudoEdgeBase &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The object from which to copy-assign.
     */
    HierarchicalPseudoEdgeBase &operator=(const HierarchicalPseudoEdgeBase &other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The object from which to move-assign.
     */
    HierarchicalPseudoEdgeBase &operator=(HierarchicalPseudoEdgeBase &&other) noexcept;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~HierarchicalPseudoEdgeBase() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Find out whether the object is of a given type.
     *
     * @return Whether the object is of a given type.
     */
    template <typename TOBJECT>
    auto ObjectIsA() const;

    /**
     * @brief Get the object.
     *
     * @return The object.
     */
    template <typename TOBJECT = TBASE>
    auto &GetObject() const;

    /**
     * @brief Find out whether the underlying edge is of a given type.
     *
     * @return Whether the underlying edge is of a given type.
     */
    template <typename TEDGE>
    auto UnderlyingEdgeIsA() const;

    /**
     * @brief Get the underlying edge.
     *
     * @return The underlying edge.
     */
    template <typename TEDGE = Edge>
    auto &GetUnderlyingEdge() const;

protected:
    /**
     * @brief Constructor.
     *
     * @param wpObject Weak pointer to the object.
     * @param wpOwningObject Weak pointer to the owning object.
     * @param wpUnderlyingEdge Weak pointer to the underlying edge.
     */
    HierarchicalPseudoEdgeBase(TBASE_D_wPtr wpObject, TBASE_D_wPtr wpOwningObject,
                               typename Edge::wPtr wpUnderlyingEdge) noexcept;

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Default constructor (for serialization purposes).
     */
    HierarchicalPseudoEdgeBase() noexcept;
#endif  // #ifdef KOALA_ENABLE_CEREAL

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the object weak pointer.
     *
     * @return The object weak pointer.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, ObjectWeakPointer, m_wpObject, const, ReadLock, m_mutex);

    /**
     * @brief Get the owning object weak pointer.
     *
     * @return The owning object weak pointer.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, OwningObjectWeakPointer, m_wpOwningObject, const);

    /**
     * @brief Set the object weak pointer.
     *
     * @param wpObject The object weak pointer.
     */
    KL_SIMPLE_MOVE_SETTER_LOCK(ObjectWeakPointer, TBASE_D_wPtr, wpObject, m_wpObject, WriteLock,
                               m_mutex);

    /**
     * @brief The underlying edge weak pointer.
     *
     * @return The underlying edge weak pointer.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, UnderlyingEdgeWeakPointer, m_wpUnderlyingEdge, const);

    /**
     * @brief Get the object shared pointer.
     *
     * @return The object shared pointer.
     */
    auto ObjectSharedPointer() const;

    /**
     * @brief Get the owning object shared pointer.
     *
     * @return The owning object shared pointer.
     */
    auto OwningObjectSharedPointer() const;

    /**
     * @brief The underlying edge shared pointer.
     *
     * @return The underlying edge shared pointer.
     */
    auto UnderlyingEdgeSharedPointer() const;

    template <typename TA, typename TB>
    friend class HierarchicalObjectTemplate;

    template <typename TA, typename TB, bool TC>
    friend class HierarchicalEdge;

    template <typename T>
    friend class HierarchicalEdgeBase;

    template <typename T>
    friend class HierarchicalVisualizationUtility;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    mutable Mutex m_mutex;  ///< A mutex for locking this object during concurrent access.

    TBASE_D_wPtr m_wpObject;                 ///< Weak pointer to the object.
    TBASE_D_wPtr m_wpOwningObject;           ///< Weak pointer to the owning object.
    typename Edge::wPtr m_wpUnderlyingEdge;  ///< Weak pointer to the underlying edge.

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief HierarchicalPseudoEdge class template.
 */
template <typename TBASE, typename TEDGE>
class HierarchicalPseudoEdge : public HierarchicalPseudoEdgeBase<TBASE>
{
public:
    /**
     * @brief Default copy constructor.
     */
    HierarchicalPseudoEdge(const HierarchicalPseudoEdge &) = default;

    /**
     * @brief Default move constructor.
     */
    HierarchicalPseudoEdge(HierarchicalPseudoEdge &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    HierarchicalPseudoEdge &operator=(const HierarchicalPseudoEdge &) = default;

    /**
     * @brief Default move assignment operator.
     */
    HierarchicalPseudoEdge &operator=(HierarchicalPseudoEdge &&) = default;

    /**
     * @brief Default destructor.
     */
    ~HierarchicalPseudoEdge() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the underlying edge.
     *
     * @return The underlying edge.
     */
    auto &GetUnderlyingEdge() const;

protected:
    using sPtr = std::shared_ptr<HierarchicalPseudoEdge>;  ///< Alias for a shared pointer.
    using sPtrSet = std::unordered_set<sPtr>;  ///< Alias for an unordered set of shared pointers.
    using wPtr = std::weak_ptr<HierarchicalPseudoEdge>;     ///< Alias for a weak pointer.
    using wPtrSet = std::set<wPtr, std::owner_less<wPtr>>;  ///< Alias for a set of weak pointers.

    //----------------------------------------------------------------------------------------------

    using HierarchicalPseudoEdgeBase<TBASE>::HierarchicalPseudoEdgeBase;  ///< Use base class
                                                                          ///< constructor.

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Default constructor (for serialization purposes).
     */
    HierarchicalPseudoEdge() = default;
#endif  // #ifdef KOALA_ENABLE_CEREAL

    template <typename TA, typename TB>
    friend class HierarchicalObjectTemplate;

    template <typename TA, typename TB, bool TC>
    friend class HierarchicalEdge;

    template <typename T>
    friend class HierarchicalEdgeBase;

    template <typename T>
    friend class HierarchicalVisualizationUtility;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);
#endif  // #ifdef KOALA_ENABLE_CEREAL
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Registry/HierarchicalPseudoEdge.txx"

#endif  // #ifndef KL_HIERARCHICAL_PSEUDO_EDGE_H