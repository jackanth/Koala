/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Templates/HierarchicalObjectTemplate.h
 *
 * @brief Header file for the hierarchical object (HierarchicalObjectTemplate) abstract class
 * template.
 */

#ifndef KL_HIERARCHICAL_OBJECT_TEMPLATE_H
#define KL_HIERARCHICAL_OBJECT_TEMPLATE_H

#include "koala/RangeBasedContainer.h"
#include "koala/Registry/HierarchicalEdge.h"
#include "koala/Registry/HierarchicalPseudoEdge.h"
#include "koala/Templates/RegisteredObjectTemplate.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/base_class.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief Forward declaration of the HierarchicalVisualizationUtility class template.
 */
template <typename T>
class HierarchicalVisualizationUtility;

/**
 * @brief HierarchicalObjectTemplate abstract class template.
 */
template <typename TBASE, typename TALIAS = std::string>
class HierarchicalObjectTemplate : public RegisteredObjectTemplate<TBASE, TALIAS>
{
private:
    using TBASE_D = std::decay_t<TBASE>;    ///< Alias for decayed TBASE type.
    using TALIAS_D = std::decay_t<TALIAS>;  ///< Alias for decayed TALIAS type.

public:
    using HierarchicalObject =
        HierarchicalObjectTemplate<TBASE, TALIAS>;  ///< Alias for HierarchicalObject class.
    using RegisteredObject =
        typename HierarchicalObject::RegisteredObject;     ///< Alias for RegisteredObject class.
    using Registry = typename RegisteredObject::Registry;  ///< Alias for registry class.

    /**
     * @brief Copy constructor.
     *
     * @param other The object from which to copy-construct.
     */
    HierarchicalObjectTemplate(const HierarchicalObjectTemplate &other) noexcept;

    /**
     * @brief Move constructor.
     *
     * @param other The object from which to move-construct.
     */
    HierarchicalObjectTemplate(HierarchicalObjectTemplate &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The object from which to copy-assign.
     */
    auto &operator=(const HierarchicalObjectTemplate &other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The object from which to move-assign.
     */
    auto &operator=(HierarchicalObjectTemplate &&other) noexcept;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~HierarchicalObjectTemplate() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the range-based container for getting contained objects.
     *
     * @return The range-based container for getting contained objects.
     */
    template <typename TOBJECT = TBASE_D>
    auto Contained() const;

    /**
     * @brief Find out whether there is a containing object (or a containing object of a specific
     * type).
     *
     * @return Whether there exists a containing object of this type.
     */
    template <typename TOBJECT = TBASE_D>
    auto HasContainingObjectOfType() const noexcept;

    /**
     * @brief Get the containing object.
     *
     * @return The containing object.
     */
    template <typename TOBJECT = TBASE_D>
    auto &Containing() const;

    /**
     * @brief Subsume an object into this object.
     *
     * @param arg The contained object, contained object alias or contained object ID.
     */
    template <typename T,
              typename = std::enable_if_t<std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                                          std::is_base_of<TBASE_D, std::decay_t<T>>::value ||
                                          std::is_same<ID_t, std::decay_t<T>>::value>>
    void Subsume(T &&arg);

    /**
     * @brief Subsume a set of objects into this object.
     *
     * @param objectSetToSubsume The set of contained objects, contained object aliases or
     * contained object IDs.
     */
    template <typename TSET>
    void SubsumeSet(TSET &&objectSetToSubsume);

    /**
     * @brief Subsume sets of objects into this object.
     *
     * @param objectSetsToSubsume The sets of contained objects, contained object aliases or
     * contained object IDs.
     */
    template <typename... TSETS>
    void SubsumeSets(TSETS &&... objectSetsToSubsume);

    /**
     * @brief Find out whether this object encloses another object.
     *
     * @param arg The object, object alias or object ID.
     */
    template <typename T,
              typename = std::enable_if_t<std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                                          std::is_base_of<TBASE_D, std::decay_t<T>>::value ||
                                          std::is_same<ID_t, std::decay_t<T>>::value>>
    auto Encloses(T &&arg) const;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the range-based container for getting daughters.
     *
     * @return The range-based container for getting daughters.
     */
    template <typename TOBJECT = TBASE_D>
    auto Daughters() const;

    /**
     * @brief Get the range-based container for getting daughter edges.
     *
     * @return The range-based container for getting daughter edges.
     */
    template <typename TEDGE = DefaultEdge<TBASE_D>,
              typename = std::enable_if_t<
                  std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, true>,
                                  std::decay_t<TEDGE>>::value ||
                  std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, false>,
                                  std::decay_t<TEDGE>>::value>>
    auto DaughterEdges() const;

    /**
     * @brief Add a new daughter edge.
     *
     * @param arg The daughter, daughter alias or daughter ID.
     * @param args The arguments to pass to the edge constructor.
     *
     * @return The new edge.
     */
    template <typename TEDGE = DefaultEdge<TBASE_D>, typename T, typename... TARGS,
              typename = std::enable_if_t<
                  (std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                   std::is_base_of<TBASE_D, std::decay_t<T>>::value ||
                   std::is_same<ID_t, std::decay_t<T>>::value) &&
                  (std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, true>,
                                   std::decay_t<TEDGE>>::value ||
                   std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, false>,
                                   std::decay_t<TEDGE>>::value)>>
    auto &AddDaughterEdge(T &&arg, TARGS &&... args);

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the range-based container for getting parents.
     *
     * @return The range-based container for getting parents.
     */
    template <typename TOBJECT = TBASE_D>
    auto Parents() const;

    /**
     * @brief Get the range-based container for getting parent edges.
     *
     * @return The range-based container for getting parent edges.
     */
    template <typename TEDGE = DefaultEdge<TBASE_D>,
              typename = std::enable_if_t<
                  std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, true>,
                                  std::decay_t<TEDGE>>::value ||
                  std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, false>,
                                  std::decay_t<TEDGE>>::value>>
    auto ParentEdges() const;

    /**
     * @brief Add a new parent edge.
     *
     * @param arg The parent, parent alias or parent ID.
     * @param args The arguments to pass to the edge constructor.
     *
     * @return The new edge.
     */
    template <typename TEDGE = DefaultEdge<TBASE_D>, typename T, typename... TARGS,
              typename = std::enable_if_t<
                  (std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                   std::is_base_of<TBASE_D, std::decay_t<T>>::value ||
                   std::is_same<ID_t, std::decay_t<T>>::value) &&
                  (std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, true>,
                                   std::decay_t<TEDGE>>::value ||
                   std::is_base_of<HierarchicalEdge<std::decay_t<TEDGE>, TBASE_D, false>,
                                   std::decay_t<TEDGE>>::value)>>
    auto &AddParentEdge(T &&arg, TARGS &&... args);

protected:
    using Koala_wPtr = typename RegisteredObject::Koala_wPtr;  ///< Alias for a weak pointer to
                                                               ///< the instance of Koala.
    using Registry_wPtr =
        typename RegisteredObject::Registry_wPtr;  ///< Alias for a weak pointer to the registry.

    using TBASE_wPtr = std::weak_ptr<TBASE_D>;  ///< Alias for a weak pointer to the object.
    using TBASE_wPtr_const =
        std::weak_ptr<const TBASE_D>;             ///< Alias for a const weak pointer to the object.
    using TBASE_sPtr = std::shared_ptr<TBASE_D>;  ///< Alias for a shared pointer to the object.
    using TBASE_wPtrSet =
        std::set<TBASE_wPtr,
                 std::owner_less<TBASE_wPtr>>;  ///< Alias for a set of weak pointers to the object.
    using TBASE_wPtrVector =
        std::vector<TBASE_wPtr>;  ///< Alias for a vector of weak ptrs to the object.
    using TBASE_sPtrSet =
        std::unordered_set<TBASE_sPtr>;  ///< Alias for a set of shared pointers to the object.
    using wPtr =
        std::weak_ptr<HierarchicalObjectTemplate>;  ///< Alias for weak pointer to this object.
    using wPtrSet = std::set<wPtr, std::owner_less<wPtr>>;  ///< Alias for a set of weak pointers to
                                                            ///< this object.

    using Edge = HierarchicalEdgeBase<TBASE_D>;                  ///< Alias for an edge base.
    using PseudoEdgeBase = HierarchicalPseudoEdgeBase<TBASE_D>;  ///< Alias for a pseudo-edge base.
    using Edge_sPtr = typename Edge::sPtr;        ///< Alias for shared pointer to an edge.
    using Edge_sPtrSet = typename Edge::sPtrSet;  ///< Alias for set of shared pointers to edges.
    using PseudoEdgeBase_wPtr =
        typename PseudoEdgeBase::wPtr;  ///< Alias for weak ptr to a pseudo-edge base.
    using PseudoEdgeBase_wPtrSet =
        typename PseudoEdgeBase::wPtrSet;  ///< Alias for set of weak ptrs to pseudo-edge bases.
    using PseudoEdgeBase_sPtr =
        typename PseudoEdgeBase::sPtr;  ///< Alias for a shared pointer to a pseudo-edge base.
    using MemberEdgeSetGetter = std::function<PseudoEdgeBase_wPtrSet &(
        const TBASE_wPtr)>;  ///< Alias for an edge set getter.

    template <typename TEDGE>
    using PseudoEdge =
        HierarchicalPseudoEdge<TBASE_D, std::decay_t<TEDGE>>;  ///< Alias for a pseudo-edge.

    /**
     * @brief Constructor.
     *
     * @param wpRegistry Weak pointer to the associated registry.
     * @param id Unique ID for the object.
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    HierarchicalObjectTemplate(Registry_wPtr wpRegistry, const ID_t id, Koala_wPtr wpKoala) noexcept;

    /**
     * @brief Default constructor (for serialization purposes and for constructing base for
     * uninitialized Koala).
     */
    HierarchicalObjectTemplate() noexcept;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a string to represent the state of the object in a graph node.
     *
     * @return A string to represent the state of the object in a graph node.
     */
    virtual std::string GetGraphNodeLabel() const;

    /**
     * @brief Get the set of daughter weak pointers (note: does not lock the registry).
     *
     * @return The set of daughter weak pointers.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, DaughterEdgeWeakPointers, m_daughterEdges, const,
                                    ReadLock, m_mutexDaughters);

    /**
     * @brief Get the set of parent weak pointers (note: does not lock the registry).
     *
     * @return The set of parent weak pointers.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, ParentEdgeWeakPointers, m_parentEdges, const, ReadLock,
                                    m_mutexParents);

    /**
     * @brief Get the set of parent weak pointers (note: does not lock the registry).
     *
     * @return The set of parent weak pointers.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, ContainedWeakPointers, m_contained, const, ReadLock,
                                    m_mutexContained);

    /**
     * @brief Append to the list of contained weak pointers.
     *
     * @param wpContained The contained weak pointer to append.
     *
     * @return Success.
     */
    bool AppendContainedWeakPointers(const TBASE_wPtr &wpContained);

    /**
     * @brief Remove a weak pointer from the list of contained objects.
     *
     * @param wpContained The contained weak pointer to remove.
     *
     * @return Success.
     */
    bool RemoveContainedWeakPointer(const TBASE_wPtr &wpContained);

    /**
     * @brief Get the set of containing weak pointers (note: does not lock the registry).
     *
     * @return The set of containing weak pointers.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, ContainingWeakPointers, m_containing, const, ReadLock,
                                    m_mutexContaining);

    /**
     * @brief Append to the list of containing weak pointers.
     *
     * @param wpContaining The containing weak pointer to append.
     *
     * @return Success.
     */
    bool AppendContainingWeakPointers(const TBASE_wPtr &wpContaining);

    /**
     * @brief Get the containing weak pointer (note: does not lock the registry).
     *
     * @return The containing weak pointer.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(const, ContainingWeakPointer, m_wpContaining, const, ReadLock,
                                    m_mutexContaining);

    /**
     * @brief Set the containing weak pointer (note: does not lock the registry).
     *
     * @param wpContaining The containing weak pointer.
     */
    KL_SIMPLE_MOVE_SETTER_LOCK(ContainingWeakPointer, TBASE_wPtr, wpContaining, m_wpContaining,
                               WriteLock, m_mutexContaining);

    friend Registry;  ///< Alias for the object registry from the base class.

    template <typename T>
    friend class HierarchicalVisualizationUtility;

    friend class Koala;

private:
    mutable Mutex m_mutexDaughters;   ///< A mutex for locking daughter-related variables.
    mutable Mutex m_mutexParents;     ///< A mutex for locking parent-related variables.
    mutable Mutex m_mutexContained;   ///< A mutex for locking contained-related variables.
    mutable Mutex m_mutexContaining;  ///< A mutex for locking containing-related variables.

    PseudoEdgeBase_wPtrSet m_daughterEdges;         ///< The daughter pseudo-edges.
    PseudoEdgeBase_wPtrSet m_parentEdges;           ///< The parent pseudo-edges.
    TBASE_wPtrSet m_contained;                      ///< The contained objects.
    TBASE_wPtrSet m_containing;                     ///< All the containing objects.
    TBASE_wPtr m_wpContaining;                      ///< Weak pointer to the containing object.
    PseudoEdgeBase_wPtrSet m_relatedDaughterEdges;  ///< The related daughter edges.
    PseudoEdgeBase_wPtrSet m_relatedParentEdges;    ///< The related parent edges.
    Edge_sPtrSet m_edges;                           ///< The edges.

    /**
     * @brief Subsume a set of objects into one object.
     *
     * @param objectsToSubsume The set of weak pointers to the objects to subsume.
     */
    void SubsumeImpl(TBASE_wPtrSet &&objectsToMerge);

    /**
     * @brief Recursively append the containing weak pointers of the contained objects of a given
     * object to merge.
     *
     * @param spObjectToMerge Shared pointer to the object to merge.
     * @param wpThis Weak pointer to this object.
     */
    void RecursivelyAppendContainingWeakPointers(const TBASE_sPtr &spObjectToMerge,
                                                 const TBASE_wPtr &wpThis) const;

    /**
     * @brief Check the consistency of a subsume operation, clean up the set, and return a
     * (possibly null) shared pointer to the containing object.
     *
     * @param objectsToMerge The set of objects to merge.
     *
     * @return Shared pointer to the containing object.
     */
    auto CheckSubsumeConsistency(TBASE_wPtrSet &objectsToMerge) const;

    /**
     * @brief Find out whether a member is internal to a merge.
     *
     * @param objectsToMerge The set of objects to merge.
     * @param spMember Shared pointer to the member.
     *
     * @return Whether the member is internal.
     */
    auto IsMemberInternal(const TBASE_wPtrSet &objectsToMerge, const TBASE_sPtr &spMember) const;

    /**
     * @brief Find out whether an edge already exists.
     *
     * @param edgeSet The edge set.
     * @param spEdge Shared pointer to the edge.
     *
     * @return Whether the edge already exists.
     */
    auto DoesEdgeExist(const PseudoEdgeBase_wPtrSet &edgeSet,
                       const PseudoEdgeBase_sPtr &spEdge) const;

    /**
     * @brief Append a member edge.
     *
     * @param spMember Shared pointer to the member.
     * @param spEdge Shared pointer to the underlying edge.
     * @param memberIsParent Whether the member is the parent.
     * @param reciprocate Whether to reciprocate.
     */
    void AppendMemberEdge(const TBASE_sPtr &spMember, const Edge_sPtr &spEdge,
                          const bool memberIsParent, const bool reciprocate);

    /**
     * @brief Subsume some member edges.
     *
     * @param objectsToMerge The set of merging objects.
     * @param edgesToSubsume The set of edges to subsume.
     * @param edgesToAppend The set of edges to append.
     * @param areParentEdges Whether the edges are parent edges.
     */
    void SubsumeMemberEdges(const TBASE_wPtrSet &objectsToMerge,
                            const PseudoEdgeBase_wPtrSet &edgesToSubsume,
                            const PseudoEdgeBase_wPtrSet &edgesToAppend, const bool areParentEdges);

    /**
     * @brief Redirect related member edges to the new object.
     *
     * @param objectsToMerge The set of objects to merge.
     * @param spObjectToMerge Shared pointer to the current object.
     * @param relatedEdges The set of related edges.
     * @param relatedEdgesToAppend The set of related edges to append.
     * @param areParentEdges Whether the edges are parent edges.
     */
    void RedirectRelatedMemberEdges(const TBASE_wPtrSet &objectsToMerge,
                                    const TBASE_sPtr &spObjectToMerge,
                                    const PseudoEdgeBase_wPtrSet &relatedEdges,
                                    PseudoEdgeBase_wPtrSet &relatedEdgesToAppend,
                                    const bool areParentEdges);

    /**
     * @brief Append the list of members.
     *
     * @param thisAddMemberEdges The edge set of this member's members to add to.
     * @param memberAddMemberEdges The edge set of the member's members to add to.
     * @param spMember Shared pointer to the member to append.
     * @param memberIsParent Whether the member is the parent.
     * @param args The arguments to pass to the edge constructor.
     *
     * @return The new edge.
     */
    template <typename TEDGE, typename... TARGS>
    auto &AddMemberEdge(PseudoEdgeBase_wPtrSet &thisAddMemberEdges,
                        PseudoEdgeBase_wPtrSet &memberAddMemberEdges, const TBASE_sPtr &spMember,
                        const bool memberIsParent, TARGS &&... args);

    /**
     * @brief Add an edge shared pointer to the list.
     *
     * @param spEdge Shared pointer to the edge.
     */
    void AddEdgeSharedPtr(const Edge_sPtr &spEdge);

    /**
     * @brief Append the list of members (implementation method).
     *
     * @param thisAddMemberEdges The edge set of this member's members to add to.
     * @param memberAddMemberEdges The edge set of the member's members to add to.
     * @param spMember Shared pointer to the member to append.
     * @param spUnderlyingEdge The underlying edge to use.
     * @param memberIsParent Whether the member is the parent.
     */
    void AddMemberEdgeImpl(PseudoEdgeBase_wPtrSet &thisAddMemberEdges,
                           PseudoEdgeBase_wPtrSet &memberAddMemberEdges, const TBASE_sPtr &spMember,
                           const Edge_sPtr &spUnderlyingEdge, const bool memberIsParent);

    /**
     * @brief Find the biggest disjoint member.
     *
     * @param spCurrentMember Shared pointer to the current member.
     * @param wpThis Weak pointer to this object.
     *
     * @return Shared pointer to the biggest disjoint member.
     */
    auto FindBiggestDisjointMember(const TBASE_sPtr &spCurrentMember,
                                   const TBASE_wPtr &wpThis) const -> TBASE_sPtr;

    /**
     * @brief Recursively add pseudo-edges up the chain.
     *
     * @param thisAddMemberEdges The edge set to which to add the new edge.
     * @param spCurrentMember Shared pointer to the current member.
     * @param spBiggestMember Shared pointer to the biggest member.
     * @param spUnderlyingEdge Shared pointer to the underlying edge.
     * @param biggestMemberIsParent Whether the biggest member is the parent.
     */
    void RecursivelyAddPseudoEdges(PseudoEdgeBase_wPtrSet &thisAddMemberEdges,
                                   const TBASE_sPtr &spCurrentMember,
                                   const TBASE_sPtr &spBiggestMember,
                                   const Edge_sPtr &spUnderlyingEdge,
                                   const bool biggestMemberIsParent);

    /**
     * @brief Get the set of related parent pseudo-edge weak pointers (note: does not lock the
     * registry).
     *
     * @return The set of related pseudo-edge weak pointers.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, RelatedParentEdges, m_relatedParentEdges, const);

    /**
     * @brief Get the set of related parent pseudo-edge weak pointers (note: does not lock the
     * registry).
     *
     * @return The set of related pseudo-edge weak pointers.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, RelatedDaughterEdges, m_relatedDaughterEdges, const);

    /**
     * @brief Add a related parent edge.
     *
     * @param wpPseudoEdge Weak pointer to the edge.
     *
     * @return Success.
     */
    auto AddRelatedParentEdge(const PseudoEdgeBase_wPtr &wpPseudoEdge);

    /**
     * @brief Add a related daughter edge.
     *
     * @param wpPseudoEdge Weak pointer to the edge.
     *
     * @return Success.
     */
    auto AddRelatedDaughterEdge(const PseudoEdgeBase_wPtr &wpPseudoEdge);

    /**
     * @brief Remove a related parent edge.
     *
     * @param wpPseudoEdge Weak pointer to the edge.
     *
     * @return Success.
     */
    auto RemoveRelatedParentEdge(const PseudoEdgeBase_wPtr &wpPseudoEdge);

    /**
     * @brief Remove a related daughter edge.
     *
     * @param wpPseudoEdge Weak pointer to the edge.
     *
     * @return Success.
     */
    auto RemoveRelatedDaughterEdge(const PseudoEdgeBase_wPtr &wpPseudoEdge);

    /**
     * @brief Remove a parent edge.
     *
     * @param wpPseudoEdge Weak pointer to the edge.
     *
     * @return Success.
     */
    auto RemoveParentEdge(const PseudoEdgeBase_wPtr &wpPseudoEdge);

    /**
     * @brief Remove a daughter edge.
     *
     * @param wpPseudoEdge Weak pointer to the edge.
     *
     * @return Success.
     */
    auto RemoveDaughterEdge(const PseudoEdgeBase_wPtr &wpPseudoEdge);

    /**
     * @brief Append to the list of member weak pointers.
     *
     * @param memberEdges The member edge set to use.
     * @param wpPseudoEdge Weak pointer to the pseudo-edge.
     *
     * @return Success.
     */
    auto AppendMemberWeakPointers(PseudoEdgeBase_wPtrSet &memberEdges,
                                  const PseudoEdgeBase_wPtr &wpPseudoEdge);

    /**
     * @brief A method for recursively iterating over a parameter pack to produce a set of shared
     * pointers.
     *
     * @param set The current set.
     * @param sets The remaining sets.
     *
     * @return A set of shared pointers to the objects in the current set.
     */
    template <typename TSET, typename... TSETS>
    auto RecursivelyGetSharedPointers(TSET &&set, TSETS &&... sets) const;

    /**
     * @brief A method for recursively iterating over a parameter pack to produce a set of shared
     * pointers (the terminating method).
     *
     * @param set The current set.
     *
     * @return A set of shared pointers to the objects in the current set.
     */
    template <typename TSET>
    auto RecursivelyGetSharedPointers(TSET &&set) const;

    /**
     * @brief A method for recursively iterating over a parameter pack to produce a set of weak
     * pointers.
     *
     * @param set The current set.
     * @param sets The remaining sets.
     *
     * @return A set of weak pointers to the objects in the current set.
     */
    template <typename TSET, typename... TSETS>
    auto RecursivelyGetWeakPointers(TSET &&set, TSETS &&... sets) const;

    /**
     * @brief A method for recursively iterating over a parameter pack to produce a set of weak
     * pointers (the terminating method).
     *
     * @param set The current set.
     *
     * @return A set of weak pointers to the objects in the current set.
     */
    template <typename TSET>
    auto RecursivelyGetWeakPointers(TSET &&set) const;

    /**
     * @brief Get a shared pointer to a member.
     *
     * @param arg The member, member alias or member ID.
     *
     * @return The shared pointer.
     */
    template <typename T,
              typename = std::enable_if_t<std::is_same<TALIAS_D, std::decay_t<T>>::value ||
                                          std::is_base_of<TBASE_D, std::decay_t<T>>::value ||
                                          std::is_same<ID_t, std::decay_t<T>>::value>>
    auto GetSharedPointerToMember(T &&arg) const;

    /**
     * @brief Get a weak pointer to this object.
     *
     * @return The weak pointer.
     */
    auto GetWeakPointer();

    /**
     * @brief Get a weak pointer to this object.
     *
     * @return The weak pointer.
     */
    auto GetWeakPointer() const;

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

    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Templates/HierarchicalObjectTemplate.txx"

#endif  // #ifndef KL_HIERARCHICAL_OBJECT_TEMPLATE_H
