/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file src/Objects/TestObject.cxx
 *
 * @brief Implementation of the test object (TestObject) class.
 */

#include "TestObject.h"

TestObject::TestObject(Registry_wPtr wpRegistry, const kl::ID_t id, Koala_wPtr wpKoala) noexcept
    : HierarchicalObject(std::move_if_noexcept(wpRegistry), id, std::move_if_noexcept(wpKoala))
{
}