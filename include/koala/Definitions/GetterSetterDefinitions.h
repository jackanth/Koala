/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Definitions/GetterSetterDefinitions.h
 *
 * @brief Header file for koala getter/setter definitions.
 */

#ifndef KL_GETTER_SETTER_DEFINITIONS_H
#define KL_GETTER_SETTER_DEFINITIONS_H 1

namespace kl
{
/**
 * @brief Forward declaration of the Lock class template.
 */
template <typename TLOCK, typename T>
class Lock;
}  // namespace kl

/**
 * @brief Macro for a simple getter function.
 *
 * @param typeQual The type qualifier.
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 */
#define KL_SIMPLE_GETTER(typeQual, fnName, memberVar, constQual) \
    auto typeQual fnName() constQual noexcept { return memberVar; }

/**
 * @brief Macro for a simple getter function with lock.
 *
 * @param typeQual The type qualifier.
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LOCK(typeQual, fnName, memberVar, constQual, lockType, memberMutex) \
    auto typeQual fnName() constQual noexcept                                                \
    {                                                                                        \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};                  \
        return memberVar;                                                                    \
    }

/**
 * @brief Macro for a simple getter function with two locks.
 *
 * @param typeQual The type qualifier.
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LOCKS(typeQual, fnName, memberVar, constQual, lockType1, memberMutex1, \
                               lockType2, memberMutex2)                                         \
    auto typeQual fnName() constQual noexcept                                                   \
    {                                                                                           \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};                  \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};                  \
        return memberVar;                                                                       \
    }

/**
 * @brief Macro for a simple getter lvalue function (for use with get-by-reference).
 *
 * @param returnTypeConstQual
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 */
#define KL_SIMPLE_GETTER_LVAL(returnTypeConstQual, fnName, memberVar, constQual) \
    auto returnTypeConstQual& fnName() constQual& noexcept { return memberVar; }

/**
 * @brief Macro for a simple getter lvalue function (for use with get-by-reference) with lock.
 *
 * @param returnTypeConstQual
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_LOCK(returnTypeConstQual, fnName, memberVar, constQual, lockType, \
                                   memberMutex)                                                 \
    auto returnTypeConstQual& fnName() constQual& noexcept                                      \
    {                                                                                           \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};                     \
        return memberVar;                                                                       \
    }

/**
 * @brief Macro for a simple getter lvalue function (for use with get-by-reference) with two locks.
 *
 * @param returnTypeConstQual
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_LOCKS(returnTypeConstQual, fnName, memberVar, constQual, lockType1, \
                                    memberMutex1, lockType2, memberMutex2)                        \
    auto returnTypeConstQual& fnName() constQual& noexcept                                        \
    {                                                                                             \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};                    \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};                    \
        return memberVar;                                                                         \
    }

/**
 * @brief Macro for a simple getter lvalue const and non-const functions (for use with
 * get-by-reference).
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 */
#define KL_SIMPLE_GETTER_LVAL_CONSTS(fnName, memberVar) \
    auto& fnName() & noexcept { return memberVar; }     \
    auto const& fnName() const& noexcept { return memberVar; }

/**
 * @brief Macro for a simple getter lvalue const and non-const functions (for use with
 * get-by-reference) with lock.
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_CONSTS_LOCK(fnName, memberVar, lockType, memberMutex) \
    auto& fnName() & noexcept                                                       \
    {                                                                               \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};         \
        return memberVar;                                                           \
    }                                                                               \
    auto const& fnName() const& noexcept                                            \
    {                                                                               \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};         \
        return memberVar;                                                           \
    }

/**
 * @brief Macro for a simple getter lvalue const and non-const functions (for use with
 * get-by-reference) with two locks.
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_CONSTS_LOCKS(fnName, memberVar, lockType1, memberMutex1, lockType2, \
                                           memberMutex2)                                          \
    auto& fnName() & noexcept                                                                     \
    {                                                                                             \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};                    \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};                    \
        return memberVar;                                                                         \
    }                                                                                             \
    auto const& fnName() const& noexcept                                                          \
    {                                                                                             \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};                    \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};                    \
        return memberVar;                                                                         \
    }

/**
 * @brief Macro for a simple getter lvalue const and non-const functions and rvalue function (for
 * use with get-by-reference).
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 */
#define KL_SIMPLE_GETTER_LVAL_CONSTS_RVAL(fnName, memberVar)   \
    auto& fnName() & noexcept { return memberVar; }            \
    auto const& fnName() const& noexcept { return memberVar; } \
    auto&& fnName() && noexcept { return std::move_if_noexcept(memberVar); }

/**
 * @brief Macro for a simple getter lvalue const and non-const functions and rvalue function (for
 * use with get-by-reference) with lock.
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_CONSTS_RVAL_LOCK(fnName, memberVar, lockType, memberMutex) \
    auto& fnName() & noexcept                                                            \
    {                                                                                    \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};              \
        return memberVar;                                                                \
    }                                                                                    \
    auto const& fnName() const& noexcept                                                 \
    {                                                                                    \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};              \
        return memberVar;                                                                \
    }                                                                                    \
    auto&& fnName() && noexcept                                                          \
    {                                                                                    \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};              \
        return std::move_if_noexcept(memberVar);                                         \
    }

/**
 * @brief Macro for a simple getter lvalue const and non-const functions and rvalue function (for
 * use with get-by-reference) with two locks.
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_CONSTS_RVAL_LOCKS(fnName, memberVar, lockType1, memberMutex1, \
                                                lockType2, memberMutex2)                    \
    auto& fnName() & noexcept                                                               \
    {                                                                                       \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};              \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};              \
        return memberVar;                                                                   \
    }                                                                                       \
    auto const& fnName() const& noexcept                                                    \
    {                                                                                       \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};              \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};              \
        return memberVar;                                                                   \
    }                                                                                       \
    auto&& fnName() && noexcept                                                             \
    {                                                                                       \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};              \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};              \
        return std::move_if_noexcept(memberVar);                                            \
    }

/**
 * @brief Macro for a simple move-getter rvalue function (for use with get-by-reference).
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 */
#define KL_SIMPLE_GETTER_RVAL(fnName, memberVar) \
    auto&& fnName() && noexcept { return std::move_if_noexcept(memberVar); }

/**
 * @brief Macro for a simple move-getter rvalue function (for use with get-by-reference) with lock.
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to set.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_GETTER_RVAL_LOCK(fnName, memberVar, lockType, memberMutex) \
    auto&& fnName() && noexcept                                              \
    {                                                                        \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};  \
        return std::move_if_noexcept(memberVar);                             \
    }

/**
 * @brief Macro for a simple move-getter rvalue function (for use with get-by-reference) with two
 * locks.
 *
 * @param fnName The name of the function.
 * @param memberVar The member variable to set.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_GETTER_RVAL_LOCKS(fnName, memberVar, lockType1, memberMutex1, lockType2, \
                                    memberMutex2)                                          \
    auto&& fnName() && noexcept                                                            \
    {                                                                                      \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};             \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};             \
        return std::move_if_noexcept(memberVar);                                           \
    }

/**
 * @brief Macro for a simple getter lvalue function and rvalue function (for use with
 * get-by-reference).
 *
 * @param returnTypeConstQual
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 */
#define KL_SIMPLE_GETTER_LVAL_RVAL(returnTypeConstQual, fnName, memberVar, constQual) \
    auto returnTypeConstQual& fnName() constQual& noexcept { return memberVar; }      \
    auto&& fnName() && noexcept { return std::move_if_noexcept(memberVar); }

/**
 * @brief Macro for a simple getter lvalue function and rvalue function (for use with
 * get-by-reference) with lock.
 *
 * @param returnTypeConstQual
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_RVAL_LOCK(returnTypeConstQual, fnName, memberVar, constQual, \
                                        lockType, memberMutex)                             \
    auto returnTypeConstQual& fnName() constQual& noexcept                                 \
    {                                                                                      \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};                \
        return memberVar;                                                                  \
    }                                                                                      \
    auto&& fnName() && noexcept                                                            \
    {                                                                                      \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};                \
        return std::move_if_noexcept(memberVar);                                           \
    }

/**
 * @brief Macro for a simple getter lvalue function and rvalue function (for use with
 * get-by-reference) with two locks.
 *
 * @param returnTypeConstQual
 * @param fnName The name of the function.
 * @param memberVar The member variable to get.
 * @param constQual The function const qualifier.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_GETTER_LVAL_RVAL_LOCKS(returnTypeConstQual, fnName, memberVar, constQual, \
                                         lockType1, memberMutex1, lockType2, memberMutex2)  \
    auto returnTypeConstQual& fnName() constQual& noexcept                                  \
    {                                                                                       \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};              \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};              \
        return memberVar;                                                                   \
    }                                                                                       \
    auto&& fnName() && noexcept                                                             \
    {                                                                                       \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};              \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};              \
        return std::move_if_noexcept(memberVar);                                            \
    }

/**
 * @brief Macro for a simple setter function.
 *
 * @param fnName The name of the function.
 * @param paramType The type of the parameter.
 * @param paramName The name of the parameter.
 * @param memberVar The member variable to set.
 */
#define KL_SIMPLE_SETTER(fnName, paramType, paramName, memberVar) \
    void fnName(paramType paramName) noexcept { memberVar = paramName; }

/**
 * @brief Macro for a simple setter function with lock.
 *
 * @param fnName The name of the function.
 * @param paramType The type of the parameter.
 * @param paramName The name of the parameter.
 * @param memberVar The member variable to set.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_SETTER_LOCK(fnName, paramType, paramName, memberVar, lockType, memberMutex) \
    void fnName(paramType paramName) noexcept                                                 \
    {                                                                                         \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};                   \
        memberVar = paramName;                                                                \
    }

/**
 * @brief Macro for a simple setter function with two locks.
 *
 * @param fnName The name of the function.
 * @param paramType The type of the parameter.
 * @param paramName The name of the parameter.
 * @param memberVar The member variable to set.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_SETTER_LOCKS(fnName, paramType, paramName, memberVar, lockType1, memberMutex1, \
                               lockType2, memberMutex2)                                          \
    void fnName(paramType paramName) noexcept                                                    \
    {                                                                                            \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};                   \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};                   \
        memberVar = paramName;                                                                   \
    }

/**
 * @brief Macro for a simple setter function using std::move.
 *
 * @param fnName The name of the function.
 * @param paramType The type of the parameter.
 * @param paramName The name of the parameter.
 * @param memberVar The member variable to set.
 */
#define KL_SIMPLE_MOVE_SETTER(fnName, paramType, paramName, memberVar) \
    void fnName(paramType paramName) noexcept { memberVar = std::move_if_noexcept(paramName); }

/**
 * @brief Macro for a simple setter function using std::move with lock.
 *
 * @param fnName The name of the function.
 * @param paramType The type of the parameter.
 * @param paramName The name of the parameter.
 * @param memberVar The member variable to set.
 * @param lockType The type of the lock.
 * @param memberMutex The member mutex to lock.
 */
#define KL_SIMPLE_MOVE_SETTER_LOCK(fnName, paramType, paramName, memberVar, lockType, memberMutex) \
    void fnName(paramType paramName) noexcept                                                      \
    {                                                                                              \
        [[maybe_unused]] const auto koalaMutexLock = lockType{memberMutex};                        \
        memberVar = std::move_if_noexcept(paramName);                                              \
    }

/**
 * @brief Macro for a simple setter function using std::move with two locks.
 *
 * @param fnName The name of the function.
 * @param paramType The type of the parameter.
 * @param paramName The name of the parameter.
 * @param memberVar The member variable to set.
 * @param lockType1 The type of the first lock.
 * @param memberMutex1 The first member mutex to lock.
 * @param lockType2 The type of the second lock.
 * @param memberMutex2 The second member mutex to lock.
 */
#define KL_SIMPLE_MOVE_SETTER_LOCKS(fnName, paramType, paramName, memberVar, lockType1, \
                                    memberMutex1, lockType2, memberMutex2)              \
    void fnName(paramType paramName) noexcept                                           \
    {                                                                                   \
        [[maybe_unused]] const auto koalaMutexLock1 = lockType1{memberMutex1};          \
        [[maybe_unused]] const auto koalaMutexLock2 = lockType2{memberMutex2};          \
        memberVar = std::move_if_noexcept(paramName);                                   \
    }

#endif  // #ifndef KL_GETTER_SETTER_DEFINITIONS_H