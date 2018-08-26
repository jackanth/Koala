/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/test/TestObject.h
 *
 * @brief Header file for the test object (TestObject) class.
 */

#ifndef KL_TEST_OBJECT_H
#define KL_TEST_OBJECT_H 1

#include "koala/Definitions.h"
#include "koala/Registry/ObjectAssociation.h"
#include "koala/Templates/HierarchicalObjectTemplate.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#include "cereal/types/base_class.hpp"
#include "cereal/types/polymorphic.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

/**
 * @brief TestObject class.
 */
class TestObject : public kl::HierarchicalObjectTemplate<TestObject>,
                   public std::enable_shared_from_this<TestObject>
{
protected:
    /**
     * @brief Constructor.
     *
     * @param wpRegistry Weak pointer to the associated registry.
     * @param id Unique ID for the object.
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    TestObject(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a shared pointer to the object.
     *
     * @return A shared pointer to the object.
     */
    KL_GET_SHARED_POINTER(TestObject);

    /**
     * @brief Get the is-cereal-serializable boolean.
     *
     * @return The is-cereal-serializable boolean.
     */
    KL_IS_SERIALIZABLE(true);

    /**
     * @brief Get a string to represent the state of the object in a graph node.
     *
     * @return A string to represent the state of the object in a graph node.
     */
    std::string GetGraphNodeLabel() const noexcept override;

    friend Registry;  ///< Alias for the object registry from the base class.
    friend class kl::Koala;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

    template <typename T>
    friend class kl::HierarchicalVisualizationUtility;

    template <typename TA, typename TB>
    friend class kl::ObjectAssociation;

    template <typename TA, typename TB>
    friend class kl::RegisteredObjectTemplate;

public:
    KL_OBJECT_ALIASES(TestObject);  ///< Aliases for reference wrappers, unordered sets and vectors.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Default copy constructor.
     */
    TestObject(const TestObject &) = default;

    /**
     * @brief Default move constructor.
     */
    TestObject(TestObject &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    TestObject &operator=(const TestObject &) = default;

    /**
     * @brief Default move assignment operator.
     */
    TestObject &operator=(TestObject &&) = default;

    /**
     * @brief Default destructor.
     */
    ~TestObject() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a printable name for the object.
     *
     * @return A printable name for the object.
     */
    KL_PRINTABLE_NAME("TestObject");

    /**
     * @brief Get a string that identifies a given instantiation of the object.
     *
     * @return A string that identifies a given instantiation of the object.
     */
    KL_IDENTIFIER_STRING(this->HasAlias() ? this->Alias() : std::string{});

private:
#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Default constructor (for serialization purposes).
     */
    TestObject() = default;

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

inline std::string TestObject::GetGraphNodeLabel() const noexcept
{
    return this->HasAlias() ? this->Alias() : std::to_string(this->ID());
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void TestObject::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<HierarchicalObject>(this));
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
CEREAL_REGISTER_TYPE(TestObject)
CEREAL_REGISTER_TYPE(TestObject::Registry)
CEREAL_REGISTER_TYPE(TestObject::HierarchicalObject)
CEREAL_REGISTER_TYPE(kl::ObjectAssociation<TestObject>)
#endif  // #ifdef KOALA_ENABLE_CEREAL

#endif  // #ifndef KL_TEST_OBJECT_H
