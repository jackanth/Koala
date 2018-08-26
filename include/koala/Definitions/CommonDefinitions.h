/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Definitions/CommonDefinitions.h
 *
 * @brief Header file for koala common definitions.
 */

#ifndef KL_COMMON_DEFINITIONS_H
#define KL_COMMON_DEFINITIONS_H 1

#include <chrono>
#include <experimental/filesystem>
#include <list>
#include <map>
#include <set>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief Macro for casting a value as std::size_t.
 *
 * @param value The value to cast.
 */
#define SIZE_T(value) static_cast<std::size_t>(value)

/**
 * @brief Macro for running a function and starting and stopping the function recorder as required
 * if debug mode is turned on. Assumes function is void.
 *
 * @param fnName The name of the function to call.
 * @param ... The arguments to pass to the function.
 */
#if defined(KOALA_DEBUG)
#define KL_RECORD_FUNC(fnName, ...)                                                        \
    [&]() {                                                                                \
        if (!(this->GetKoala().DebugMode()))                                               \
            fnName(__VA_ARGS__);                                                           \
                                                                                           \
        else                                                                               \
        {                                                                                  \
            const auto &functionIdInMacro =                                                \
                this->GetKoala().KoalaFunctionRecorder().RecordFunctionStart(              \
                    this->GetKoala(), std::string{#fnName});                               \
            fnName(__VA_ARGS__);                                                           \
            this->GetKoala().KoalaFunctionRecorder().RecordFunctionEnd(this->GetKoala(),   \
                                                                       functionIdInMacro); \
        }                                                                                  \
    }()
#else
#define KL_RECORD_FUNC(fnName, ...) fnName(__VA_ARGS__)
#endif  // #if defined(KOALA_DEBUG)

/**
 * @brief Macro for running a function and starting and stopping the function recorder as required
 * if debug mode is turned on, returning the value of the function. Function must not be void.
 *
 * @param fnName The name of the function to call.
 * @param ... The arguments to pass to the function.
 */
#if defined(KOALA_DEBUG)
#define KL_RECORD_FUNC_RESULT(fnName, ...)                                                 \
    [&]() -> decltype(auto) {                                                              \
        if (!(this->GetKoala().DebugMode()))                                               \
            return fnName(__VA_ARGS__);                                                    \
                                                                                           \
        else                                                                               \
        {                                                                                  \
            const auto &functionIdInMacro =                                                \
                this->GetKoala().KoalaFunctionRecorder().RecordFunctionStart(              \
                    this->GetKoala(), std::string{#fnName});                               \
            decltype(fnName(__VA_ARGS__)) result = fnName(__VA_ARGS__);                    \
            this->GetKoala().KoalaFunctionRecorder().RecordFunctionEnd(this->GetKoala(),   \
                                                                       functionIdInMacro); \
            return result;                                                                 \
        }                                                                                  \
    }()
#else
#define KL_RECORD_FUNC_RESULT(fnName, ...) fnName(__VA_ARGS__)
#endif  // #if defined(KOALA_DEBUG)

/**
 * @brief Dummy class template for displaying object types in compiler errors. Usage for variable
 * `x`: `TypeDisplayer<decltype(x)>{};`.
 */
template <typename T>
class TypeDisplayer;

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

namespace kl
{
// STL-derived type definitions.
using ID_t = std::size_t;                               ///< Alias for an ID type.
using IdVector = std::vector<ID_t>;                     ///< Alias for a vector of IDs.
using IdUnorderedSet = std::unordered_set<ID_t>;        ///< Alias for an unordered set of IDs.
using IdPair = std::pair<ID_t, ID_t>;                   ///< Alias for a pair of IDs.
using IdPairVector = std::vector<IdPair>;               ///< Alias for a vector of ID pairs.
using IdPairUnorderedSet = std::unordered_set<IdPair>;  ///< Alias for an unordered set of ID pairs.
using StringVector = std::vector<std::string>;          ///< Alias for a vector of strings.
using StringUnorderedSet =
    std::unordered_set<std::string>;  ///< Alias for an unordered set of strings.
using StringUnorderedMap =
    std::unordered_map<std::string, std::string>;  ///< Alias for an unordered map between strings.
using StringList = std::list<std::string>;         ///< Alias for a list of strings.
using Mutex = std::shared_timed_mutex;             ///< Alias for a mutex.
using MutexReadLock = std::shared_lock<Mutex>;     ///< Alias for a mutex read lock.
using MutexWriteLock = std::unique_lock<Mutex>;    ///< Alias for a mutex write lock.
using Path = std::experimental::filesystem::path;  ///< Alias for a path.
using PathVector = std::vector<Path>;              ///< Alias for a vector of paths.
using PathSet = std::set<Path>;                    ///< Alias for a set of paths.
using PathMap = std::map<Path, Path>;              ///< Alias for an unordered map between paths.
using Clock = std::chrono::system_clock;           ///< Alias for a system clock.
using TimePoint = Clock::time_point;               ///< Alias for a point in time.
using Milliseconds = std::chrono::milliseconds;    ///< Alias for a milliseconds-duration.
using Seconds = std::chrono::seconds;              ///< Alias for a seconds-duration.
using Minutes = std::chrono::minutes;              ///< Alias for a minutes-duration.
using Hours = std::chrono::hours;                  ///< Alias for an hours-duration.
using Days = std::chrono::duration<int, std::ratio<60 * 60 * 24>>;  ///< Alias for a days duration.
using Weeks =
    std::chrono::duration<int, std::ratio<60 * 60 * 24 * 7>>;  ///< Alias for a weeks-duration.
using Years =
    std::chrono::duration<int, std::ratio<60 * 60 * 24 * 365>>;  ///< Alias for a a years-duration.

//----------------------------------------------------------------------------------------------

/**
 * @brief Functor for a hash function for unordered sets of unordered sets of IDs.
 */
struct SetHashFunctor
{
    /**
     * @brief Combine two hashes to produce a new hash.
     *
     * @param newHash The first and new hash.
     * @param hashKey The second hash
     */
    inline void combineHash(ID_t &newHash, const ID_t hashKey) const
    {
        newHash ^= hashKey + 0x9e3779b9 + (newHash << 6) + (newHash >> 2);
    }

    /**
     * @brief Return the hash key for a given set of IDs.
     *
     * @param idSet The set of IDs.
     *
     * @return The hash key.
     */
    auto operator()(const IdUnorderedSet &idSet) const noexcept
    {
        auto hashKey = SIZE_T(0UL);

        for (const auto id : idSet) combineHash(hashKey, std::hash<ID_t>()(id));

        return hashKey;
    }
};

//----------------------------------------------------------------------------------------------

using SetOfIdUnSets =
    std::unordered_set<IdUnorderedSet, SetHashFunctor>;  ///< Alias for an unordered set of IDs.

//----------------------------------------------------------------------------------------------

/**
 * @brief Functor for a hash function for unordered sets of unordered sets of IDs.
 */
struct SetOfSetsHashFunctor
{
    /**
     * @brief Combine two hashes to produce a new hash.
     *
     * @param newHash The first and new hash.
     * @param hashKey The second hash
     */
    inline void combineHash(ID_t &newHash, const ID_t hashKey) const
    {
        newHash ^= hashKey + 0x9e3779b9 + (newHash << 6) + (newHash >> 2);
    }

    /**
     * @brief Return the hash key for a given set of ID sets.
     *
     * @param idSetOfSets The set of ID sets.
     *
     * @return The hash key.
     */
    auto operator()(const SetOfIdUnSets &idSetOfSets) const noexcept
    {
        auto hashKey = SIZE_T(0UL);

        for (const auto &idSet : idSetOfSets) combineHash(hashKey, SetHashFunctor()(idSet));

        return hashKey;
    }
};

//----------------------------------------------------------------------------------------------

/**
 * @brief Convert a set of reference wrappers to a set of const reference wrappers.
 *
 * @param refSet The set of reference wrappers.
 *
 * @return The set of const reference wrappers.
 */
template <typename TOBJECT>
typename TOBJECT::UnorderedConstRefSet ToConstRef(const typename TOBJECT::UnorderedRefSet &refSet)
{
    auto constRefSet = typename TOBJECT::UnorderedConstRefSet{};

    for (const auto &ref : refSet) constRefSet.emplace(ref.get());

    return constRefSet;
}

/**
 * @brief Convert a vector of reference wrappers to a vector of const reference wrappers.
 *
 * @param refVector The vector of reference wrappers.
 *
 * @return The vector of const reference wrappers.
 */
template <typename TOBJECT>
typename TOBJECT::ConstRefVector ToConstRef(const typename TOBJECT::RefVector &refVector)
{
    auto constRefVector = typename TOBJECT::ConstRefVector{};

    for (const auto &ref : refVector) constRefVector.emplace_back(ref.get());

    return constRefVector;
}

/**
 * @brief Convert a reference wrapper to a const reference wrapper.
 *
 * @param ref The reference wrapper.
 *
 * @return The const reference wrapper.
 */
template <typename TOBJECT>
inline typename TOBJECT::ConstRef ToConstRef(const typename TOBJECT::RefWrapper &ref)
{
    return typename TOBJECT::ConstRef{ref.get()};
}

//----------------------------------------------------------------------------------------------

using SetOfSetOfIdUnSets =
    std::unordered_set<SetOfIdUnSets,
                       SetOfSetsHashFunctor>;  ///< Alias for an unordered set of IDs.
}  // namespace kl

//--------------------------------------------------------------------------------------------------

/**
 * @brief Operator for printing a StringVector to ostream.
 *
 * @param os The ostream object.
 * @param stringVector The StringVector to print.
 *
 * @return The new ostream object.
 */
inline std::ostream &operator<<(std::ostream &os, const ::kl::StringVector &stringVector)
{
    for (auto iter = stringVector.cbegin(); iter != stringVector.cend(); ++iter)
    {
        os << *iter;

        if (std::next(iter, 1UL) != stringVector.cend()) os << std::endl;
    }

    return os;
}

//--------------------------------------------------------------------------------------------------

/**
 * @brief Operator for printing a StringSet to ostream.
 *
 * @param os The ostream object.
 * @param stringSet The StringSet to print.
 *
 * @return The new ostream object.
 */
inline std::ostream &operator<<(std::ostream &os, const ::kl::StringUnorderedSet &stringSet)
{
    for (auto iter = stringSet.cbegin(); iter != stringSet.cend(); ++iter)
    {
        os << *iter;

        if (std::next(iter, 1UL) != stringSet.cend()) os << std::endl;
    }

    return os;
}

//--------------------------------------------------------------------------------------------------

/**
 * @brief Operator for adding two paths together.
 *
 * @param lhs The lhs path.
 * @param rhs The rhs path.
 *
 * @return The new path.
 */
inline kl::Path operator+(const kl::Path &lhs, const kl::Path &rhs)
{
    auto combinedPath = lhs;
    combinedPath += rhs;
    return combinedPath;
}
#endif  // #ifndef KL_COMMON_DEFINITIONS_H