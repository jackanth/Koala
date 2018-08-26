/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/RangeBasedContainer.txx
 *
 * @brief Template implementation header file for objects related to the range based container
 * (RangeBasedContainer) class template.
 */

#ifndef KL_RANGE_BASED_CONTAINER_IMPL_H
#define KL_RANGE_BASED_CONTAINER_IMPL_H

namespace kl
{
template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
RangeBasedIterator<TCONTAINER, TBASE, TCAST, TOBJECT>::RangeBasedIterator(
    const RangeBasedContainerInstance &container, const std::size_t index,
    SharedPtrRetriver sharedPtrRetriever, FinalGetterFunction finalGetterFunction) noexcept
    : m_index{index},
      m_container{container},
      m_sharedPtrRetriever{std::move(sharedPtrRetriever)},
      m_finalGetterFunction{std::move(finalGetterFunction)}
{
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
inline auto RangeBasedIterator<TCONTAINER, TBASE, TCAST, TOBJECT>::operator!=(
    const RangeBasedIterator &other) const noexcept
{
    return (m_index != other.m_index);
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
inline auto &RangeBasedIterator<TCONTAINER, TBASE, TCAST, TOBJECT>::operator++() noexcept
{
    ++m_index;
    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
inline decltype(auto) RangeBasedIterator<TCONTAINER, TBASE, TCAST, TOBJECT>::operator*() const
{
    if (const auto spObject = m_sharedPtrRetriever(m_container.get(m_index)))
        return m_finalGetterFunction(std::dynamic_pointer_cast<TCAST_D>(spObject));

    KL_THROW("Failed to retrieve shared pointer to object during loop");
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
template <typename... TSETS>
auto RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::CreateObjectList(
    const TCONTAINER_D &objectSet, TSETS &&... objectSets) const
{
    auto objectList = this->CreateObjectList(objectSet);
    auto otherObjectsList = this->CreateObjectList(std::forward<TSETS>(objectSets)...);

    for (auto &&object : otherObjectsList) this->InsertIntoContainer(objectList, object);

    return objectList;
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
auto RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::CreateObjectList(
    TCONTAINER_D objectSet) const
{
    for (auto iter = objectSet.cbegin(); iter != objectSet.cend(); /* no iterator */)
    {
        if (!std::dynamic_pointer_cast<TCAST_D>(m_sharedPtrRetriever(*iter)) ||
            !m_objectValidityFunction(*iter))
            iter = objectSet.erase(iter);

        else
            ++iter;
    }

    return objectSet;
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
template <typename TCONT, typename T>
inline auto RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::InsertIntoContainer(
    TCONT &container, T &&object) const
    -> decltype(container.insert(std::forward<T>(object)), void())
{
    container.insert(std::forward<T>(object));
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
template <typename TCONT, typename T>
inline auto RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::InsertIntoContainer(
    TCONT &container, T &&object) const
    -> decltype(container.push_back(std::forward<T>(object)), void())
{
    container.push_back(std::forward<T>(object));
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
template <typename... TSETS>
RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::RangeBasedContainer(
    ObjectValidityFunction objectValidityFunction, SharedPtrRetriver sharedPtrRetriever,
    FinalGetterFunction finalGetterFunction, ReadLock readLock1, ReadLock readLock2,
    TSETS &&... objectSets)
    : m_maxIndex{SIZE_T(0UL)},
      m_containedObjects{},
      m_objectValidityFunction{std::move(objectValidityFunction)},
      m_sharedPtrRetriever{std::move(sharedPtrRetriever)},
      m_finalGetterFunction{std::move(finalGetterFunction)},
      m_readLock1{std::move(readLock1)},
      m_readLock2{std::move(readLock2)}

{
    static_assert(TypeExpanderCondType<std::is_same<std::decay_t<TSETS>, TCONTAINER_D>...>::value,
                  "Not all sets passed to the range based container were of the type TCONTAINER");

    m_containedObjects = this->CreateObjectList(std::forward<TSETS>(objectSets)...);
    m_maxIndex = m_containedObjects.size();
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
inline auto RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::begin() const noexcept
{
    return RangeBasedIteratorInstance{*this, SIZE_T(0UL), m_sharedPtrRetriever,
                                      m_finalGetterFunction};
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
inline auto RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::end() const noexcept
{
    return RangeBasedIteratorInstance{*this, m_maxIndex, m_sharedPtrRetriever,
                                      m_finalGetterFunction};
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
inline decltype(auto) RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::get(
    const std::size_t index) const
{
    return *std::next(m_containedObjects.cbegin(), static_cast<std::int32_t>(index));
}

//--------------------------------------------------------------------------------------------------

template <typename TCONTAINER, typename TBASE, typename TCAST, typename TOBJECT>
inline auto RangeBasedContainer<TCONTAINER, TBASE, TCAST, TOBJECT>::size() const noexcept
{
    return m_containedObjects.size();
}
}  // namespace kl

#endif  // #ifndef KL_RANGE_BASED_CONTAINER_IMPL_H