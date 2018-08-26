/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/HierarchicalObjectUtility.txx
 *
 * @brief Template implementation header file for the hierarchical object utility
 * (HierarchicalObjectUtility) class.
 */

#ifndef KL_HIERARCHICAL_OBJECT_UTILITY_IMPL_H
#define KL_HIERARCHICAL_OBJECT_UTILITY_IMPL_H 1

namespace kl
{
template <typename TDESIREDDAUGHTER, typename... TDAUGHTERS, typename TOBJ>
inline void HierarchicalObjectUtility::RecurseOverDaughters(
    TOBJ &obj, const std::function<void(TDESIREDDAUGHTER &)> &operationFn)
{
    RecurseOverDaughtersImpl<std::decay_t<TDESIREDDAUGHTER>, TDAUGHTERS...>(obj, operationFn,
                                                                            SIZE_T(0UL));
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDDAUGHTER, typename TDAUGHTER, typename TOBJ>
inline void HierarchicalObjectUtility::RecurseOverDaughters(
    TOBJ &obj, const std::function<bool(const TDAUGHTER &)> &conditionFn,
    const std::function<void(TDESIREDDAUGHTER &)> &operationFn)
{
    RecurseOverDaughtersImpl<std::decay_t<TDESIREDDAUGHTER>, std::decay_t<TDAUGHTER>>(
        obj, conditionFn, operationFn);
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDPARENT, typename... TPARENTS, typename TOBJ>
inline void HierarchicalObjectUtility::RecurseOverParents(
    TOBJ &obj, const std::function<void(TDESIREDPARENT &)> &operationFn)
{
    RecurseOverParentsImpl<std::decay_t<TDESIREDPARENT>, TPARENTS...>(obj, operationFn,
                                                                      SIZE_T(0UL));
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDPARENT, typename TPARENT, typename TOBJ>
inline void HierarchicalObjectUtility::RecurseOverParents(
    TOBJ &obj, const std::function<bool(const TPARENT &)> &conditionFn,
    const std::function<void(TDESIREDPARENT &)> &operationFn)
{
    RecurseOverParentsImpl<std::decay_t<TDESIREDPARENT>, std::decay_t<TPARENT>>(obj, conditionFn,
                                                                                operationFn);
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDCONTAINED, typename... TCONTAINEDS, typename TOBJ>
inline void HierarchicalObjectUtility::RecurseOverContained(
    TOBJ &obj, const std::function<void(TDESIREDCONTAINED &)> &operationFn)
{
    RecurseOverContainedImpl<std::decay_t<TDESIREDCONTAINED>, TCONTAINEDS...>(obj, operationFn,
                                                                              SIZE_T(0UL));
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDCONTAINED, typename TCONTAINED, typename TOBJ>
inline void HierarchicalObjectUtility::RecurseOverContained(
    TOBJ &obj, const std::function<bool(const TCONTAINED &)> &conditionFn,
    const std::function<void(TDESIREDCONTAINED &)> &operationFn)
{
    RecurseOverContainedImpl<std::decay_t<TDESIREDCONTAINED>, TCONTAINED>(obj, conditionFn,
                                                                          operationFn);
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TDESIREDDAUGHTER, typename TDAUGHTER, typename... TDAUGHTERS, typename TOBJ>
void HierarchicalObjectUtility::RecurseOverDaughtersImpl(
    TOBJ &obj, const std::function<void(TDESIREDDAUGHTER &)> &operationFn,
    const std::size_t numShifts)
{
    using TDESIREDDAUGHTER_D = std::decay_t<TDESIREDDAUGHTER>;
    using TDAUGHTER_D = std::decay_t<TDAUGHTER>;

    TestHierarchicalObject(obj);

    if (obj.GetDaughterList().empty()) return;

    for (auto &daughter : obj.template Daughters<TDAUGHTER_D>())
        RecurseOverDaughtersImpl<TDESIREDDAUGHTER_D, TDAUGHTER_D, TDAUGHTERS...>(
            daughter, operationFn, SIZE_T(0UL));

    if (sizeof...(TDAUGHTERS) > numShifts)
        RecurseOverDaughtersImpl<TDESIREDDAUGHTER_D, TDAUGHTERS..., TDAUGHTER_D>(
            obj, operationFn, numShifts + static_cast<size_t>(1UL));

    for (auto &daughter : obj.template Daughters<TDESIREDDAUGHTER_D>()) operationFn(daughter);
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDDAUGHTER, typename TDAUGHTER, typename TOBJ>
void HierarchicalObjectUtility::RecurseOverDaughtersImpl(
    TOBJ &obj, const std::function<bool(const TDAUGHTER &)> &conditionFn,
    const std::function<void(TDESIREDDAUGHTER &)> &operationFn)
{
    using TDESIREDDAUGHTER_D = std::decay_t<TDESIREDDAUGHTER>;
    using TDAUGHTER_D = std::decay_t<TDAUGHTER>;

    TestHierarchicalObject(obj);

    if (obj.GetDaughterList().empty()) return;

    for (auto &daughter : obj.template Daughters<TDAUGHTER_D>())
    {
        if (conditionFn(daughter))
            RecurseOverDaughtersImpl<TDESIREDDAUGHTER_D, TDAUGHTER_D>(daughter, conditionFn,
                                                                      operationFn);
    }

    for (auto &daughter : obj.template Daughters<TDESIREDDAUGHTER_D>()) operationFn(daughter);
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDPARENT, typename TPARENT, typename... TPARENTS, typename TOBJ>
void HierarchicalObjectUtility::RecurseOverParentsImpl(
    TOBJ &obj, const std::function<void(TDESIREDPARENT &)> &operationFn,
    const std::size_t numShifts)
{
    using TDESIREDPARENT_D = std::decay_t<TDESIREDPARENT>;
    using TPARENT_D = std::decay_t<TPARENT>;

    TestHierarchicalObject(obj);

    if (obj.GetParentList().empty()) return;

    for (auto &parent : obj.template Parents<TPARENT_D>())
        RecurseOverParentsImpl<TDESIREDPARENT_D, TPARENT_D, TPARENTS...>(parent, operationFn,
                                                                         SIZE_T(0UL));

    if (sizeof...(TPARENTS) > numShifts)
        RecurseOverParentsImpl<TDESIREDPARENT_D, TPARENTS..., TPARENT_D>(
            obj, operationFn, numShifts + static_cast<size_t>(1UL));

    for (auto &parent : obj.template Parents<TDESIREDPARENT_D>()) operationFn(parent);
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDPARENT, typename TPARENT, typename TOBJ>
void HierarchicalObjectUtility::RecurseOverParentsImpl(
    TOBJ &obj, const std::function<bool(const TPARENT &)> &conditionFn,
    const std::function<void(TDESIREDPARENT &)> &operationFn)
{
    using TDESIREDPARENT_D = std::decay_t<TDESIREDPARENT>;
    using TPARENT_D = std::decay_t<TPARENT>;

    TestHierarchicalObject(obj);

    if (obj.GetParentList().empty()) return;

    for (auto &parent : obj.template Parents<TPARENT_D>())
    {
        if (conditionFn(parent))
            RecurseOverParentsImpl<TDESIREDPARENT_D, TPARENT_D>(parent, conditionFn, operationFn);
    }

    for (auto &parent : obj.template Parents<TDESIREDPARENT_D>()) operationFn(parent);
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDCONTAINED, typename TCONTAINED, typename... TCONTAINEDS, typename TOBJ>
void HierarchicalObjectUtility::RecurseOverContainedImpl(
    TOBJ &obj, const std::function<void(TDESIREDCONTAINED &)> &operationFn,
    const std::size_t numShifts)
{
    using TDESIREDCONTAINED_D = std::decay_t<TDESIREDCONTAINED>;
    using TCONTAINED_D = std::decay_t<TCONTAINED>;

    TestHierarchicalObject(obj);

    if (obj.GetContainedList().empty()) return;

    for (auto &containedObj : obj.template Contained<TCONTAINED_D>())
    {
        RecurseOverContainedImpl<TDESIREDCONTAINED_D, TCONTAINED_D, TCONTAINEDS...>(
            containedObj, operationFn, SIZE_T(0UL));
    }

    if (sizeof...(TCONTAINEDS) > numShifts)
        RecurseOverContainedImpl<TDESIREDCONTAINED_D, TCONTAINEDS..., TCONTAINED_D>(
            obj, operationFn, numShifts + SIZE_T(1UL));

    for (auto &containedObj : obj.template Contained<TDESIREDCONTAINED_D>())
        operationFn(containedObj);
}

//--------------------------------------------------------------------------------------------------

template <typename TDESIREDCONTAINED, typename TCONTAINED, typename TOBJ>
void HierarchicalObjectUtility::RecurseOverContainedImpl(
    TOBJ &obj, const std::function<bool(const TCONTAINED &)> &conditionFn,
    const std::function<void(TDESIREDCONTAINED &)> &operationFn)
{
    using TDESIREDCONTAINED_D = std::decay_t<TDESIREDCONTAINED>;
    using TCONTAINED_D = std::decay_t<TCONTAINED>;

    TestHierarchicalObject(obj);

    if (obj.GetContainedList().empty()) return;

    for (auto &containedObj : obj.template Contained<TCONTAINED_D>())
    {
        if (conditionFn(containedObj))
            RecurseOverContainedImpl<TDESIREDCONTAINED_D, TCONTAINED_D>(containedObj, conditionFn,
                                                                        operationFn);
    }

    for (auto &containedObj : obj.template Contained<TDESIREDCONTAINED_D>())
        operationFn(containedObj);
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJ>
void HierarchicalObjectUtility::TestHierarchicalObject(TOBJ &&object)
{
    using TOBJ_D = std::decay_t<TOBJ>;
    static_assert(
        std::is_base_of<
            HierarchicalObjectTemplate<typename TOBJ_D::RegisteredObject::KoalaBaseType,
                                       typename TOBJ_D::RegisteredObject::KoalaAliasType>,
            TOBJ_D>::value,
        "Could not recurse over members because chosen object was not a public hierarchical "
        "object");

    if (!object.GetRegistry().template DoesObjectExist<TOBJ_D>(object))
        KL_THROW(
            "Could not recurse over members because chosen object did not exist in its own "
            "registry");
}
}  // namespace kl

#endif  // #ifndef KL_HIERARCHICAL_OBJECT_UTILITY_IMPL_H