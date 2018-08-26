/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/RangeBasedContainer.h
 *
 * @brief Header file for objects related to the range based container (RangeBasedContainer) class
 * template.
 */

#ifndef KL_RANGE_BASED_CONTAINER_H
#define KL_RANGE_BASED_CONTAINER_H

namespace kl
{
/**
 * @brief Forward declaration of RegisteredObjectTemplate abstract class template.
 */
template <typename TBASE, typename TALIAS>
class RegisteredObjectTemplate;

/**
 * @brief Forward declaration of HierarchicalObjectTemplate abstract class template.
 */
template <typename TBASE, typename TALIAS>
class HierarchicalObjectTemplate;

/**
 * @brief Forward declaration of ObjectRegistry class template.
 */
template <typename TBASE, typename TALIAS>
class ObjectRegistry;

/**
 * @brief Forward declaration of range-based container class template.
 */
template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
class RangeBasedContainer;

//--------------------------------------------------------------------------------------------------

/**
 * @brief A custom iterator class template for providing range-based for-loops. Particularly useful
 * for abstract base classes.
 */
template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
class RangeBasedIterator
{
private:
    using TCONTAINER_D = std::decay_t<TCONTAINER>;        ///< Alias for decayed TCONTAINER type.
    using TINSTANCE = typename TCONTAINER_D::value_type;  ///< Alias for TINSTANCE type.
    using TBASE_D = std::decay_t<TBASE>;                  ///< Alias for decayed TBASE type.
    using TCAST_D = std::decay_t<TCAST>;                  ///< Alias for decayed TCAST type.
    using TOBJECT_D = std::decay_t<TOBJECT>;              ///< Alias for decayed TOBJECT type.

    using RangeBasedContainerInstance =
        RangeBasedContainer<TCONTAINER_D, TBASE_D, TCAST_D,
                            TOBJECT_D>;  ///< Alias for specialized range-based container.

    using SharedPtrRetriver = std::function<std::shared_ptr<TBASE_D>(
        const TINSTANCE &)>;  ///< Alias for shared pointer retriever.
    using FinalGetterFunction = std::function<TOBJECT_D &(
        const std::shared_ptr<TCAST_D> &)>;  ///< Alias for final getter function.

    std::size_t m_index;                              ///< The iterator index.
    const RangeBasedContainerInstance &m_container;   ///< The corresponding range-based container.
    const SharedPtrRetriver m_sharedPtrRetriever;     ///< A function to retrieve a shared pointer.
    const FinalGetterFunction m_finalGetterFunction;  ///< A function to get the final object from
                                                      ///< the cast shared pointer.

protected:
    /**
     * @brief Constructor.
     *
     * @param container The corresponding range-based container.
     * @param index The iterator index.
     * @param sharedPtrRetriever A function to retrieve a shared pointer.
     * @param finalGetterFunction A function to get the final object from cast shared ptr.
     */
    RangeBasedIterator(const RangeBasedContainerInstance &container, const std::size_t index,
                       SharedPtrRetriver sharedPtrRetriever,
                       FinalGetterFunction finalGetterFunction) noexcept;

    friend RangeBasedContainerInstance;  ///< The specialized range-based container.

public:
    /**
     * @brief Default copy constructor.
     */
    RangeBasedIterator(const RangeBasedIterator &) = default;

    /**
     * @brief Default move constructor.
     */
    RangeBasedIterator(RangeBasedIterator &&) = default;

    /**
     * @brief Deleted copy assignment operator.
     */
    RangeBasedIterator &operator=(const RangeBasedIterator &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    RangeBasedIterator &operator=(RangeBasedIterator &&) = delete;

    /**
     * @brief Default destructor.
     */
    ~RangeBasedIterator() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Not-equal-to operator.
     *
     * @param other The other RangeBasedIterator object.
     */
    auto operator!=(const RangeBasedIterator &other) const noexcept;

    /**
     * @brief Increment operator.
     *
     * @return The incremented iterator.
     */
    auto &operator++() noexcept;

    /**
     * @brief Dereferencing operator.
     *
     * @return A dereferenced object.
     */
    decltype(auto) operator*() const;
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief A custom container class template for providing range-based for-loops. Particularly
 * useful for abstract base classes.
 */
template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
class RangeBasedContainer
{
private:
    /**
     * @brief A struct for helping to expand boolean-type-yielding operators like std::is_same<Ts>
     * for parameter packs.
     */
    template <typename... TCONDS>
    struct TypeExpanderCondType : std::true_type
    {
    };

    /**
     * @brief A struct for helping to expand boolean-type-yielding operators like std::is_same<Ts>
     * for parameter packs.
     */
    template <typename TCOND, typename... TCONDS>
    struct TypeExpanderCondType<TCOND, TCONDS...>
        : std::conditional<TCOND::value, TypeExpanderCondType<TCONDS...>, std::false_type>::type
    {
    };

    using TCONTAINER_D = std::decay_t<TCONTAINER>;        ///< Alias for decayed TCONTAINER type.
    using TINSTANCE = typename TCONTAINER_D::value_type;  ///< Alias for TINSTANCE type.
    using TBASE_D = std::decay_t<TBASE>;                  ///< Alias for decayed TBASE type.
    using TCAST_D = std::decay_t<TCAST>;                  ///< Alias for decayed TCAST type.
    using TOBJECT_D = std::decay_t<TOBJECT>;              ///< Alias for decayed TOBJECT type.

    using RangeBasedIteratorInstance =
        RangeBasedIterator<TCONTAINER_D, TBASE_D, TCAST_D,
                           TOBJECT_D>;  ///< Alias for specialized range-based iterator.

    using ObjectValidityFunction =
        std::function<bool(const TINSTANCE &)>;  ///< Alias for object validity function.
    using SharedPtrRetriver = std::function<std::shared_ptr<TBASE_D>(
        const TINSTANCE &)>;  ///< Alias for shared pointer retriever.
    using FinalGetterFunction = std::function<TOBJECT_D &(
        const std::shared_ptr<TCAST_D> &)>;  ///< Alias for final getter function.

    std::size_t m_maxIndex;                           ///< The number of contained objects.
    TCONTAINER_D m_containedObjects;                  ///< The contained objects.
    ObjectValidityFunction m_objectValidityFunction;  ///< A function to check instance's validity.
    SharedPtrRetriver m_sharedPtrRetriever;           ///< A function to retrieve a shared pointer.
    FinalGetterFunction m_finalGetterFunction;  ///< A function to get the final object from the
                                                ///< cast shared pointer.
    ReadLock m_readLock1;  ///< A first read lock for ensuring registry members aren't deleted
                           ///< during looping.
    ReadLock m_readLock2;  ///< A second read lock for ensuring registry members aren't deleted
                           ///< during looping.

    /**
     * @brief Create the object list from the sets of sets by removing all invalid objects.
     *
     * @param objectSet The object set.
     * @param objectSets The object sets.
     */
    template <typename... TSETS>
    auto CreateObjectList(const TCONTAINER_D &objectSet, TSETS &&... objectSets) const;

    /**
     * @brief Create the object list from the set by remove all invalid objects.
     *
     * @param objectSet The object set.
     */
    auto CreateObjectList(TCONTAINER_D objectSet) const;

    /**
     * @brief Insert a value into a container.
     *
     * @param container The container.
     * @param object The value to insert.
     */
    template <typename TCONT, typename T>
    auto InsertIntoContainer(TCONT &container, T &&object) const
        -> decltype(container.insert(std::forward<T>(object)), void());

    /**
     * @brief Insert a value into a container.
     *
     * @param container The container.
     * @param object The value to insert.
     */
    template <typename TCONT, typename T>
    auto InsertIntoContainer(TCONT &container, T &&object) const
        -> decltype(container.push_back(std::forward<T>(object)), void());

protected:
    /**
     * @brief Constructor.
     *
     * @param objectValidityFunction A function to check instance's validity.
     * @param sharedPtrRetriever A function to retrieve a shared pointer.
     * @param finalGetterFunction A function to get the final object from cast shared ptr.
     * @param readLock1 The first mutex read lock.
     * @param readLock2 The second mutex read lock.
     * @param objectSets The object sets.
     */
    template <typename... TSETS>
    RangeBasedContainer(ObjectValidityFunction objectValidityFunction,
                        SharedPtrRetriver sharedPtrRetriever,
                        FinalGetterFunction finalGetterFunction, ReadLock readLock1,
                        ReadLock readLock2, TSETS &&... objectSets);

    friend RangeBasedIteratorInstance;  ///< The specialized range-based iterator.

    template <typename TA, typename TB>
    friend class RegisteredObjectTemplate;

    template <typename TA, typename TB>
    friend class HierarchicalObjectTemplate;

    template <typename TA, typename TB>
    friend class ObjectRegistry;

public:
    /**
     * @brief Deleted copy constructor.
     */
    RangeBasedContainer(const RangeBasedContainer &) = delete;

    /**
     * @brief Default move constructor.
     */
    RangeBasedContainer(RangeBasedContainer &&) = default;

    /**
     * @brief Deleted copy assignment operator.
     */
    RangeBasedContainer &operator=(const RangeBasedContainer &) = delete;

    /**
     * @brief Default move assignment operator.
     */
    RangeBasedContainer &operator=(RangeBasedContainer &&) = default;

    /**
     * @brief Default destructor.
     */
    ~RangeBasedContainer() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief The container begin method.
     *
     * @return The iterator at index 0.
     */
    auto begin() const noexcept;

    /**
     * @brief The container end method.
     *
     * @return The iterator at the maximum index.
     */
    auto end() const noexcept;

    /**
     * @brief The container get method.
     *
     * @param index The index of the object to get.
     *
     * @return The container member.
     */
    decltype(auto) get(const std::size_t index) const;

    /**
     * @brief The container size method.
     *
     * @return The number of contained items.
     */
    auto size() const noexcept;
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/RangeBasedContainer.txx"

#endif  // #ifndef KL_RANGE_BASED_CONTAINER_H
