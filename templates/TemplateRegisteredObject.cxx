/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file src/Objects/Foo.cxx
 *
 * @brief Implementation of the foo (Foo) class.
 */

#include "Objects/Foo.h"

Foo::Foo(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept
    : RegisteredObject(std::move_if_noexcept(wpRegistry), id, std::move_if_noexcept(wpKoala))
{
}