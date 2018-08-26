/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE',
 * which is part of source code package.
 */
/// @endcond

/**
 * @file src/Objects/Foo.cxx
 *
 * @brief Implementation of the foo (Foo) class.
 */

#include "Objects/Foo.h"

Foo::Foo(Registry_wPtr wpRegistry, const ID_t id, Koala_wPtr wpKoala) noexcept
    : HierarchicalObject(std::move_if_noexcept(wpRegistry), id, std::move_if_noexcept(wpKoala))
{
}