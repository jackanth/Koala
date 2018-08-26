/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Registry/HierarchicalEdge.h
 *
 * @brief Header file for the hierarchical edge base (HierarchicalEdgeBase), hierarchical edge
 * (HierarchicalEdge), and default edge (DefaultEdge) class templates.
 */

#ifndef KL_HIERARCHICAL_EDGE_H
#define KL_HIERARCHICAL_EDGE_H 1

#include "koala/Definitions.h"
#include "koala/Registry/HierarchicalPseudoEdge.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/polymorphic.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief Forward declaration of HierarchicalEdge class template.
 */
template <typename TA, typename TB, bool TC>
class HierarchicalEdge;

/**
 * @brief Forward declaration of HierarchicalObjectTemplate abstract class template.
 */
template <typename TA, typename TB>
class HierarchicalObjectTemplate;

/**
 * @brief HierarchicalEdgeBase class template.
 */
template <typename TBASE>
class HierarchicalEdgeBase : public std::enable_shared_from_this<HierarchicalEdgeBase<TBASE>>
{
public:
    /**
     * @brief Enum enumerating the different styles of edge.
     */
    enum class STYLE
    {
        SOLID,   ///< A solid edge.
        DASHED,  ///< A dashed edge.
        DOTTED,  ///< A dotted edge.
        BOLD     ///< A bold edge.
    };

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Copy constructor.
     *
     * @param other The object from which to copy-construct.
     */
    HierarchicalEdgeBase(const HierarchicalEdgeBase &other) noexcept;

    /**
     * @brief Move constructor.
     *
     * @param other The object from which to move-construct.
     */
    HierarchicalEdgeBase(HierarchicalEdgeBase &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The object from which to copy-assign.
     */
    HierarchicalEdgeBase &operator=(const HierarchicalEdgeBase &other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The object from which to move-assign.
     */
    HierarchicalEdgeBase &operator=(HierarchicalEdgeBase &&other) noexcept;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~HierarchicalEdgeBase() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Find out whether the parent object is of a given type.
     *
     * @return Whether the parent object is of a given type.
     */
    template <typename TOBJECT>
    auto ParentIsA() const;

    /**
     * @brief Find out whether the daughter object is of a given type.
     *
     * @return Whether the daughter object is of a given type.
     */
    template <typename TOBJECT>
    auto DaughterIsA() const;

    /**
     * @brief Get the parent object.
     *
     * @return The parent object.
     */
    template <typename TOBJECT = TBASE>
    auto &GetParent() const;

    /**
     * @brief Get the daughter object.
     *
     * @return The daughter object.
     */
    template <typename TOBJECT = TBASE>
    auto &GetDaughter() const;

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

public:
    // ATTN protect me.
    using wPtr = std::weak_ptr<HierarchicalEdgeBase>;  ///< Alias for a weak pointer.

protected:
    using TBASE_D = std::decay_t<TBASE>;  ///< Alias for the decayed base type.
    using TBASE_D_wPtr =
        std::weak_ptr<TBASE_D>;  ///< Alias for a weak pointer to the decayed base type.
    using sPtr = std::shared_ptr<HierarchicalEdgeBase>;  ///< Alias for a shared pointer.
    using sPtrSet = std::unordered_set<sPtr>;  ///< Alias for an unordered set of shared pointers.
    using PseudoEdgeBase = HierarchicalPseudoEdgeBase<TBASE_D>;  ///< Alias for a pseudo-edge.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Constructor.
     *
     * @param id The unique ID.
     * @param wpParentObject Weak pointer to the parent object.
     * @param wpDaughterObject Weak pointer to the daughter object.
     */
    HierarchicalEdgeBase(TBASE_D_wPtr wpParentObject, TBASE_D_wPtr wpDaughterObject) noexcept;

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Default constructor (for serialization purposes).
     */
    HierarchicalEdgeBase() noexcept;
#endif  // #ifdef KOALA_ENABLE_CEREAL

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the parent weak pointer.
     *
     * @return The parent weak pointer.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, ParentWeakPointer, m_wpParentObject, const, ReadLock,
                                    m_mutex);

    /**
     * @brief Get the daughter weak pointer.
     *
     * @return The daughter weak pointer.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, DaughterWeakPointer, m_wpDaughterObject, const, ReadLock,
                                    m_mutex);

    /**
     * @brief Set the parent weak pointer.
     *
     * @param wpParentObject The parent weak pointer.
     */
    KL_SIMPLE_MOVE_SETTER_LOCK(ParentWeakPointer, TBASE_D_wPtr, wpParentObject, m_wpParentObject,
                               WriteLock, m_mutex);

    /**
     * @brief Set the daughter weak pointer.
     *
     * @param wpDaughterObject The daughter weak pointer.
     */
    KL_SIMPLE_MOVE_SETTER_LOCK(DaughterWeakPointer, TBASE_D_wPtr, wpDaughterObject,
                               m_wpDaughterObject, WriteLock, m_mutex);

    /**
     * @brief Get the parent shared pointer.
     *
     * @return The parent shared pointer.
     */
    auto ParentSharedPointer() const;

    /**
     * @brief Get the daughter shared pointer.
     *
     * @return The daughter shared pointer.
     */
    auto DaughterSharedPointer() const;

    /**
     * @brief Get a string to represent the state of the object in a graph edge.
     *
     * @return A string to represent the state of the object in a graph edge.
     */
    virtual std::string GetGraphEdgeLabel() const;

    /**
     * @brief Get the graph edge style enum.
     *
     * @return The graph edge style enum.
     */
    virtual STYLE GetGraphEdgeStyle() const;

    /**
     * @brief Find out whether two edges are equivalent.
     *
     * @param other The other object.
     *
     * @return Whether they are equivalent.
     */
    virtual bool IsEquivalent(const HierarchicalEdgeBase &other) const;

    /**
     * @brief Create a pseudo-edge.
     *
     * @param wpObject Weak pointer to the object.
     * @param wpOwningObject Weak pointer to the owning object.
     *
     * @return Weak pointer to the new pseudo-edge.
     */
    virtual auto CreatePseudoEdge(TBASE_D_wPtr wpParent, TBASE_D_wPtr wpDaughter) ->
        typename PseudoEdgeBase::wPtr = 0;

    /**
     * @brief Clear all the pseudo edges.
     */
    void ClearPseudoEdges() noexcept;

    /**
     * @brief Find out whether this edge is inheritable.
     *
     * @return Whether it is inheritable.
     */
    virtual bool IsInheritable() const = 0;

    /**
     * @brief Add a pseudo-edge weak pointer.
     *
     * @param wpPseudoEdge The pseudo-edge weak pointer.
     */
    void AddPseudoEdgeWeakPointer(typename PseudoEdgeBase::wPtr wpPseudoEdge) noexcept;

    template <typename T>
    friend class HierarchicalVisualizationUtility;

    template <typename T>
    friend class HierarchicalPseudoEdgeBase;

    template <typename TA, typename TB>
    friend class HierarchicalObjectTemplate;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    mutable Mutex m_mutex;  ///< A mutex for locking this object during concurrent access.

    TBASE_D_wPtr m_wpParentObject;                   ///< Weak pointer to the parent object.
    TBASE_D_wPtr m_wpDaughterObject;                 ///< Weak pointer to the daughter object.
    typename PseudoEdgeBase::sPtrSet m_pseudoEdges;  ///< The pseudo-edges.

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
 * @brief HierarchicalEdge class template.
 */
template <typename TEDGE, typename TBASE, bool TINHERITABLE>
class HierarchicalEdge : public HierarchicalEdgeBase<TBASE>
{
public:
    /**
     * @brief Default copy constructor.
     */
    HierarchicalEdge(const HierarchicalEdge &) = default;

    /**
     * @brief Default move constructor.
     */
    HierarchicalEdge(HierarchicalEdge &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    HierarchicalEdge &operator=(const HierarchicalEdge &) = default;

    /**
     * @brief Default move assignment operator.
     */
    HierarchicalEdge &operator=(HierarchicalEdge &&) = default;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~HierarchicalEdge() = default;

protected:
    using TBASE_D = std::decay_t<TBASE>;             ///< Alias for the decayed base type.
    using wPtr = std::weak_ptr<HierarchicalEdge>;    ///< Alias for a weak pointer.
    using sPtr = std::shared_ptr<HierarchicalEdge>;  ///< Alias for a shared pointer.
    using sPtrSet = std::unordered_set<sPtr>;  ///< Alias for an unordered set of shared pointers.
    using PseudoEdge = HierarchicalPseudoEdge<TBASE_D, TEDGE>;  ///< Alias for a pseudo-edge.

    //----------------------------------------------------------------------------------------------

    using HierarchicalEdgeBase<TBASE>::HierarchicalEdgeBase;  ///< Use base class constructor.

    /**
     * @brief Default constructor (for serialization purposes).
     */
    HierarchicalEdge() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Find out whether this edge is inheritable.
     *
     * @return Whether it is inheritable.
     */
    constexpr bool IsInheritable() const final;

    /**
     * @brief Create a pseudo-edge.
     *
     * @param wpObject Weak pointer to the object.
     * @param wpOwningObject Weak pointer to the owning object.
     *
     * @return Weak pointer to the new pseudo-edge.
     */
    auto CreatePseudoEdge(typename HierarchicalEdgeBase<TBASE>::TBASE_D_wPtr wpParent,
                          typename HierarchicalEdgeBase<TBASE>::TBASE_D_wPtr wpDaughter) ->
        typename HierarchicalEdgeBase<TBASE>::PseudoEdgeBase::wPtr final;

    template <typename TA, typename TB>
    friend class HierarchicalObjectTemplate;

    template <typename T>
    friend class HierarchicalVisualizationUtility;

    template <typename T>
    friend class HierarchicalPseudoEdgeBase;

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

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief DefaultEdge class template.
 */
template <typename TBASE>
class DefaultEdge : public HierarchicalEdge<DefaultEdge<TBASE>, std::decay_t<TBASE>, true>
{
public:
    /**
     * @brief Default copy constructor.
     */
    DefaultEdge(const DefaultEdge &) = default;

    /**
     * @brief Default move constructor.
     */
    DefaultEdge(DefaultEdge &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    DefaultEdge &operator=(const DefaultEdge &) = default;

    /**
     * @brief Default move assignment operator.
     */
    DefaultEdge &operator=(DefaultEdge &&) = default;

    /**
     * @brief Default destructor.
     */
    ~DefaultEdge() = default;

protected:
    using HierarchicalEdge<DefaultEdge, std::decay_t<TBASE>,
                           true>::HierarchicalEdge;  ///< Use base class constructor.

    /**
     * @brief Default constructor (for serialization purposes).
     */
    DefaultEdge() = default;

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

#include "koala/Registry/HierarchicalEdge.txx"

#endif  // #ifndef KL_HIERARCHICAL_EDGE_H