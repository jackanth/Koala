/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file include/Objects/Foo.h
 *
 * @brief Header file for the foo (Foo) class.
 */

#ifndef FOO_H
#define FOO_H 1

#include "koala/Definitions.h"
#include "koala/Registry/ObjectAssociation.h"
#include "koala/Templates/HierarchicalObjectTemplate.h"

#include "cereal/access.hpp"
#include "cereal/types/base_class.hpp"
#include "cereal/types/polymorphic.hpp"

/**
 * @brief Foo class.
 */
class Foo : public kl::HierarchicalObjectTemplate<Foo>, public std::enable_shared_from_this<Foo>
{
protected:
    /**
     * @brief Constructor.
     *
     * @param wpRegistry Weak pointer to the associated registry.
     * @param id Unique ID for the object.
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    Foo(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a shared pointer to the object.
     *
     * @return A shared pointer to the object.
     */
    KL_GET_SHARED_POINTER(Foo);

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

    template <typename T>
    friend class kl::HierarchicalVisualizationUtility;
    friend class cereal::access;

    template <typename TA, typename TB>
    friend class kl::ObjectAssociation;

    template <typename TA, typename TB>
    friend class kl::RegisteredObjectTemplate;

public:
    KL_OBJECT_ALIASES(Foo);  ///< Aliases for reference wrappers, unordered sets and vectors.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Default copy constructor.
     */
    Foo(const Foo &) = default;

    /**
     * @brief Default move constructor.
     */
    Foo(Foo &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    Foo &operator=(const Foo &) = default;

    /**
     * @brief Default move assignment operator.
     */
    Foo &operator=(Foo &&) = default;

    /**
     * @brief Default destructor.
     */
    ~Foo() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get a printable name for the object.
     *
     * @return A printable name for the object.
     */
    KL_PRINTABLE_NAME("Foo");

    /**
     * @brief Get a string that identifies a given instantiation of the object.
     *
     * @return A string that identifies a given instantiation of the object.
     */
    KL_IDENTIFIER_STRING(this->HasAlias() ? this->Alias() : std::string{});

private:
    /**
     * @brief Default constructor (for serialization purposes).
     */
    Foo() = default;

    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline std::string Foo::GetGraphNodeLabel() const noexcept { return std::string{}; }

//--------------------------------------------------------------------------------------------------

template <typename TARCHIVE>
inline void Foo::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<HierarchicalObject>(this));
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

CEREAL_REGISTER_TYPE(Foo)
CEREAL_REGISTER_TYPE(Foo::Registry)
CEREAL_REGISTER_TYPE(Foo::HierarchicalObject)
CEREAL_REGISTER_TYPE(kl::ObjectAssociation<Foo>)

#endif  // #ifndef FOO_H
