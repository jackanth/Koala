/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Templates/HierarchicalObjectTemplate.txx
 *
 * @brief Template implementation header file for the hierarchical object
 * (HierarchicalObjectTemplate) abstract class template.
 */

#ifndef KL_HIERARCHICAL_OBJECT_TEMPLATE_IMPL_H
#define KL_HIERARCHICAL_OBJECT_TEMPLATE_IMPL_H

namespace kl
{
template <typename TBASE, typename TALIAS>
HierarchicalObjectTemplate<TBASE, TALIAS>::HierarchicalObjectTemplate(
    const HierarchicalObjectTemplate<TBASE, TALIAS> &other) noexcept
    : RegisteredObject{other},
      m_mutexDaughters{},
      m_mutexParents{},
      m_mutexContained{},
      m_mutexContaining{},
      m_daughterEdges{},
      m_parentEdges{},
      m_contained{},
      m_containing{},
      m_wpContaining{},
      m_relatedDaughterEdges{},
      m_relatedParentEdges{},
      m_edges{}
{
    const auto thisDaughtersLock = WriteLock{m_mutexDaughters};
    const auto thisParentsLock = WriteLock{m_mutexParents};
    const auto thisContainedLock = WriteLock{m_mutexContained};
    const auto thisContainingLock = WriteLock{m_mutexContaining};

    const auto otherDaughtersLock = ReadLock{other.m_mutexDaughters};
    const auto otherParentsLock = ReadLock{other.m_mutexParents};
    const auto otherContainedLock = ReadLock{other.m_mutexContained};
    const auto otherContainingLock = ReadLock{other.m_mutexContaining};

    m_daughterEdges = other.m_daughterEdges;
    m_parentEdges = other.m_parentEdges;
    m_contained = other.m_contained;
    m_containing = other.m_containing;
    m_wpContaining = other.m_wpContaining;
    m_relatedDaughterEdges = other.m_relatedDaughterEdges;
    m_relatedParentEdges = other.m_relatedParentEdges;
    m_edges = other.m_edges;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
HierarchicalObjectTemplate<TBASE, TALIAS>::HierarchicalObjectTemplate(
    HierarchicalObjectTemplate<TBASE, TALIAS> &&other) noexcept
    : RegisteredObject{other},
      m_mutexDaughters{},
      m_mutexParents{},
      m_mutexContained{},
      m_mutexContaining{},
      m_daughterEdges{},
      m_parentEdges{},
      m_contained{},
      m_containing{},
      m_wpContaining{},
      m_relatedDaughterEdges{},
      m_relatedParentEdges{},
      m_edges{}
{
    const auto thisDaughtersLock = WriteLock{m_mutexDaughters};
    const auto thisParentsLock = WriteLock{m_mutexParents};
    const auto thisContainedLock = WriteLock{m_mutexContained};
    const auto thisContainingLock = WriteLock{m_mutexContaining};

    const auto otherDaughtersLock = WriteLock{other.m_mutexDaughters};
    const auto otherParentsLock = WriteLock{other.m_mutexParents};
    const auto otherContainedLock = WriteLock{other.m_mutexContained};
    const auto otherContainingLock = WriteLock{other.m_mutexContaining};

    m_daughterEdges = std::move_if_noexcept(other.m_daughterEdges);
    m_parentEdges = std::move_if_noexcept(other.m_parentEdges);
    m_contained = std::move_if_noexcept(other.m_contained);
    m_containing = std::move_if_noexcept(other.m_containing);
    m_wpContaining = std::move_if_noexcept(other.m_wpContaining);
    m_relatedDaughterEdges = std::move_if_noexcept(other.m_relatedDaughterEdges);
    m_relatedParentEdges = std::move_if_noexcept(other.m_relatedParentEdges);
    m_edges = std::move_if_noexcept(other.m_edges);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto &HierarchicalObjectTemplate<TBASE, TALIAS>::operator=(
    const HierarchicalObjectTemplate<TBASE, TALIAS> &other) noexcept
{
    if (this != &other)
    {
        const auto regLock = ReadLock{this->GetRegistry().Mutex()};

        auto thisDaughtersLock = WriteLock{m_mutexDaughters, std::defer_lock};
        auto thisParentsLock = WriteLock{m_mutexParents, std::defer_lock};
        auto thisContainedLock = WriteLock{m_mutexContained, std::defer_lock};
        auto thisContainingLock = WriteLock{m_mutexContaining, std::defer_lock};

        auto otherDaughtersLock = ReadLock{other.m_mutexDaughters, std::defer_lock};
        auto otherParentsLock = ReadLock{other.m_mutexParents, std::defer_lock};
        auto otherContainedLock = ReadLock{other.m_mutexContained, std::defer_lock};
        auto otherContainingLock = ReadLock{other.m_mutexContaining, std::defer_lock};

        Lock<>::DoLock(thisDaughtersLock, thisParentsLock, thisContainedLock, thisContainingLock,
                       otherDaughtersLock, otherParentsLock, otherContainedLock,
                       otherContainingLock);

        RegisteredObject::operator=(other);

        m_daughterEdges = other.m_daughterEdges;
        m_parentEdges = other.m_parentEdges;
        m_contained = other.m_contained;
        m_containing = other.m_containing;
        m_wpContaining = other.m_wpContaining;
        m_relatedDaughterEdges = other.m_relatedDaughterEdges;
        m_relatedParentEdges = other.m_relatedParentEdges;
        m_edges = other.m_edges;
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto &HierarchicalObjectTemplate<TBASE, TALIAS>::operator=(
    HierarchicalObjectTemplate<TBASE, TALIAS> &&other) noexcept
{
    if (this != &other)
    {
        const auto regLock = ReadLock{this->GetRegistry().Mutex()};

        auto thisDaughtersLock = WriteLock{m_mutexDaughters, std::defer_lock};
        auto thisParentsLock = WriteLock{m_mutexParents, std::defer_lock};
        auto thisContainedLock = WriteLock{m_mutexContained, std::defer_lock};
        auto thisContainingLock = WriteLock{m_mutexContaining, std::defer_lock};

        auto otherDaughtersLock = WriteLock{other.m_mutexDaughters, std::defer_lock};
        auto otherParentsLock = WriteLock{other.m_mutexParents, std::defer_lock};
        auto otherContainedLock = WriteLock{other.m_mutexContained, std::defer_lock};
        auto otherContainingLock = WriteLock{other.m_mutexContaining, std::defer_lock};

        Lock<>::DoLock(thisDaughtersLock, thisParentsLock, thisContainedLock, thisContainingLock,
                       otherDaughtersLock, otherParentsLock, otherContainedLock,
                       otherContainingLock);

        RegisteredObject::operator=(other);

        m_daughterEdges = std::move_if_noexcept(other.m_daughterEdges);
        m_parentEdges = std::move_if_noexcept(other.m_parentEdges);
        m_contained = std::move_if_noexcept(other.m_contained);
        m_containing = std::move_if_noexcept(other.m_containing);
        m_wpContaining = std::move_if_noexcept(other.m_wpContaining);
        m_relatedDaughterEdges = std::move_if_noexcept(other.m_relatedDaughterEdges);
        m_relatedParentEdges = std::move_if_noexcept(other.m_relatedParentEdges);
        m_edges = std::move_if_noexcept(other.m_edges);
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::Contained() const
{
    const auto lock = ReadLock{m_mutexContained};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    using TOBJECT_D = std::decay_t<TOBJECT>;
    return RangeBasedContainer<TBASE_wPtrSet, TBASE_D, TOBJECT_D, TOBJECT_D>{
        [](const TBASE_wPtr &) { return true; },
        [](const TBASE_wPtr &wpBase) { return wpBase.lock(); },
        [](const std::shared_ptr<TOBJECT_D> &spObject) -> auto &{return *spObject;
}
, ReadLock{m_mutexContained}, ReadLock{this->GetRegistry().Mutex()}, m_contained
};  // namespace kl
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::HasContainingObjectOfType() const noexcept
{
    const auto lock = ReadLock{m_mutexContaining};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    if (const auto spObject = m_wpContaining.lock())
    {
        if (std::dynamic_pointer_cast<TOBJECT>(spObject)) return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
auto &HierarchicalObjectTemplate<TBASE, TALIAS>::Containing() const
{
    const auto lock = ReadLock{m_mutexContaining};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    if (const auto spObject = m_wpContaining.lock())
    {
        if (auto spCastObject = std::dynamic_pointer_cast<TOBJECT>(spObject)) return *spCastObject;
    }

    KL_THROW("Did not have containing object of this type for object of base type "
             << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename T, typename>
inline void HierarchicalObjectTemplate<TBASE, TALIAS>::Subsume(T &&arg)
{
    const auto regLock = WriteLock{this->GetRegistry().Mutex()};
    this->SubsumeImpl(TBASE_wPtrSet{
        static_cast<TBASE_wPtr>(this->GetSharedPointerToMember(std::forward<T>(arg)))});
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TSET>
inline void HierarchicalObjectTemplate<TBASE, TALIAS>::SubsumeSet(TSET &&objectSetToSubsume)
{
    const auto regLock = WriteLock{this->GetRegistry().Mutex()};
    this->SubsumeImpl(this->RecursivelyGetWeakPointers(std::forward<TSET>(objectSetToSubsume)));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename... TSETS>
inline void HierarchicalObjectTemplate<TBASE, TALIAS>::SubsumeSets(TSETS &&... objectSetsToSubsume)
{
    const auto regLock = WriteLock{this->GetRegistry().Mutex()};
    this->SubsumeImpl(
        this->RecursivelyGetWeakPointers(std::forward<TSETS>(objectSetsToSubsume)...));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename T, typename>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::Encloses(T &&arg) const
{
    const auto lock = ReadLock{m_mutexContaining};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    for (const auto &wpContaining :
         this->GetSharedPointerToMember(std::forward<T>(arg))->ContainingWeakPointers())
    {
        if (const auto spContaining = wpContaining.lock())
        {
            if (spContaining->ID() == this->ID()) return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::Daughters() const
{
    const auto lock = ReadLock{m_mutexDaughters};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    using TOBJECT_D = std::decay_t<TOBJECT>;

    return RangeBasedContainer<PseudoEdgeBase_wPtrSet, PseudoEdgeBase, PseudoEdgeBase, TOBJECT_D>{
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) {
            return wpPseudoEdge.lock()->template ObjectIsA<TOBJECT_D>();
        },
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) { return wpPseudoEdge.lock(); },
        [](const PseudoEdgeBase_sPtr &spEdge) -> auto &{
                                                  return spEdge->template GetObject<TOBJECT_D>();
}
, ReadLock{m_mutexDaughters}, ReadLock{this->GetRegistry().Mutex()}, m_daughterEdges
}
;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TEDGE, typename>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::DaughterEdges() const
{
    using TPSEUDOEDGE = PseudoEdge<std::decay_t<TEDGE>>;

    const auto lock = ReadLock{m_mutexDaughters};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    return RangeBasedContainer<PseudoEdgeBase_wPtrSet, PseudoEdgeBase, TPSEUDOEDGE, TPSEUDOEDGE>{
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) {
            return static_cast<bool>(std::dynamic_pointer_cast<TPSEUDOEDGE>(wpPseudoEdge.lock()));
        },
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) {
            return std::dynamic_pointer_cast<TPSEUDOEDGE>(wpPseudoEdge.lock());
        },
        [](const typename TPSEUDOEDGE::sPtr &spEdge) -> auto &{return *spEdge;
}
, ReadLock{m_mutexDaughters}, ReadLock{this->GetRegistry().Mutex()}, m_daughterEdges
}
;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TEDGE, typename T, typename... TARGS, typename>
inline auto &HierarchicalObjectTemplate<TBASE, TALIAS>::AddDaughterEdge(T &&arg, TARGS &&... args)
{
    const auto lock = WriteLock{m_mutexDaughters};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};
    const auto spMember = this->GetSharedPointerToMember(std::forward<T>(arg));

    KL_ASSERT(spMember, "Could not add daughter edge because pointer to daughter was null");
    return this->AddMemberEdge<TEDGE>(m_daughterEdges, spMember->m_parentEdges, spMember, false,
                                      std::forward<TARGS>(args)...);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TOBJECT>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::Parents() const
{
    const auto lock = ReadLock{m_mutexParents};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    using TOBJECT_D = std::decay_t<TOBJECT>;

    return RangeBasedContainer<PseudoEdgeBase_wPtrSet, PseudoEdgeBase, PseudoEdgeBase, TOBJECT_D>{
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) {
            return wpPseudoEdge.lock()->template ObjectIsA<TOBJECT_D>();
        },
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) { return wpPseudoEdge.lock(); },
        [](const PseudoEdgeBase_sPtr &spEdge) -> auto &{
                                                  return spEdge->template GetObject<TOBJECT_D>();
}
, ReadLock{m_mutexParents}, ReadLock{this->GetRegistry().Mutex()}, m_parentEdges
}
;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TEDGE, typename>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::ParentEdges() const
{
    using TPSEUDOEDGE = PseudoEdge<std::decay_t<TEDGE>>;

    const auto lock = ReadLock{m_mutexParents};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};

    return RangeBasedContainer<PseudoEdgeBase_wPtrSet, PseudoEdgeBase, TPSEUDOEDGE, TPSEUDOEDGE>{
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) {
            return static_cast<bool>(std::dynamic_pointer_cast<TPSEUDOEDGE>(wpPseudoEdge.lock()));
        },
        [](const PseudoEdgeBase_wPtr &wpPseudoEdge) {
            return std::dynamic_pointer_cast<TPSEUDOEDGE>(wpPseudoEdge.lock());
        },
        [](const typename TPSEUDOEDGE::sPtr &spEdge) -> auto &{return *spEdge;
}
, ReadLock{m_mutexParents}, ReadLock{this->GetRegistry().Mutex()}, m_parentEdges
}
;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TEDGE, typename T, typename... TARGS, typename>
inline auto &HierarchicalObjectTemplate<TBASE, TALIAS>::AddParentEdge(T &&arg, TARGS &&... args)
{
    const auto lock = WriteLock{m_mutexParents};
    const auto regLock = ReadLock{this->GetRegistry().Mutex()};
    const auto spMember = this->GetSharedPointerToMember(std::forward<T>(arg));

    KL_ASSERT(spMember, "Could not add parent edge because pointer to parent was null");
    return this->AddMemberEdge<TEDGE>(m_parentEdges, spMember->m_daughterEdges, spMember, true,
                                      std::forward<TARGS>(args)...);
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline HierarchicalObjectTemplate<TBASE, TALIAS>::HierarchicalObjectTemplate(
    Registry_wPtr wpRegistry, const ID_t id, Koala_wPtr wpKoala) noexcept
    : RegisteredObject{std::move_if_noexcept(wpRegistry), id, std::move_if_noexcept(wpKoala)},
      m_mutexDaughters{},
      m_mutexParents{},
      m_mutexContained{},
      m_mutexContaining{},
      m_daughterEdges{},
      m_parentEdges{},
      m_contained{},
      m_containing{},
      m_wpContaining{},
      m_relatedDaughterEdges{},
      m_relatedParentEdges{},
      m_edges{}
{
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline HierarchicalObjectTemplate<TBASE, TALIAS>::HierarchicalObjectTemplate() noexcept
    : RegisteredObject{},
      m_mutexDaughters{},
      m_mutexParents{},
      m_mutexContained{},
      m_mutexContaining{},
      m_daughterEdges{},
      m_parentEdges{},
      m_contained{},
      m_containing{},
      m_wpContaining{},
      m_relatedDaughterEdges{},
      m_relatedParentEdges{},
      m_edges{}
{
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline std::string HierarchicalObjectTemplate<TBASE, TALIAS>::GetGraphNodeLabel() const
{
    return std::to_string(this->ID());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline bool HierarchicalObjectTemplate<TBASE, TALIAS>::AppendContainedWeakPointers(
    const TBASE_wPtr &wpContained)
{
    // Note: no registry locking.
    const auto lock = WriteLock{m_mutexContained};

    if (!m_contained.insert(wpContained).second) return false;

    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
bool HierarchicalObjectTemplate<TBASE, TALIAS>::RemoveContainedWeakPointer(
    const TBASE_wPtr &wpContained)
{
    // Note: no registry locking.
    const auto lock = WriteLock{m_mutexContained};

    auto containedFindIter = m_contained.find(wpContained);
    if (containedFindIter == m_contained.cend()) return false;

    m_contained.erase(containedFindIter);
    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline bool HierarchicalObjectTemplate<TBASE, TALIAS>::AppendContainingWeakPointers(
    const TBASE_wPtr &wpContaining)
{
    // Note: no registry locking.
    const auto lock = WriteLock{m_mutexContaining};
    return m_containing.insert(wpContaining).second;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
void HierarchicalObjectTemplate<TBASE, TALIAS>::SubsumeImpl(TBASE_wPtrSet &&objectsToMerge)
{
    // Check all containing objects are the same and delete any dead pointers.
    const auto spInitialContaining = this->CheckSubsumeConsistency(objectsToMerge);
    const auto spContaining = m_wpContaining.lock();

    // Subsume may proceed, so set the merging objects as contained objects of this object and the
    // containing objects of the merging objects as this object.
    for (const auto &wpObjectToMerge : objectsToMerge)
    {
        const auto spObjectToMerge = wpObjectToMerge.lock();

        if ((spObjectToMerge == spInitialContaining) || (spObjectToMerge == spContaining))
            KL_THROW("Cannot merge containing object");

        spObjectToMerge->ContainingWeakPointer(this->GetWeakPointer());
        this->RecursivelyAppendContainingWeakPointers(spObjectToMerge, this->GetWeakPointer());
        m_contained.insert(wpObjectToMerge);

        // The parents and daughters of this merging object become the parents and daughters of this
        // object.
        this->SubsumeMemberEdges(objectsToMerge, spObjectToMerge->DaughterEdgeWeakPointers(),
                                 m_daughterEdges, false);
        this->SubsumeMemberEdges(objectsToMerge, spObjectToMerge->ParentEdgeWeakPointers(),
                                 m_parentEdges, true);

        this->RedirectRelatedMemberEdges(objectsToMerge, spObjectToMerge,
                                         spObjectToMerge->RelatedDaughterEdges(),
                                         m_relatedDaughterEdges, false);

        this->RedirectRelatedMemberEdges(objectsToMerge, spObjectToMerge,
                                         spObjectToMerge->RelatedParentEdges(),
                                         m_relatedParentEdges, true);
    }

    // The containing object of the merge object becomes that of the merging objects (if it has no
    // containing object already).
    if (!spContaining && spInitialContaining)
    {
        m_wpContaining = static_cast<TBASE_wPtr>(spInitialContaining);
        spInitialContaining->AppendContainedWeakPointers(
            static_cast<TBASE_wPtr>(std::dynamic_pointer_cast<TBASE>(this->GetSharedPointer())));

        for (const auto &wpContaining : spInitialContaining->ContainingWeakPointers())
            m_containing.insert(wpContaining);

        m_containing.insert(static_cast<TBASE_wPtr>(spInitialContaining));
    }

    // Finally, remove the merging objects from their initial containing object (if there was one).
    if (spInitialContaining)
    {
        for (const auto &wpObjectToMerge : objectsToMerge)
        {
            KL_ASSERT(
                spInitialContaining->RemoveContainedWeakPointer(wpObjectToMerge),
                "Subsume process failed part-way through: "
                    << "could not erase contained object from last containing object for object "
                       "of base type "
                    << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
        }
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
void HierarchicalObjectTemplate<TBASE, TALIAS>::RecursivelyAppendContainingWeakPointers(
    const TBASE_sPtr &spObjectToMerge, const TBASE_wPtr &wpThis) const
{
    spObjectToMerge->AppendContainingWeakPointers(wpThis);

    for (const auto &wpContaining : m_containing)
        spObjectToMerge->AppendContainingWeakPointers(wpContaining);

    for (const auto &wpContained : spObjectToMerge->ContainedWeakPointers())
    {
        if (const auto spContained = wpContained.lock())
            this->RecursivelyAppendContainingWeakPointers(spContained, wpThis);
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::CheckSubsumeConsistency(
    TBASE_wPtrSet &objectsToMerge) const
{
    // Check all containing objects are the same and delete any dead pointers.
    auto firstTime = true;
    auto wpInitialContaining = TBASE_wPtr{};

    for (auto iter = objectsToMerge.cbegin(); iter != objectsToMerge.cend(); /* no iterator */)
    {
        if (const auto spObject = iter->lock())
        {
            if (firstTime)
            {
                wpInitialContaining = spObject->ContainingWeakPointer();
                firstTime = false;
                ++iter;
            }

            else if (spObject->ContainingWeakPointer().lock() !=
                     wpInitialContaining.lock())  // the merging objects are incoherent
            {
                KL_THROW(
                    "Could not subsume because the objects did not have the same containing object "
                    "for "
                    "object of base type "
                    << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
            }

            else
                ++iter;
        }

        else  // it is a dead pointer, so erase it
            iter = objectsToMerge.erase(iter);
    }

    const auto spInitialContaining = wpInitialContaining.lock();
    const auto spContaining = m_wpContaining.lock();

    // Check if set if empty.
    if (firstTime || objectsToMerge.empty())
    {
        KL_THROW(
            "Could not subsume because the list of objects to subsume was empty for object of base "
            "type "
            << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
    }

    if ((spContaining != spInitialContaining) && spContaining && spInitialContaining)
    {
        // If its containing object is wrong, throw an error.
        KL_THROW(
            "Could not subsume because the initial object had an incompatible containing object "
            "for "
            "object of base type "
            << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName());
    }

    // Check that either this has no containing object, the initial containing object is empty, or
    // they both have the same containing object.
    if (spContaining && spInitialContaining && (spContaining != spInitialContaining))
    {
        KL_THROW("Could not subsume for object of base type "
                 << KL_WHITE_BOLD << this->GetRegistry().PrintableBaseName() << KL_NORMAL
                 << " because one of the following must be true: the subsuming object has no "
                    "containing object; the subsumed objects have "
                 << "no containing object; the containing objects of both subsuming and subsumed "
                    "objects are the same");
    }

    return spInitialContaining;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::IsMemberInternal(
    const TBASE_wPtrSet &objectsToMerge, const TBASE_sPtr &spMember) const
{
    if (objectsToMerge.find(static_cast<TBASE_wPtr>(spMember)) != objectsToMerge.cend())
        return true;

    const auto &daughterContaining = spMember->ContainingWeakPointers();

    for (const auto &wpObjectToMerge : objectsToMerge)
    {
        if (daughterContaining.find(wpObjectToMerge) != daughterContaining.end()) return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::DoesEdgeExist(
    const PseudoEdgeBase_wPtrSet &edgeSet, const PseudoEdgeBase_sPtr &spEdge) const
{
    const auto wpObject = spEdge->ObjectWeakPointer();

    for (const auto &wpDaughterEdge : edgeSet)
    {
        if (const auto spDaughterEdge = wpDaughterEdge.lock())
        {
            if ((spEdge->OwningObjectWeakPointer() == spDaughterEdge->OwningObjectWeakPointer()) &&
                (spEdge->UnderlyingEdgeSharedPointer()->IsEquivalent(
                    *spDaughterEdge->UnderlyingEdgeSharedPointer())))
            {
                return true;
            }
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
void HierarchicalObjectTemplate<TBASE, TALIAS>::AppendMemberEdge(const TBASE_sPtr &spMember,
                                                                 const Edge_sPtr &spEdge,
                                                                 const bool memberIsParent,
                                                                 const bool reciprocate)
{
    const auto wpPseudoEdge =
        spEdge->CreatePseudoEdge(static_cast<TBASE_wPtr>(spMember), this->GetWeakPointer());

    if (memberIsParent)
    {
        this->AppendMemberWeakPointers(m_parentEdges, wpPseudoEdge);

        if (reciprocate) spMember->AddRelatedParentEdge(wpPseudoEdge);
    }

    else
    {
        this->AppendMemberWeakPointers(m_daughterEdges, wpPseudoEdge);

        if (reciprocate) spMember->AddRelatedDaughterEdge(wpPseudoEdge);
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
void HierarchicalObjectTemplate<TBASE, TALIAS>::SubsumeMemberEdges(
    const TBASE_wPtrSet &objectsToMerge, const PseudoEdgeBase_wPtrSet &edgesToSubsume,
    const PseudoEdgeBase_wPtrSet &edgesToAppend, const bool areParentEdges)
{
    for (const auto &wpEdge : edgesToSubsume)
    {
        if (const auto spEdge = wpEdge.lock())
        {
            if (const auto spMember = spEdge->ObjectWeakPointer().lock())
            {
                if (!spEdge->UnderlyingEdgeSharedPointer()->IsInheritable()) continue;

                if (this->IsMemberInternal(objectsToMerge, spMember)) continue;

                if (this->DoesEdgeExist(edgesToAppend, spEdge)) continue;

                this->AppendMemberEdge(spMember, spEdge->UnderlyingEdgeSharedPointer(),
                                       areParentEdges, false);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
void HierarchicalObjectTemplate<TBASE, TALIAS>::RedirectRelatedMemberEdges(
    const TBASE_wPtrSet &objectsToMerge, const TBASE_sPtr &spObjectToMerge,
    const PseudoEdgeBase_wPtrSet &relatedEdges, PseudoEdgeBase_wPtrSet &relatedEdgesToAppend,
    const bool areParentEdges)
{
    auto edgesToDelete = PseudoEdgeBase_wPtrSet{};

    for (const auto &wpRelatedEdge : relatedEdges)
    {
        if (const auto spRelatedEdge = wpRelatedEdge.lock())
        {
            if (!spRelatedEdge->UnderlyingEdgeSharedPointer()->IsInheritable()) continue;

            const auto spObject = spRelatedEdge->ObjectWeakPointer().lock();
            const auto spOwner = spRelatedEdge->OwningObjectWeakPointer().lock();

            if (!spObject || !spOwner) continue;

            if (this->IsMemberInternal(objectsToMerge, spOwner)) continue;

            KL_ASSERT((spObjectToMerge == spObject), "Found incoherent pseudo-edges");

            if (!this->DoesEdgeExist(relatedEdgesToAppend, spRelatedEdge))
            {
                spRelatedEdge->ObjectWeakPointer(this->GetWeakPointer());
                relatedEdgesToAppend.insert(wpRelatedEdge);
            }

            else
            {
                if (areParentEdges)
                    KL_ASSERT(spOwner->RemoveParentEdge(wpRelatedEdge),
                              "Found incoherent pseudo-edges");

                else
                    KL_ASSERT(spOwner->RemoveDaughterEdge(wpRelatedEdge),
                              "Found incoherent pseudo-edges");
            }
        }

        edgesToDelete.insert(wpRelatedEdge);
    }

    if (areParentEdges)
    {
        for (const auto &wpEdge : edgesToDelete)
            KL_ASSERT(spObjectToMerge->RemoveRelatedParentEdge(wpEdge),
                      "Found incoherent pseudo-edges");
    }

    else
    {
        for (const auto &wpEdge : edgesToDelete)
            KL_ASSERT(spObjectToMerge->RemoveRelatedDaughterEdge(wpEdge),
                      "Found incoherent pseudo-edges");
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TEDGE, typename... TARGS>
auto &HierarchicalObjectTemplate<TBASE, TALIAS>::AddMemberEdge(
    PseudoEdgeBase_wPtrSet &thisAddMemberEdges, PseudoEdgeBase_wPtrSet &memberAddMemberEdges,
    const TBASE_sPtr &spMember, const bool memberIsParent, TARGS &&... args)
{
    using TEDGE_D = std::decay_t<TEDGE>;
    static_assert(std::is_base_of<Edge, TEDGE_D>(),
                  "Could not add member because edge was not derived from the correct edge class");
    //  static_assert(std::is_constructible<TEDGE_D, TBASE_wPtr, TBASE_wPtr, TARGS...>::value,
    //                "Incorrect arguments passed to edge constructor");

    const auto wpThis = this->GetWeakPointer();
    const auto spUnderlyingEdge =
        memberIsParent
            ? std::shared_ptr<TEDGE_D>{new TEDGE_D{static_cast<TBASE_wPtr>(spMember), wpThis,
                                                   std::forward<TARGS>(args)...}}
            : std::shared_ptr<TEDGE_D>{new TEDGE_D{wpThis, static_cast<TBASE_wPtr>(spMember),
                                                   std::forward<TARGS>(args)...}};

    // Check for an existing edge.
    for (const Edge_sPtr &spEdge : m_edges)
    {
        if (memberIsParent)
        {
            if ((spEdge->ParentWeakPointer() == static_cast<TBASE_wPtr>(spMember)) &&
                (spEdge->DaughterWeakPointer() == wpThis))
            {
                if (spEdge->IsEquivalent(*spUnderlyingEdge))
                {
                    const auto spCastEdge = std::dynamic_pointer_cast<TEDGE_D>(spEdge);
                    KL_ASSERT(spCastEdge, "Failed to cast equivalent edge");
                    return *spCastEdge;
                }
            }
        }

        else
        {
            if ((spEdge->DaughterWeakPointer() == static_cast<TBASE_wPtr>(spMember)) &&
                (spEdge->ParentWeakPointer() == wpThis))
            {
                if (spEdge->IsEquivalent(*spUnderlyingEdge))
                {
                    const auto spCastEdge = std::dynamic_pointer_cast<TEDGE_D>(spEdge);
                    KL_ASSERT(spCastEdge, "Failed to cast equivalent edge");
                    return *spCastEdge;
                }
            }
        }
    }

    m_edges.insert(spUnderlyingEdge);
    spMember->AddEdgeSharedPtr(spUnderlyingEdge);

    this->AddMemberEdgeImpl(thisAddMemberEdges, memberAddMemberEdges, spMember,
                            std::dynamic_pointer_cast<Edge>(spUnderlyingEdge), memberIsParent);

    return *spUnderlyingEdge;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline void HierarchicalObjectTemplate<TBASE, TALIAS>::AddEdgeSharedPtr(const Edge_sPtr &spEdge)
{
    m_edges.insert(spEdge);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
void HierarchicalObjectTemplate<TBASE, TALIAS>::AddMemberEdgeImpl(
    PseudoEdgeBase_wPtrSet &thisAddMemberEdges, PseudoEdgeBase_wPtrSet &memberAddMemberEdges,
    const TBASE_sPtr &spMember, const Edge_sPtr &spUnderlyingEdge, const bool memberIsParent)
{
    const auto spThis = this->GetSharedPointer();

    const auto spBiggestMember =
        this->FindBiggestDisjointMember(spMember, static_cast<TBASE_wPtr>(spThis));
    this->RecursivelyAddPseudoEdges(thisAddMemberEdges, spThis, spBiggestMember, spUnderlyingEdge,
                                    memberIsParent);

    const auto spBiggestOtherMember = spMember->FindBiggestDisjointMember(
        this->GetSharedPointer(), static_cast<TBASE_wPtr>(spMember));
    spMember->RecursivelyAddPseudoEdges(memberAddMemberEdges, spMember, spBiggestOtherMember,
                                        spUnderlyingEdge, !memberIsParent);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::FindBiggestDisjointMember(
    const TBASE_sPtr &spCurrentMember, const TBASE_wPtr &wpThis) const -> TBASE_sPtr
{
    const auto &wpContaining = spCurrentMember->ContainingWeakPointer();

    if (const auto spContaining = wpContaining.lock())
    {
        if ((m_containing.find(wpContaining) == m_containing.end()) && !(wpContaining == wpThis))
            return this->FindBiggestDisjointMember(spContaining, wpThis);
    }

    return spCurrentMember;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
void HierarchicalObjectTemplate<TBASE, TALIAS>::RecursivelyAddPseudoEdges(
    PseudoEdgeBase_wPtrSet &thisAddMemberEdges, const TBASE_sPtr &spCurrentMember,
    const TBASE_sPtr &spBiggestMember, const Edge_sPtr &spUnderlyingEdge,
    const bool biggestMemberIsParent)
{
    // Add the pseudo edge.
    spCurrentMember->AppendMemberEdge(spBiggestMember, spUnderlyingEdge, biggestMemberIsParent,
                                      true);

    // Try to recurse outwards until we reach an object that also contains the member.
    const auto wpContaining = spCurrentMember->ContainingWeakPointer();

    if (const auto spContaining = wpContaining.lock())
    {
        const auto &memberContaining = spBiggestMember->ContainingWeakPointers();

        if ((memberContaining.find(wpContaining) == memberContaining.end()) &&
            !(wpContaining == static_cast<TBASE_wPtr>(spBiggestMember)))
            this->RecursivelyAddPseudoEdges(thisAddMemberEdges, spContaining, spBiggestMember,
                                            spUnderlyingEdge, biggestMemberIsParent);
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::AddRelatedParentEdge(
    const PseudoEdgeBase_wPtr &wpPseudoEdge)
{
    return m_relatedParentEdges.insert(wpPseudoEdge).second;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::AddRelatedDaughterEdge(
    const PseudoEdgeBase_wPtr &wpPseudoEdge)
{
    return m_relatedDaughterEdges.insert(wpPseudoEdge).second;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::RemoveRelatedParentEdge(
    const PseudoEdgeBase_wPtr &wpPseudoEdge)
{
    const auto findIter = m_relatedParentEdges.find(wpPseudoEdge);

    if (findIter != m_relatedParentEdges.end())
    {
        m_relatedParentEdges.erase(findIter);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::RemoveRelatedDaughterEdge(
    const PseudoEdgeBase_wPtr &wpPseudoEdge)
{
    const auto findIter = m_relatedDaughterEdges.find(wpPseudoEdge);

    if (findIter != m_relatedDaughterEdges.end())
    {
        m_relatedDaughterEdges.erase(findIter);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::RemoveParentEdge(
    const PseudoEdgeBase_wPtr &wpPseudoEdge)
{
    const auto findIter = m_parentEdges.find(wpPseudoEdge);

    if (findIter != m_parentEdges.end())
    {
        m_parentEdges.erase(findIter);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::RemoveDaughterEdge(
    const PseudoEdgeBase_wPtr &wpPseudoEdge)
{
    const auto findIter = m_daughterEdges.find(wpPseudoEdge);

    if (findIter != m_daughterEdges.end())
    {
        m_daughterEdges.erase(findIter);
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::AppendMemberWeakPointers(
    PseudoEdgeBase_wPtrSet &memberEdges, const PseudoEdgeBase_wPtr &wpPseudoEdge)
{
    return memberEdges.emplace(wpPseudoEdge).second;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TSET, typename... TSETS>
auto HierarchicalObjectTemplate<TBASE, TALIAS>::RecursivelyGetSharedPointers(TSET &&set,
                                                                             TSETS &&... sets) const
{
    auto pointerSet = TBASE_sPtrSet{};
    for (auto &&setItem : set)
        pointerSet.insert(this->GetRegistry().GetSharedPointer(
            std::forward<decltype(setItem.get())>(setItem.get())));

    // Recurse over the rest of the parameter pack.
    auto newPointerSet = this->RecursivelyGetSharedPointers(std::forward<TSETS>(sets)...);
    pointerSet.insert(std::make_move_iterator(newPointerSet.begin()),
                      std::make_move_iterator(newPointerSet.end()));

    return pointerSet;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TSET>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::RecursivelyGetSharedPointers(
    TSET &&set) const
{
    auto pointerSet = TBASE_sPtrSet{};
    for (auto &&setItem : set)
        pointerSet.insert(this->GetRegistry().GetSharedPointer(
            std::forward<decltype(setItem.get())>(setItem.get())));

    return pointerSet;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TSET, typename... TSETS>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::RecursivelyGetWeakPointers(
    TSET &&set, TSETS &&... sets) const
{
    auto pointerSet = TBASE_wPtrSet{};
    for (const auto &spObject :
         this->RecursivelyGetSharedPointers(std::forward<TSET>(set), std::forward<TSETS>(sets)...))
        pointerSet.insert(static_cast<TBASE_wPtr>(spObject));

    return pointerSet;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename TSET>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::RecursivelyGetWeakPointers(TSET &&set) const
{
    auto pointerSet = TBASE_wPtrSet{};
    for (const auto &spObject : this->RecursivelyGetSharedPointers(std::forward<TSET>(set)))
        pointerSet.insert(static_cast<TBASE_wPtr>(spObject));

    return pointerSet;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
template <typename T, typename>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::GetSharedPointerToMember(T &&arg) const
{
    return this->GetRegistry().GetSharedPointer(std::forward<T>(arg));
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::GetWeakPointer()
{
    return static_cast<TBASE_wPtr>(this->GetSharedPointer());
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE, typename TALIAS>
inline auto HierarchicalObjectTemplate<TBASE, TALIAS>::GetWeakPointer() const
{
    return static_cast<TBASE_wPtr_const>(this->GetSharedPointer());
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TALIAS>
template <typename TARCHIVE>
inline void HierarchicalObjectTemplate<TBASE, TALIAS>::save(TARCHIVE &archive) const
{
    archive(cereal::base_class<RegisteredObject>(this), m_daughterEdges, m_parentEdges, m_contained,
            m_containing, m_wpContaining, m_relatedDaughterEdges, m_relatedParentEdges, m_edges);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TBASE, typename TALIAS>
template <typename TARCHIVE>
inline void HierarchicalObjectTemplate<TBASE, TALIAS>::load(TARCHIVE &archive)
{
    archive(cereal::base_class<RegisteredObject>(this), m_daughterEdges, m_parentEdges, m_contained,
            m_containing, m_wpContaining, m_relatedDaughterEdges, m_relatedParentEdges, m_edges);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_HIERARCHICAL_OBJECT_TEMPLATE_IMPL_H