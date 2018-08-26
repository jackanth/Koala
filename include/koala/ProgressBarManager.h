/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/ProgressBarManager.h
 *
 * @brief Header file for the progress bar manager (ProgressBarManager) class.
 */

#ifndef KL_PROGRESS_BAR_MANAGER_H
#define KL_PROGRESS_BAR_MANAGER_H 1

#include "koala/Definitions.h"
#include "koala/ProgressBar.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

#include <future>
#include <thread>

namespace kl
{
/**
 * @brief ProgressBarManager class template.
 */
template <typename TKOALA>
class ProgressBarManager
{
public:
    using sPtr =
        std::shared_ptr<ProgressBarManager>;  ///< Alias for a shared pointer to this object.

    /**
     * @brief Copy constructor.
     *
     * @param other The object from which to copy-construct.
     */
    ProgressBarManager(const ProgressBarManager &other) noexcept;

    /**
     * @brief Move constructor.
     *
     * @param other The object from which to move-construct.
     */
    ProgressBarManager(ProgressBarManager &&other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The object from which to copy-assign.
     */
    ProgressBarManager &operator=(const ProgressBarManager &other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The object from which to move-assign.
     */
    ProgressBarManager &operator=(ProgressBarManager &&other) noexcept;

    /**
     * @brief Default destructor.
     */
    ~ProgressBarManager() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get whether to display progress bars.
     *
     * @return Whether to display progress bars.
     */
    KL_SIMPLE_GETTER_LOCK(, DisplayProgressBars, m_displayProgressBars, const, ReadLock, m_mutex);

    /**
     * @brief Set whether to display progress bars.
     *
     * @param display Whether to display progress bars.
     */
    void DisplayProgressBars(const bool display);

    /**
     * @brief Get the progress bar latency.
     *
     * @return The progress bar latency in milliseconds.
     */
    KL_SIMPLE_GETTER_LOCK(, ProgressBarLatency, m_progressBarLatency, const, ReadLock, m_mutex);

    /**
     * @brief Set the progress bar latency.
     *
     * @param progressBarLatency The progress bar latency in milliseconds.
     */
    KL_SIMPLE_SETTER_LOCK(ProgressBarLatency, const std::size_t, progressBarLatency,
                          m_progressBarLatency, WriteLock, m_mutex);

    /**
     * @brief Get whether to print progress bars on a loop.
     *
     * @return Whether to print progress bars on a loop.
     */
    KL_SIMPLE_GETTER_LOCK(, LoopPrintProgressBars, m_loopPrintProgressBars, const, ReadLock,
                          m_mutex);

    /**
     * @brief Set the progress bar loop-print boolean.
     *
     * @param loopPrintProgressBars The progress bar loop-print boolean
     */
    void LoopPrintProgressBars(const bool loopPrintProgressBars);

    /**
     * @brief Create a progress bar.
     *
     * @param name The name of the bar.
     * @param args The arguments to pas to the progress bar constructor, excluding the redraw
     * callback.
     *
     * @return The progress bar.
     */
    template <typename... TARGS>
    auto &CreateProgressBar(std::string name, TARGS &&... args);

    /**
     * @brief Update a progress bar.
     *
     * @param name The name of the bar.
     * @param progress The new progress.
     */
    void UpdateProgressBar(const std::string &name, const float progress = 0.f);

    /**
     * @brief Delete a progress bar.
     *
     * @param name The name of the bar.
     */
    void DeleteProgressBar(const std::string &name);

protected:
    /**
     * @brief Constructor.
     *
     * @param wpKoala Weak pointer to the instance of Koala.
     */
    ProgressBarManager(const std::weak_ptr<std::decay_t<TKOALA>> &wpKoala) noexcept;

    friend class Koala;

#ifdef KOALA_ENABLE_CEREAL
    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

private:
    using TKOALA_D = std::decay_t<TKOALA>;  ///< Alias for a decayed TKOALA type.
    using Koala_wPtr =
        std::weak_ptr<TKOALA_D>;  ///< Alias for a weak pointer to a decayed TKOALA type.

    mutable Mutex m_mutex;  ///< A mutex for locking this object under concurrent access.

    Koala_wPtr m_wpKoala;                           ///< Weak pointer to the instance of Koala.
    ProgressBar::NameMap m_progressBarMap;          ///< A map from names to progress bars.
    StringVector m_progressBarNames;                ///< The ordered list of progress bar names.
    std::atomic_bool m_displayProgressBars;         ///< Whether to display progress bars.
    mutable std::size_t m_lastNumberOfBars;         ///< The last number of progress bars printed.
    mutable std::future<void> m_progressBarFuture;  ///< The progress bar future.
    std::size_t m_progressBarLatency;          ///< The latency when reprinting the progress bars.
    std::atomic_bool m_loopPrintProgressBars;  ///< Whether to print progress bars on a loop.

    /**
     * @brief Print the progress bars.
     *
     * @param pStreamBuf Pointer to the streambuf object.
     */
    void PrintProgressBars(std::streambuf *const pStreamBuf);

    /**
     * @brief Delete any dead progress bars.
     */
    void DeleteDeadBars();

    /**
     * @brief Get the instance of Koala.
     *
     * @return The instance of Koala.
     */
    auto &GetKoala() const;

    /**
     * @brief Print progress bars in a timed loop.
     */
    void PrintProgressBarsOnLoop();

    /**
     * @brief Delete a progress bar (implementation method).
     *
     * @param name The name of the progress bar to delete.
     *
     * @return An iterator to continue looping over the map.
     */
    auto DeleteProgressBarImpl(const std::string &name);

    /**
     * @brief Stream newline callback method.
     *
     * @param pStreamBuf Pointer to the streambuf object.
     */
    void StreamNewlineCallback(std::streambuf *const pStreamBuf);

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Method template for serializing an object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);
#endif  // #ifdef KOALA_ENABLE_CEREAL
};
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/ProgressBarManager.txx"

#endif  // #ifndef KL_PROGRESS_BAR_MANAGER_H