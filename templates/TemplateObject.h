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

#include "cereal/access.hpp"

/**
 * @brief Foo class.
 */
class Foo
{
public:
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

protected:
    /**
     * @brief Default constructor.
     */
    Foo() = default;

    friend class cereal::access;

private:
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

template <typename TARCHIVE>
inline void Foo::serialize(TARCHIVE &)
{
}

#endif  // #ifndef FOO_H