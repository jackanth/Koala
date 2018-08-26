/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Definitions/RegistryDefinitions.h
 *
 * @brief Header file for koala registry definitions.
 */

#ifndef KL_REGISTRY_DEFINITIONS_H
#define KL_REGISTRY_DEFINITIONS_H 1

/**
 * @brief Macro for creating an object.
 *
 * @param objectType The object type.
 * @param ... The args to pass on.
 */
#define KL_CREATE(objectType, ...)                          \
    this->GetKoala()                                        \
        .template FetchRegistry<std::decay_t<objectType>>() \
        .template Create<objectType>(__VA_ARGS__)

/**
 * @brief Macro for creating an object by alias.
 *
 * @param objectType The object type.
 * @param ... The args to pass on.
 */
#define KL_CREATE_BY_ALIAS(objectType, ...)                 \
    this->GetKoala()                                        \
        .template FetchRegistry<std::decay_t<objectType>>() \
        .template CreateByAlias<std::decay_t<objectType>>(__VA_ARGS__)

/**
 * @brief Macro for getting an object.
 *
 * @param objectType The object type.
 * @param arg The arg to pass on.
 */
#define KL_GET(objectType, arg)                             \
    this->GetKoala()                                        \
        .template FetchRegistry<std::decay_t<objectType>>() \
        .template Get<std::decay_t<objectType>>(arg)

/**
 * @brief Macro for getting all the objects in a given registry as a list of reference wrappers.
 *
 * @param objectType The object type.
 */
#define KL_GET_ALL_LIST(objectType)                         \
    this->GetKoala()                                        \
        .template FetchRegistry<std::decay_t<objectType>>() \
        .template GetAllList<std::decay_t<objectType>>()

/**
 * @brief Macro for getting all the objects in a given registry.
 *
 * @param objectType The object type.
 */
#define KL_GET_ALL(objectType)                              \
    this->GetKoala()                                        \
        .template FetchRegistry<std::decay_t<objectType>>() \
        .template GetAll<std::decay_t<objectType>>()

/**
 * @brief Macro for deleting an object.
 *
 * @param objectType
 * @param arg The arg to pass on.
 */
#define KL_DELETE(objectType, arg) \
    this->GetKoala().template FetchRegistry<std::decay_t<objectType>>().Delete(arg)

/**
 * @brief Macro for deleting all the objects from a given registry.
 *
 * @param objectType The object type.
 */
#define KL_DELETE_ALL(objectType) \
    this->GetKoala().template FetchRegistry<std::decay_t<objectType>>().DeleteAll()

/**
 * @brief Macro for getting an object alias.
 *
 * @param objectType The object type.
 * @param arg The arg to pass on.
 */
#define KL_GET_ALIAS(objectType, arg)                       \
    this->GetKoala()                                        \
        .template FetchRegistry<std::decay_t<objectType>>() \
        .template GetObjectAlias<std::decay_t<objectType>>(arg)

/**
 * @brief Macro for finding out whether an object exists.
 *
 * @param objectType The object type.
 * @param arg The arg to pass on.
 */
#define KL_EXISTS(objectType, arg)                          \
    this->GetKoala()                                        \
        .template FetchRegistry<std::decay_t<objectType>>() \
        .template DoesObjectExist<std::decay_t<objectType>>(arg)

/**
 * @brief Macro for forming an association between two variables.
 *
 * @param thisObject This object.
 * @param otherObject The other object.
 */
#define KL_ASSOCIATE(thisObject, otherObject) \
    thisObject.template Associate<std::decay_t<decltype(thisObject)>>(otherObject)

/**
 * @brief Macro for forming an association between two variables with an indicator.
 *
 * @param thisObject This object.
 * @param otherObject The other object.
 * @param indicator The indicator.
 */
#define KL_ASSOCIATE_WITH_INDICATOR(thisObject, otherObject, indicator) \
    thisObject.template Associate<std::decay_t<decltype(thisObject)>>(otherObject, indicator)

/**
 * @brief Macro for dissolving an association between two variables.
 *
 * @param thisObject This object.
 * @param otherObject The other object.
 */
#define KL_DISSOCIATE(thisObject, otherObject) \
    thisObject.template Dissociate<std::decay_t<decltype(thisObject)>>(otherObject)

/**
 * @brief Macro for dissolving an association between two variables with an indicator.
 *
 * @param thisObject This object.
 * @param otherObject The other object.
 * @param indicator The indicator.
 */
#define KL_DISSOCIATE_WITH_INDICATOR(thisObject, otherObject, indicator) \
    thisObject.template Dissociate<std::decay_t<decltype(thisObject)>>(otherObject, indicator)

#endif  // #ifndef KL_REGISTRY_DEFINITIONS_H