/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Definitions/ObjectDefinitions.h
 *
 * @brief Header file for koala object definitions.
 */

#ifndef KL_OBJECT_DEFINITIONS_H
#define KL_OBJECT_DEFINITIONS_H 1

/**
 * @brief Macro for stating aliases for a registered object.
 */
#define KL_OBJECT_ALIASES(objectType)                                      \
    using RefWrapper = RefWrapperTemplate<objectType>;                     \
    using ConstRefWrapper = ConstRefWrapperTemplate<objectType>;           \
    using UnorderedRefSet = UnorderedRefSetTemplate<objectType>;           \
    using UnorderedConstRefSet = UnorderedConstRefSetTemplate<objectType>; \
    using RefVector = RefVectorTemplate<objectType>;                       \
    using ConstRefVector = ConstRefVectorTemplate<objectType>

/**
 * @brief Macro for a function returning the is-cereal-serializable boolean.
 *
 * @param isSerializable THe is-cereal-serializable boolean.
 */
#define KL_IS_SERIALIZABLE(isSerializable)                                 \
    static constexpr auto IsCerealSerializable() noexcept->bool            \
    {                                                                      \
        static_assert(std::is_same<bool, decltype(isSerializable)>::value, \
                      "Argument of KL_IS_SERIALIZABLE must be boolean");   \
        return isSerializable;                                             \
    }

/**
 * @brief Macro for functions returning a shared pointer to this object.
 *
 * @param objectType The object type.
 */
#define KL_GET_SHARED_POINTER(objectType)                                                     \
    std::shared_ptr<std::decay_t<objectType>> GetSharedPointer() override                     \
    {                                                                                         \
        return std::dynamic_pointer_cast<std::decay_t<objectType>>(this->shared_from_this()); \
    }                                                                                         \
    std::shared_ptr<const std::decay_t<objectType>> GetSharedPointer() const override         \
    {                                                                                         \
        return std::dynamic_pointer_cast<const std::decay_t<objectType>>(                     \
            this->shared_from_this());                                                        \
    }

/**
 * @brief Macro for a function returning the printable name for an object.
 *
 * @param name The name.
 */
#define KL_PRINTABLE_NAME(name)                                                           \
    auto PrintableName() const->std::string override                                      \
    {                                                                                     \
        static_assert(std::is_constructible<std::string, decltype(name)>::value,          \
                      "Argument of KL_PRINTABLE_NAME must be std::string-constructible"); \
        return std::string{name};                                                         \
    }                                                                                     \
    static auto StaticPrintableName()                                                     \
    {                                                                                     \
        static_assert(std::is_constructible<std::string, decltype(name)>::value,          \
                      "Argument of KL_PRINTABLE_NAME must be std::string-constructible"); \
        return std::string{name};                                                         \
    }

/**
 * @brief Macro for a function returning the printable name for an object.
 *
 * @param identifier The identifier.
 */
#define KL_IDENTIFIER_STRING(identifier)                                                     \
    auto GetIdentifierString() const->std::string override                                   \
    {                                                                                        \
        static_assert(std::is_constructible<std::string, decltype(identifier)>::value,       \
                      "Argument of KL_IDENTIFIER_STRING must be std::string-constructible"); \
        return std::string{identifier};                                                      \
    }

/**
 * @brief Macro for a function returning the printable name for an object with lock.
 *
 * @param identifier The identifier.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_IDENTIFIER_STRING_LOCK(identifier, lockType, memberMutex)                         \
    auto GetIdentifierString() const->std::string override                                   \
    {                                                                                        \
        static_assert(std::is_constructible<std::string, decltype(identifier)>::value,       \
                      "Argument of KL_IDENTIFIER_STRING must be std::string-constructible"); \
        const auto koalaMutexLock = lockType{memberMutex};                                   \
        return std::string{identifier};                                                      \
    }

/**
 * @brief Macro for defining an enum entry from a table.
 *
 * @param enum The enum entry name.
 */
#define KL_DEFINE_ENUM_ENTRY(enum) enum,

/**
 * @brief Macro for stringizing an enum from a table.
 *
 * @param enum The enum entry name.
 */
#define KL_STRINGIZE_ENUM_SWITCH_CASE(enum) \
    case enum:                              \
        return std::string{#enum};

/**
 * @brief Macro for defining an enum entry from a table with the code.
 *
 * @param enum The enum entry name.
 * @param code The enum code.
 */
#define KL_DEFINE_ENUM_ENTRY_CODE(enum, code) enum = code,

/**
 * @brief Macro for stringizing an enum from a table with the code.
 *
 * @param enum The enum entry name.
 * @param code The enum code (not used here but used in table).
 */
#define KL_STRINGIZE_ENUM_SWITCH_CASE_CODE(enum, code) \
    case enum:                                         \
        return std::string{#enum};

/**
 * @brief Macro for defining an enum entry from a table with scope.
 *
 * @param scope The scope to use (not used here but used in table).
 * @param enum The enum entry name.
 */
#define KL_DEFINE_ENUM_ENTRY_SCOPED(scope, enum) enum,

/**
 * @brief Macro for stringizing an enum from a table with scope.
 *
 * @param scope The scope to use.
 * @param enum The enum entry name.
 */
#define KL_STRINGIZE_ENUM_SWITCH_CASE_SCOPED(scope, enum) \
    case scope enum:                                      \
        return std::string{#enum};

/**
 * @brief Macro for defining an enum entry from a table with scope.
 *
 * @param scope The scope to use (not used here but used in table).
 * @param enum The enum entry name.
 * @param code The enum code.
 */
#define KL_DEFINE_ENUM_ENTRY_CODE_SCOPED(scope, enum, code) enum = code,

/**
 * @brief Macro for stringizing an enum from a table with scope.
 *
 * @param scope The scope to use.
 * @param enum The enum entry name.
 * @param code The enum code (not used here but used in table).
 */
#define KL_STRINGIZE_ENUM_SWITCH_CASE_CODE_SCOPED(scope, enum, code) \
    case scope enum:                                                 \
        return std::string{#enum};

#endif  // #ifndef KL_OBJECT_DEFINITIONS_H