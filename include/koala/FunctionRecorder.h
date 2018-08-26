/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/FunctionRecorder.h
 *
 * @brief Header file for the function recorder (FunctionRecorder) class.
 */

#ifndef KL_FUNCTION_RECORDER_H
#define KL_FUNCTION_RECORDER_H 1

#include "koala/Definitions.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

#include <atomic>

namespace kl
{
/**
 * @brief FunctionRecorder class.
 */
class FunctionRecorder
{
public:
    /**
     * @brief Copy constructor.
     *
     * @param other The object from which to copy-construct.
     */
    FunctionRecorder(const FunctionRecorder &other) noexcept;

    /**
     * @brief Move constructor.
     *
     * @param other The object from which to move-construct.
     */
    FunctionRecorder(FunctionRecorder &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The object from which to copy-assign.
     */
    FunctionRecorder &operator=(const FunctionRecorder &other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The object from which to move-assign.
     */
    FunctionRecorder &operator=(FunctionRecorder &&other) noexcept;

    /**
     * @brief Default destructor.
     */
    ~FunctionRecorder() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Record start of function.
     *
     * @param koala The instance of Koala.
     * @param functionName The name of the function.
     * @param algorithmCall Whether this is an algorithm call.
     *
     * @return The ID of the new function call.
     */
    ID_t RecordFunctionStart(const Koala &koala, std::string functionName,
                             const bool algorithmCall = false);

    /**
     * @brief Record start of function.
     *
     * @param koala The instance of Koala.
     * @param functionId The ID of the function whose end to record.
     *
     * @return Success.
     */
    bool RecordFunctionEnd(const Koala &koala, const ID_t functionId) noexcept;

    /**
     * @brief Reset the state of the clock.
     */
    void ResetClockState() noexcept;

    /**
     * @brief Get the current depth.
     *
     * @return The current depth.
     */
    KL_SIMPLE_GETTER_LOCK(, CurrentDepth, m_depthStack.front(), const, ReadLock, m_mutex);

protected:
    /**
     * @brief Constructor.
     */
    FunctionRecorder() noexcept;

    /**
     * @brief Push a value onto the top of the current depth stack.
     *
     * @param value The value to push.
     */
    void PushDepthStack(const std::size_t value = SIZE_T(1UL)) const;

    /**
     * @brief Pop a value off the top of the current depth stack.
     */
    void PopDepthStack() const;

    friend class Koala;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    /**
     * @brief Function call class.
     */
    class FunctionCall
    {
    public:
        /**
         * @brief Get the ID.
         *
         * @return The ID.
         */
        KL_SIMPLE_GETTER(, ID, m_id, const);

        /**
         * @brief Get the function name.
         *
         * @return The function name.
         */
        KL_SIMPLE_GETTER_LVAL_RVAL(const, FunctionName, m_functionName, const);

        /**
         * @brief Get the is-algorithm boolean.
         *
         * @return The is-algorithm boolean.
         */
        KL_SIMPLE_GETTER(, IsAlgorithm, m_isAlgorithm, const);

        /**
         * @brief Get the start time point.
         *
         * @return The start time point.
         */
        KL_SIMPLE_GETTER_LVAL_RVAL(const, StartTimePoint, m_startTimePoint, const);

#ifdef KOALA_ENABLE_CEREAL
        /**
         * @brief Default constructor (for serialization purposes).
         */
        FunctionCall() noexcept;
#endif  // #ifdef KOALA_ENABLE_CEREAL

    protected:
        /**
         * @brief Constructor.
         *
         * @param id The unique ID of this object.
         * @param functionName The name of the called function/algorithm.
         * @param isAlgorithm Whether this is an algorithm rather than a function.
         * @param startTimePoint The start time point.
         */
        FunctionCall(const kl::ID_t id, std::string functionName, const bool isAlgorithm,
                     TimePoint startTimePoint) noexcept;

        friend class FunctionRecorder;

#ifdef KOALA_ENABLE_CEREAL
        friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

    private:
        kl::ID_t m_id;               ///< The unique ID for this function call.
        std::string m_functionName;  ///< The user-defined name of the function call.
        bool m_isAlgorithm;          ///< Whether this is an algorithm being called.
        TimePoint m_startTimePoint;  ///< The time point at which the function started.

#ifdef KOALA_ENABLE_CEREAL
        /**
         * @brief Method template for serializing object.
         *
         * @param archive The cereal archive object.
         */
        template <typename TARCHIVE>
        void serialize(TARCHIVE &archive);
#endif  // #ifdef KOALA_ENABLE_CEREAL
    };

    //----------------------------------------------------------------------------------------------

    using ActiveFunctionIdMap =
        std::unordered_map<ID_t, FunctionCall>;  ///< Alias for a map from the ID to the
                                                 ///< FunctionCall object.

    mutable Mutex m_mutex;  ///< A mutex for locking this object during concurrent access.

    ActiveFunctionIdMap
        m_activeFunctionMap;           ///< A map from the IDs to the active FunctionCall objects.
    TimePoint m_globalStartTimePoint;  ///< The time at which this object was instantiated.
    mutable std::vector<std::size_t> m_depthStack;  ///< The current function call-depth stack.
    mutable std::atomic<ID_t> m_idCounter;          ///< Counter for assigning function IDs.

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for saving an object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void save(TARCHIVE &archive) const;

    /**
     * @brief Method template for loading an object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void load(TARCHIVE &archive);
#endif  // #ifdef KOALA_ENABLE_CEREAL
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline FunctionRecorder::FunctionRecorder(const FunctionRecorder &other) noexcept
    : m_mutex{},
      m_activeFunctionMap{},
      m_globalStartTimePoint{},
      m_depthStack{},
      m_idCounter{SIZE_T(0UL)}
{
    const auto thisLock = kl::WriteLock{m_mutex};
    const auto otherLock = ReadLock{other.m_mutex};

    m_activeFunctionMap = other.m_activeFunctionMap;
    m_globalStartTimePoint = other.m_globalStartTimePoint;
    m_depthStack = other.m_depthStack;
    m_idCounter.store(other.m_idCounter.load());
}

//--------------------------------------------------------------------------------------------------

inline FunctionRecorder::FunctionRecorder(FunctionRecorder &&other) noexcept
    : m_mutex{},
      m_activeFunctionMap{},
      m_globalStartTimePoint{},
      m_depthStack{},
      m_idCounter{SIZE_T(0UL)}
{
    const auto thisLock = kl::WriteLock{m_mutex};
    const auto otherLock = WriteLock{other.m_mutex};

    m_activeFunctionMap = std::move_if_noexcept(other.m_activeFunctionMap);
    m_globalStartTimePoint = std::move_if_noexcept(other.m_globalStartTimePoint);
    m_depthStack = std::move_if_noexcept(other.m_depthStack);
    m_idCounter.store(other.m_idCounter.load());
}

//--------------------------------------------------------------------------------------------------

inline FunctionRecorder &FunctionRecorder::operator=(const FunctionRecorder &other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = ReadLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        auto tmp = FunctionRecorder{other};
        *this = std::move_if_noexcept(tmp);
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

inline FunctionRecorder &FunctionRecorder::operator=(FunctionRecorder &&other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = WriteLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        m_activeFunctionMap = std::move_if_noexcept(other.m_activeFunctionMap);
        m_globalStartTimePoint = std::move_if_noexcept(other.m_globalStartTimePoint);
        m_depthStack = std::move_if_noexcept(other.m_depthStack);
        m_idCounter.store(other.m_idCounter.load());
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

inline ID_t FunctionRecorder::RecordFunctionStart(const Koala &koala, std::string functionName,
                                                  const bool algorithmCall)
{
    const auto lock = WriteLock{m_mutex};
    const auto timepointNow = Clock::now();
    const auto newFunctionCall =
        FunctionCall{m_idCounter, functionName, algorithmCall, timepointNow};

    KL_ASSERT(m_activeFunctionMap.emplace(m_idCounter++, newFunctionCall).second,
              "Could not insert new record into function recorder");

    // Get the start time in milliseconds (since Koala was instantiated).
    const auto startTimeMs =
        std::chrono::duration_cast<Milliseconds>(timepointNow - m_globalStartTimePoint);
    const auto numStartTimeMs = static_cast<int>(startTimeMs.count());

    // Print the start-of-function info.
    using namespace std::string_literals;
    const auto runType = algorithmCall ? "algorithm"s : "function"s;
    const auto outputString = "Start of "s + runType + " '"s + newFunctionCall.FunctionName() +
                              "' at "s + std::to_string(numStartTimeMs) + " ms"s;

    OutputUtility::PrintRecorderStart(koala, m_depthStack.back()++, outputString);
    return newFunctionCall.ID();
}

//--------------------------------------------------------------------------------------------------

inline bool FunctionRecorder::RecordFunctionEnd(const Koala &koala, const ID_t functionId) noexcept
{
    const auto lock = WriteLock{m_mutex};
    const auto iter = m_activeFunctionMap.find(functionId);
    if (iter == m_activeFunctionMap.cend()) return false;

    const auto &startTimePoint = iter->second.StartTimePoint();
    const auto endTimePoint = Clock::now();
    const auto &functionName = iter->second.FunctionName();
    const auto algorithmCall = iter->second.IsAlgorithm();

    // Get the end time in milliseconds (since Koala was instantiated).
    const auto endTimeMs =
        std::chrono::duration_cast<Milliseconds>(endTimePoint - m_globalStartTimePoint);
    const auto numEndTimeMs = static_cast<int>(endTimeMs.count());

    // Work out the elapsed time and print the end-of-function info.
    const auto elapsedMs = std::chrono::duration_cast<Milliseconds>(endTimePoint - startTimePoint);
    const auto numElapsedMs = static_cast<int>(elapsedMs.count());

    using namespace std::string_literals;
    const auto runType = algorithmCall ? "algorithm"s : "function"s;
    const auto outputString = "End of "s + runType + "   '"s + functionName + "' at "s +
                              std::to_string(numEndTimeMs) + " ms. "s;
    const auto elapsedString = "Elapsed time: "s + std::to_string(numElapsedMs) + " ms"s;

    OutputUtility::PrintRecorderEnd(koala, --m_depthStack.back(), outputString, elapsedString);

    m_activeFunctionMap.erase(iter);
    return true;
}

//--------------------------------------------------------------------------------------------------

inline void FunctionRecorder::ResetClockState() noexcept
{
    const auto lock = WriteLock{m_mutex};
    m_activeFunctionMap.clear();
    m_globalStartTimePoint = Clock::now();
    m_depthStack = {SIZE_T(1UL)};
    m_idCounter.store(SIZE_T(0UL));
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline FunctionRecorder::FunctionRecorder() noexcept
    : m_mutex{},
      m_activeFunctionMap{},
      m_globalStartTimePoint{Clock::now()},
      m_depthStack{1UL},
      m_idCounter{0UL}
{
}

//--------------------------------------------------------------------------------------------------

inline void FunctionRecorder::PushDepthStack(const std::size_t value) const
{
    m_depthStack.push_back(value);
}

//--------------------------------------------------------------------------------------------------

inline void FunctionRecorder::PopDepthStack() const
{
    if (m_depthStack.size() > SIZE_T(1UL)) m_depthStack.pop_back();
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void FunctionRecorder::save(TARCHIVE &archive) const
{
    const auto lock = ReadLock{m_mutex};
    archive(m_activeFunctionMap, m_globalStartTimePoint, m_depthStack, m_idCounter.load());
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void FunctionRecorder::load(TARCHIVE &archive)
{
    const auto lock = WriteLock{m_mutex};

    auto depthStack = std::vector<std::size_t>{};
    auto idCounter = SIZE_T(0UL);
    archive(m_activeFunctionMap, m_globalStartTimePoint, depthStack, idCounter);

    m_idCounter.store(idCounter);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline FunctionRecorder::FunctionCall::FunctionCall(const kl::ID_t id, std::string functionName,
                                                    const bool isAlgorithm,
                                                    TimePoint startTimePoint) noexcept
    : m_id{id},
      m_functionName{std::move_if_noexcept(functionName)},
      m_isAlgorithm{isAlgorithm},
      m_startTimePoint{std::move_if_noexcept(startTimePoint)}
{
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
inline FunctionRecorder::FunctionCall::FunctionCall() noexcept
    : m_id{SIZE_T(0UL)}, m_functionName{}, m_isAlgorithm{false}, m_startTimePoint{Clock::now()}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void FunctionRecorder::FunctionCall::serialize(TARCHIVE &archive)
{
    archive(m_id, m_functionName, m_isAlgorithm, m_startTimePoint);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_FUNCTION_RECORDER_H