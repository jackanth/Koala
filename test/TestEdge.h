/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/test/TestEdge.h
 *
 * @brief Header file for the test edge (TestEdge) class.
 */

#ifndef TEST_EDGE_H
#define TEST_EDGE_H 1

#include "TestObject.h"
#include "koala/Registry/HierarchicalEdge.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/polymorphic.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

namespace kl
{
/**
 * @brief TestEdge class.
 */
class TestEdge : public HierarchicalEdge<TestEdge, TestObject, true>
{
public:
    /**
     * @brief Default copy constructor.
     */
    TestEdge(const TestEdge &) = default;

    /**
     * @brief Default move constructor.
     */
    TestEdge(TestEdge &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    TestEdge &operator=(const TestEdge &) = default;

    /**
     * @brief Default move assignment operator.
     */
    TestEdge &operator=(TestEdge &&) = default;

    /**
     * @brief Default destructor.
     */
    ~TestEdge() = default;

protected:
    using HierarchicalEdge<TestEdge, TestObject,
                           true>::HierarchicalEdge;  ///< Use base class constructor.

    /**
     * @brief Default constructor (for serialization purposes).
     */
    TestEdge() = default;

    /**
     * @brief Get the graph edge style.
     *
     * @return The graph edge style.
     */
    STYLE GetGraphEdgeStyle() const override;

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

inline TestEdge::STYLE TestEdge::GetGraphEdgeStyle() const { return STYLE::DASHED; }

}  // namespace kl

#endif  // #ifndef KL_HIERARCHICAL_EDGE_H