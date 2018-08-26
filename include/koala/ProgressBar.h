/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/ProgressBar.h
 *
 * @brief Header file for the progress bar (ProgressBar) class.
 */

#ifndef KL_PROGRESS_BAR_H
#define KL_PROGRESS_BAR_H 1

#include "koala/Definitions/GetterSetterDefinitions.h"
#include "koala/Lock.h"

#ifdef KOALA_ENABLE_CEREAL
#include "cereal/access.hpp"
#endif  // #ifdef KOALA_ENABLE_CEREAL

#include <functional>

namespace kl
{
/**
 * @brief Forward declaration of the ProgressBarManager class template.
 */
template <typename T>
class ProgressBarManager;

/**
 * @brief ProgressBar class.
 */
class ProgressBar
{
public:
    /**
     * @brief Enum enumerating the different available bar colours.
     */
    enum class COLOUR
    {
        RED,      ///< The colour red.
        GREEN,    ///< The colour green.
        YELLOW,   ///< The colour yellow.
        MAGENTA,  ///< The colour magenta.
        BLUE,     ///< The colour blue.
        CYAN      ///< The colour cyan.
    };

    using sPtr = std::shared_ptr<ProgressBar>;  ///< Alias for a shared pointer to this object.
    using NameMap =
        std::unordered_map<std::string,
                           ProgressBar::sPtr>;  ///< Alias for a map from names to progress bars.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Deleted copy constructor.
     */
    ProgressBar(const ProgressBar &) = delete;

    /**
     * @brief Default move constructor.
     */
    ProgressBar(ProgressBar &&) = default;

    /**
     * @brief Deleted copy assignment operator.
     */
    ProgressBar &operator=(const ProgressBar &) = delete;

    /**
     * @brief Default move assignment operator.
     */
    ProgressBar &operator=(ProgressBar &&) = default;

    /**
     * @brief Default destructor.
     */
    ~ProgressBar() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the current progress.
     *
     * @return The current progress.
     */
    KL_SIMPLE_GETTER_LOCK(, Progress, m_progress, const, ReadLock, m_mutex);

    /**
     * @brief Set the current progress.
     *
     * @param progress The current progress.
     */
    void Progress(const float progress) noexcept;

    /**
     * @brief Get the colour.
     *
     * @return The colour.
     */
    KL_SIMPLE_GETTER_LOCK(, Colour, m_colour, const, ReadLock, m_mutex);

    /**
     * @brief Set the colour.
     *
     * @param colour The colour.
     */
    void Colour(const COLOUR colour) noexcept;

    /**
     * @brief Get the label.
     *
     * @return The label.
     */
    KL_SIMPLE_GETTER_LOCK(, Label, m_label, const, ReadLock, m_mutex);

    /**
     * @brief Set the label.
     *
     * @param label The label.
     */
    void Label(std::string label) noexcept;

    /**
     * @brief Get whether to display an ETA.
     *
     * @return Whether to display an ETA.
     */
    KL_SIMPLE_GETTER_LOCK(, DisplayEta, m_displayEta, const, ReadLock, m_mutex);

    /**
     * @brief Whether to display an ETA.
     *
     * @param displayEta Whether to display an ETA.
     */
    void DisplayEta(const bool displayEta) noexcept;

    /**
     * @brief Get whether to display the time elapsed.
     *
     * @return Whether to display the time elapsed.
     */
    KL_SIMPLE_GETTER_LOCK(, DisplayTimeElapsed, m_displayTimeElapsed, const, ReadLock, m_mutex);

    /**
     * @brief Whether to display the time elapsed.
     *
     * @param displayTimeElapsed Whether to display the time elapsed.
     */
    void DisplayTimeElapsed(const bool displayTimeElapsed) noexcept;

    /**
     * @brief Get the number of seconds elapsed.
     *
     * @return Number of seconds elapsed.
     */
    std::size_t SecondsElapsed() const;

    /**
     * @brief Get the auto-delete option.
     *
     * @return The auto-delete option.
     */
    KL_SIMPLE_GETTER_LOCK(, AutoDelete, m_autoDelete, const, ReadLock, m_mutex);

    /**
     * @brief Set the auto-delete option.
     *
     * @param autoDelete The auto-delete option.
     */
    void AutoDelete(const bool autoDelete) noexcept;

    /**
     * @brief Get the auto-delete delay.
     *
     * @return The auto-delete delay.
     */
    KL_SIMPLE_GETTER_LOCK(, AutoDeleteDelay, m_autoDeleteDelay, const, ReadLock, m_mutex);

    /**
     * @brief Set the auto-delete delay.
     *
     * @param autoDeleteDelay The auto-delete delay in milliseconds.
     */
    void AutoDeleteDelay(const bool autoDeleteDelay) noexcept;

    /**
     * @brief Get whether the progress bar has completed.
     *
     * @return Whether the progress bar has completed.
     */
    KL_SIMPLE_GETTER_LOCK(, Completed, m_completed, const, ReadLock, m_mutex);

private:
    using RedrawCallback = std::function<void()>;  ///< Alias for a callback to redraw the bars.

    mutable Mutex m_mutex;       ///< The mutex.
    TimePoint m_startTimePoint;  ///< The time at which this object was instantiated.
    float m_progress;            ///< The current progress.
    std::string m_label;         ///< The bar label.
    COLOUR m_colour;             ///< The bar colour.
    bool m_displayEta;           ///< Whether to display an ETA.
    bool m_displayTimeElapsed;   ///< Whether to display the time elapsed.
    bool m_autoDelete;  ///< Whether to automatically delete the progress bar when it is complete.
    std::size_t m_autoDeleteDelay;    ///< The delay in milliseconds after which to delete a full
                                      ///< progress bar.
    TimePoint m_endTimePoint;         ///< The time at which the progress bar completed.
    bool m_completed;                 ///< Whether the progress bar has completed.
    RedrawCallback m_redrawCallback;  ///< A callback to redraw the bars.

#ifdef KOALA_ENABLE_CEREAL
    /**
     * @brief Default constructor for serialization purposes.
     */
    ProgressBar() noexcept;

    /**
     * @brief Method template for serializing object.
     *
     * @param archive The cereal archive object.
     */
    template <typename TARCHIVE>
    void serialize(TARCHIVE &archive);

    friend class cereal::access;
#endif  // #ifdef KOALA_ENABLE_CEREAL

protected:
    /**
     * @brief Constructor.
     *
     * @param redrawCallback The redraw callback.
     * @param label The bar label.
     * @param colour The bar colour.
     * @param displayEta Whether to display an ETA.
     * @param displayTimeElapsed Whether to display the time elapsed.
     * @param progress The initial progress.
     */
    ProgressBar(RedrawCallback redrawCallback, std::string label = std::string{},
                const COLOUR colour = COLOUR::GREEN, const bool displayEta = false,
                const bool displayTimeElapsed = false, const float progress = 0.f) noexcept;

    /**
     * @brief Get the amount of time elapsed since completion.
     *
     * @return The time elapsed.
     */
    std::size_t TimeElapsedSinceCompletion() const;

    template <typename T>
    friend class ProgressBarManager;
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline void ProgressBar::Colour(const COLOUR colour) noexcept
{
    {
        const auto lock = WriteLock{m_mutex};
        m_colour = colour;
    }

    m_redrawCallback();
}

//--------------------------------------------------------------------------------------------------

inline void ProgressBar::Label(std::string label) noexcept
{
    {
        const auto lock = WriteLock{m_mutex};
        m_label = std::move_if_noexcept(label);
    }

    m_redrawCallback();
}

//--------------------------------------------------------------------------------------------------

inline void ProgressBar::DisplayEta(const bool displayEta) noexcept
{
    const auto lock = WriteLock{m_mutex};
    {
        m_displayEta = displayEta;
    }

    m_redrawCallback();
}

//--------------------------------------------------------------------------------------------------

inline void ProgressBar::DisplayTimeElapsed(const bool displayTimeElapsed) noexcept
{
    {
        const auto lock = WriteLock{m_mutex};
        m_displayTimeElapsed = displayTimeElapsed;
    }

    m_redrawCallback();
}

//--------------------------------------------------------------------------------------------------

inline void ProgressBar::AutoDelete(const bool autoDelete) noexcept
{
    {
        const auto lock = WriteLock{m_mutex};
        m_autoDelete = autoDelete;
    }

    m_redrawCallback();
}

//--------------------------------------------------------------------------------------------------

inline void ProgressBar::AutoDeleteDelay(const bool autoDeleteDelay) noexcept
{
    {
        const auto lock = WriteLock{m_mutex};
        m_autoDeleteDelay = autoDeleteDelay;
    }

    m_redrawCallback();
}

//--------------------------------------------------------------------------------------------------

inline ProgressBar::ProgressBar(RedrawCallback redrawCallback, std::string label,
                                const COLOUR colour, const bool displayEta,
                                const bool displayTimeElapsed, const float progress) noexcept
    : m_mutex{},
      m_startTimePoint{Clock::now()},
      m_progress{progress},
      m_label{std::move_if_noexcept(label)},
      m_colour{colour},
      m_displayEta{displayEta},
      m_displayTimeElapsed{displayTimeElapsed},
      m_autoDelete{false},
      m_autoDeleteDelay{static_cast<std::size_t>(1000UL)},
      m_endTimePoint{},
      m_completed{false},
      m_redrawCallback{std::move_if_noexcept(redrawCallback)}
{
}

//--------------------------------------------------------------------------------------------------

inline void ProgressBar::Progress(float progress) noexcept
{
    {
        const auto lock = WriteLock{m_mutex};

        if (progress < 0.f)
            progress = 0.f;

        else if (progress > 1.f)
            progress = 1.f;

        if (progress == 1.f)
        {
            m_endTimePoint = Clock::now();
            m_completed = true;
        }
    }

    m_progress = progress;
    m_redrawCallback();
}

//--------------------------------------------------------------------------------------------------

inline std::size_t ProgressBar::SecondsElapsed() const
{
    const auto elapsedSeconds =
        std::chrono::duration_cast<Seconds>(Clock::now() - m_startTimePoint);
    return static_cast<std::size_t>(elapsedSeconds.count());
}

//--------------------------------------------------------------------------------------------------

inline std::size_t ProgressBar::TimeElapsedSinceCompletion() const
{
    if (m_completed)
    {
        const auto elapsedMilliSeconds =
            std::chrono::duration_cast<Milliseconds>(Clock::now() - m_endTimePoint);
        return static_cast<std::size_t>(elapsedMilliSeconds.count());
    }

    return static_cast<std::size_t>(0UL);
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
inline ProgressBar::ProgressBar() noexcept
    : m_mutex{},
      m_startTimePoint{Clock::now()},
      m_progress{0.f},
      m_label{},
      m_colour{COLOUR::GREEN},
      m_displayEta{true},
      m_displayTimeElapsed{true},
      m_autoDelete{true},
      m_autoDeleteDelay{static_cast<std::size_t>(1000UL)},
      m_endTimePoint{},
      m_completed{false}
{
}
#endif  // #ifdef KOALA_ENABLE_CEREAL

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TARCHIVE>
inline void ProgressBar::serialize(TARCHIVE &archive)
{
    archive(m_startTimePoint, m_progress, m_label, m_colour, m_displayEta, m_displayTimeElapsed,
            m_autoDelete, m_autoDeleteDelay, m_endTimePoint, m_completed);
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_PROGRESS_BAR_H