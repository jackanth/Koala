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
#include "koala/Registry/ObjectRegistry.h"
#include "koala/Templates/RegisteredObjectTemplate.h"

#include "cereal/access.hpp"
#include "cereal/types/base_class.hpp"
#include "cereal/types/polymorphic.hpp"

/**
 * @brief Foo class.
 */
class Foo : public kl::RegisteredObjectTemplate<Foo>, public std::enable_shared_from_this<Foo>
{
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

protected:
    /**
     * @brief Constructor.
     *
     * @param wpRegistry Weak pointer to the associated registry.
     * @param id Unique ID for the object.
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    Foo(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept;

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

    friend Registry;  ///< Alias for the object registry from the base class.
    friend class kl::Koala;
    friend class cereal::access;

    template <typename TA, typename TB>
    friend class kl::ObjectAssociation;

    template <typename TA, typename TB>
    friend class kl::RegisteredObjectTemplate;

private:
    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);

    /**
     * @brief Default constructor (for serialization purposes).
     */
    Foo() = default;
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TARCHIVE>
inline void Foo::serialize(TARCHIVE &archive)
{
    archive(cereal::base_class<RegisteredObject>(this));
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

CEREAL_REGISTER_TYPE(Foo)
CEREAL_REGISTER_TYPE(Foo::Registry)
CEREAL_REGISTER_TYPE(kl::ObjectAssociation<Foo>)

#endif  // #ifndef FOO_H