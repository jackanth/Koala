/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/DebugUtility.h
 *
 * @brief Header file for the debug utility (DebugUtility) class.
 */

#ifndef KOALA_DEBUG_UTILITY_H
#define KOALA_DEBUG_UTILITY_H 1

#include "koala/Koala/Koala.h"

namespace kl
{
/**
 * @brief DebugUtility class.
 */
class DebugUtility
{
public:
    /**
     * @brief Deleted constructor.
     */
    DebugUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    DebugUtility(const DebugUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    DebugUtility(DebugUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    DebugUtility &operator=(const DebugUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    DebugUtility &operator=(DebugUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~DebugUtility() = delete;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get some information about the objects associated with a given object.
     *
     * @param colourize Whether to colourize the output.
     * @param object The object.
     * @param objects The objects.
     *
     * @return A vector of strings.
     */
    template <typename TOBJECT, typename... TOBJECTS>
    static auto GetObjectAssociationInformation(const bool colourize, TOBJECT &&object,
                                                TOBJECTS &&... objects);

    /**
     * @brief Get some information about the objects associated with a given object.
     *
     * @param colourize Whether to colourize the output.
     * @param object The object.
     *
     * @return A vector of strings.
     */
    template <typename TOBJECT>
    static auto GetObjectAssociationInformation(const bool colourize, TOBJECT &&object);

    /**
     * @brief Get some information about the objects associated with objects of a given type.
     *
     * @param colourize Whether to colourize the output.
     * @param koala The instance of Koala.
     *
     * @return A vector of strings.
     */
    template <typename TOBJECTA, typename TOBJECTB, typename... TOBJECTS>
    static auto GetRegistryAssociationInformation(const bool colourize, const Koala &koala);

    /**
     * @brief Get some information about the objects associated with objects of a given type.
     *
     * @param colourize Whether to colourize the output.
     * @param koala The instance of Koala.
     *
     * @return A vector of strings.
     */
    template <typename TOBJECT>
    static auto GetObjectAssociationInformation(const bool colourize, const Koala &koala);

    /**
     * @brief Get some information about the objects associated with objects of a given type.
     *
     * @param colourize Whether to colourize the output.
     * @param koala The instance of Koala.
     *
     * @return A vector of strings.
     */
    template <typename TOBJECT>
    static auto GetRegistryAssociationInformation(const bool colourize, const Koala &koala);

private:
    /**
     * @brief Get the longest name lengths.
     *
     * @param object The object.
     *
     * @return The longest type name, longest ID, longest registry name and longest identifier (all
     * up to maxima).
     */
    template <typename TOBJECT>
    static std::tuple<std::size_t, std::size_t, std::size_t, std::size_t> GetLongestNames(
        TOBJECT &&object);

    /**
     * @brief Populate the information vector.
     *
     * @param informationVector The information vector.
     * @param object The object.
     * @param longestTypeName The longest type name.
     * @param longestId The longest ID.
     * @param longestRegistryName The longest registry name.
     * @param longestIdentifier The longest identifier.
     * @param colourize Whether to colourize the output.
     */
    template <typename TOBJECT>
    static void PopulateInformationVector(StringVector &informationVector, TOBJECT &&object,
                                          const std::size_t longestTypeName,
                                          const std::size_t longestId,
                                          const std::size_t longestRegistryName,
                                          const std::size_t longestIdentifier,
                                          const bool colourize);
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Utilities/DebugUtility.txx"

#endif  // #ifndef KOALA_DEBUG_REPORT_UTILITY_H