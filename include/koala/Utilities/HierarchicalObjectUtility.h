/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/HierarchicalObjectUtility.h
 *
 * @brief Header file for the hierarchical object utility (HierarchicalObjectUtility) class.
 */

#ifndef KL_HIERARCHICAL_OBJECT_UTILITY_H
#define KL_HIERARCHICAL_OBJECT_UTILITY_H 1

namespace kl
{
/**
 * @brief HierarchicalObjectUtility class.
 */
class HierarchicalObjectUtility
{
public:
    /**
     * @brief Deleted constructor.
     */
    HierarchicalObjectUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    HierarchicalObjectUtility(const HierarchicalObjectUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    HierarchicalObjectUtility(HierarchicalObjectUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    HierarchicalObjectUtility &operator=(const HierarchicalObjectUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    HierarchicalObjectUtility &operator=(HierarchicalObjectUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~HierarchicalObjectUtility() = delete;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Recurse over daughters of a hierarchical object.
     *
     * @param obj The hierarchical object.
     * @param operationFn The operation to apply to the daughter.
     */
    template <typename TDESIREDDAUGHTER, typename... TDAUGHTERS, typename TOBJ>
    static void RecurseOverDaughters(TOBJ &obj,
                                     const std::function<void(TDESIREDDAUGHTER &)> &operationFn);

    /**
     * @brief Recurse over daughters of a hierarchical object with a condition.
     *
     * @param obj The hierarchical object.
     * @param conditionFn The condition under which to loop over a daughter.
     * @param operationFn The operation to apply to the daughter.
     */
    template <typename TDESIREDDAUGHTER, typename TDAUGHTER, typename TOBJ>
    static void RecurseOverDaughters(TOBJ &obj,
                                     const std::function<bool(const TDAUGHTER &)> &conditionFn,
                                     const std::function<void(TDESIREDDAUGHTER &)> &operationFn);

    /**
     * @brief Recurse over parents of a hierarchical object.
     *
     * @param obj The hierarchical object.
     * @param operationFn The operation to apply to the parent.
     */
    template <typename TDESIREDPARENT, typename... TPARENTS, typename TOBJ>
    static void RecurseOverParents(TOBJ &obj,
                                   const std::function<void(TDESIREDPARENT &)> &operationFn);

    /**
     * @brief Recurse over parents of a hierarchical object with a condition.
     *
     * @param obj The hierarchical object.
     * @param conditionFn The condition under which to loop over a parent.
     * @param operationFn The operation to apply to the parent.
     */
    template <typename TDESIREDPARENT, typename TPARENT, typename TOBJ>
    static void RecurseOverParents(TOBJ &obj,
                                   const std::function<bool(const TPARENT &)> &conditionFn,
                                   const std::function<void(TDESIREDPARENT &)> &operationFn);

    /**
     * @brief Recurse over contained objects of a hierarchical object.
     *
     * @param obj The hierarchical object.
     * @param operationFn The operation to apply to the contained object.
     */
    template <typename TDESIREDCONTAINED, typename... TCONTAINEDS, typename TOBJ>
    static void RecurseOverContained(TOBJ &obj,
                                     const std::function<void(TDESIREDCONTAINED &)> &operationFn);

    /**
     * @brief Recurse over contained objects of a hierarchical object with a condition.
     *
     * @param obj The hierarchical object.
     * @param conditionFn The condition under which to loop over a contained object.
     * @param operationFn The operation to apply to the contained object.
     */
    template <typename TDESIREDCONTAINED, typename TCONTAINED, typename TOBJ>
    static void RecurseOverContained(TOBJ &obj,
                                     const std::function<bool(const TCONTAINED &)> &conditionFn,
                                     const std::function<void(TDESIREDCONTAINED &)> &operationFn);

private:
    /**
     * @brief Recurse over the daughters of a hierarchical object (implementation method).
     *
     * @param obj The hierarchical object.
     * @param operationFn The operation to apply to the daughter.
     * @param numShifts The number of template shifts performed.
     */
    template <typename TDESIREDDAUGHTER, typename TDAUGHTER, typename... TDAUGHTERS, typename TOBJ>
    static void RecurseOverDaughtersImpl(TOBJ &obj,
                                         const std::function<void(TDESIREDDAUGHTER &)> &operationFn,
                                         const std::size_t numShifts);

    /**
     * @brief Recurse over the daughters of a hierarchical object with a condition (implementation
     * method).
     *
     * @param obj The hierarchical object.
     * @param conditionFn The condition under which to loop over a daughter.
     * @param operationFn The operation to apply to the daughter.
     */
    template <typename TDESIREDDAUGHTER, typename TDAUGHTER, typename TOBJ>
    static void RecurseOverDaughtersImpl(
        TOBJ &obj, const std::function<bool(const TDAUGHTER &)> &conditionFn,
        const std::function<void(TDESIREDDAUGHTER &)> &operationFn);

    /**
     * @brief Recurse over the parents of a hierarchical object (implementation method).
     *
     * @param obj The hierarchical object.
     * @param operationFn The operation to apply to the parent.
     * @param numShifts The number of template shifts performed.
     */
    template <typename TDESIREDPARENT, typename TPARENT, typename... TPARENTS, typename TOBJ>
    static void RecurseOverParentsImpl(TOBJ &obj,
                                       const std::function<void(TDESIREDPARENT &)> &operationFn,
                                       const std::size_t numShifts);

    /**
     * @brief Recurse over the parents of a hierarchical object with a condition (implementation
     * method).
     *
     * @param obj The hierarchical object.
     * @param conditionFn The condition under which to loop over a parent.
     * @param operationFn The operation to apply to the parent.
     */
    template <typename TDESIREDPARENT, typename TPARENT, typename TOBJ>
    static void RecurseOverParentsImpl(TOBJ &obj,
                                       const std::function<bool(const TPARENT &)> &conditionFn,
                                       const std::function<void(TDESIREDPARENT &)> &operationFn);

    /**
     * @brief Recurse over the contained objects of a hierarchical object (implementation method).
     *
     * @param obj The hierarchical object.
     * @param operationFn The operation to apply to the contained object.
     * @param numShifts The number of template shifts performed.
     */
    template <typename TDESIREDCONTAINED, typename TCONTAINED, typename... TCONTAINEDS,
              typename TOBJ>
    static void RecurseOverContainedImpl(
        TOBJ &obj, const std::function<void(TDESIREDCONTAINED &)> &operationFn,
        const std::size_t numShifts);

    /**
     * @brief Recurse over the contained objects of a hierarchical object with a condition
     * (implementation method).
     *
     * @param obj The hierarchical object.
     * @param conditionFn The condition under which to loop over a contained object.
     * @param operationFn The operation to apply to the contained object.
     */
    template <typename TDESIREDCONTAINED, typename TCONTAINED, typename TOBJ>
    static void RecurseOverContainedImpl(
        TOBJ &obj, const std::function<bool(const TCONTAINED &)> &conditionFn,
        const std::function<void(TDESIREDCONTAINED &)> &operationFn);

    /**
     * @brief Test the validity of a hierarchical object.
     *
     * @param object The object to test.
     */
    template <typename TOBJ>
    static void TestHierarchicalObject(TOBJ &&object);
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Utilities/HierarchicalObjectUtility.txx"

#endif  // #ifndef KL_HIERARCHICAL_OBJECT_UTILITY_H