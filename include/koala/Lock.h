/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Lock.h
 *
 * @brief Header file for the lock (Lock) class.
 */

#ifndef KL_LOCK_H
#define KL_LOCK_H 1

#include <mutex>
#include "koala/KoalaException.h"

/**
 * @brief Macro for getting the current function's name.
 */
#ifdef __GNUC__
#define KL_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define KL_FUNCTION_NAME __func__
#endif  // #ifdef __GNUC__

/**
 * @brief Macro for turning a stream-like input into a string.
 *
 * @param stream The stream-like input.
 */
#define STREAM_TO_STRING(stream)                          \
    [&]() {                                               \
        auto outputStreamToString = std::ostringstream{}; \
        outputStreamToString << stream;                   \
        return outputStreamToString.str();                \
    }()

/**
 * @brief Macro for getting the current function's name.
 */
#define KL_FUNCTION_NAME_STR \
    std::string { KL_FUNCTION_NAME }

/**
 * @brief Macro for throwing a KoalaException.
 *
 * @param message The error message.
 */
#define KL_THROW(message)                                                                        \
    throw ::kl::KoalaException                                                                   \
    {                                                                                            \
        STREAM_TO_STRING(message), KL_FUNCTION_NAME_STR, std::string{__FILE__}, SIZE_T(__LINE__) \
    }

namespace kl
{
/**
 * @brief Lock class template.
 */
template <typename TLOCK = MutexWriteLock,
          typename = std::enable_if_t<std::is_same<std::decay_t<TLOCK>, MutexWriteLock>::value ||
                                      std::is_same<std::decay_t<TLOCK>, MutexReadLock>::value>>
class Lock
{
public:
    /**
     * @brief Constructor.
     *
     * @param mutex The mutex to lock.
     * @param args Other args to pass to the lock.
     */
    template <typename... TARGS>
    Lock(Mutex &mutex, TARGS &&... args);

    /**
     * @brief Default constructor.
     */
    Lock() = default;

    /**
     * @brief Deleted copy constructor.
     */
    Lock(const Lock &) = delete;

    /**
     * @brief Default move constructor.
     */
    Lock(Lock &&) = default;

    /**
     * @brief Deleted copy assignment operator.
     */
    Lock &operator=(const Lock &) = delete;

    /**
     * @brief Default move assignment operator.
     */
    Lock &operator=(Lock &&) = default;

    /**
     * @brief Default destructor.
     */
    ~Lock() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the lock.
     *
     * @return The lock.
     */
    KL_SIMPLE_GETTER_LVAL(, GetLock, m_lock, );

    /**
     * @brief Perform the locking.
     *
     * @param args The locks to lock.
     */
    template <typename... TARGS>
    static void DoLock(TARGS &&... args);

private:
    using TLOCK_D = std::decay_t<TLOCK>;  ///< Alias for a decayed TLOCK type.

    TLOCK_D m_lock;  ///< The lock.
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief ReadLock class template.
 */
class ReadLock : public Lock<MutexReadLock>
{
public:
    using Lock<MutexReadLock>::Lock;  ///< Use the base class constructor.
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief WriteLock class template.
 */
class WriteLock : public Lock<MutexWriteLock>
{
public:
    using Lock<MutexWriteLock>::Lock;  ///< Use the base class constructor.
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TLOCK, typename T>
template <typename... TARGS>
inline Lock<TLOCK, T>::Lock(Mutex &mutex, TARGS &&... args) : m_lock{}
{
    try
    {
        m_lock = TLOCK_D{mutex, std::forward<TARGS>(args)...};
    }

    catch (...)
    {
        KL_THROW("Failed to lock mutex (resource deadlock?)");
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TLOCK, typename T>
template <typename... TARGS>
inline void Lock<TLOCK, T>::DoLock(TARGS &&... args)
{
    try
    {
        std::lock(args.GetLock()...);
    }

    catch (...)
    {
        KL_THROW("Failed to lock mutexes (resource deadlock?)");
    }
}
}  // namespace kl

#endif  // #ifndef KL_LOCK_H