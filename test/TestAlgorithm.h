/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/test/TestAlgorithm.h
 *
 * @brief Header file for the test algorithm (TestAlgorithm) class.
 */

#ifndef KL_TEST_ALGORITHM_H
#define KL_TEST_ALGORITHM_H 1

#include "koala/Algorithm.h"

namespace kl
{
/**
 * @brief TestAlgorithm class.
 */
class TestAlgorithm : public Algorithm
{
public:
    /**
     * @brief Deleted copy constructor.
     */
    TestAlgorithm(const TestAlgorithm &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    TestAlgorithm(TestAlgorithm &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    TestAlgorithm &operator=(const TestAlgorithm &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    TestAlgorithm &operator=(TestAlgorithm &&) = delete;

    /**
     * @brief Default destructor.
     */
    ~TestAlgorithm() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a printable name for the object.
     *
     * @return A printable name for the object.
     */
    KL_PRINTABLE_NAME("TestAlgorithm");

    /**
     * @brief Get a string that identifies a given instantiation of the object.
     *
     * @return A string that identifies a given instantiation of the object.
     */
    KL_IDENTIFIER_STRING(this->HasAlias() ? this->Alias() : std::string{});

protected:
    /**
     * @brief Constructor.
     *
     * @param wpRegistry Weak pointer to the associated registry.
     * @param id Unique ID for the object.
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    TestAlgorithm(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Run the algorithm.
     *
     * @return Success.
     */
    bool Run() override;

    friend Registry;  ///< Alias for the object registry from the base class.
    friend class Koala;
};
}  // namespace kl

#endif  // #ifndef KL_TEST_ALGORITHM_H