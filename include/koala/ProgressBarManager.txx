/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/ProgressBarManager.h
 *
 * @brief Template implementation header file for the progress bar manager (ProgressBarManager)
 * class.
 */

#ifndef KL_PROGRESS_BAR_MANAGER_IMPL_H
#define KL_PROGRESS_BAR_MANAGER_IMPL_H 1

namespace kl
{
template <typename TKOALA>
ProgressBarManager<TKOALA>::ProgressBarManager(const ProgressBarManager &other) noexcept
    : m_mutex{},
      m_wpKoala{},
      m_progressBarMap{},
      m_progressBarNames{},
      m_displayProgressBars{true},
      m_lastNumberOfBars{SIZE_T(0UL)},
      m_progressBarFuture{},
      m_progressBarLatency{SIZE_T(100UL)},
      m_loopPrintProgressBars{false}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = ReadLock{other.m_mutex};

    m_wpKoala = other.m_wpKoala;
    m_progressBarMap = other.m_progressBarMap;
    m_progressBarNames = other.m_progressBarNames;
    m_displayProgressBars = other.m_displayProgressBars;
    m_progressBarLatency = other.m_progressBarLatency;

    // Note that the future is not copied, the loop-print boolean is kept as false and the last
    // number of bars is left as 0.
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
ProgressBarManager<TKOALA>::ProgressBarManager(ProgressBarManager &&other) noexcept
    : m_mutex{},
      m_wpKoala{},
      m_progressBarMap{},
      m_progressBarNames{},
      m_displayProgressBars{true},
      m_lastNumberOfBars{SIZE_T(0UL)},
      m_progressBarFuture{},
      m_progressBarLatency{SIZE_T(100UL)},
      m_loopPrintProgressBars{false}
{
    const auto thisLock = WriteLock{other.m_mutex};
    const auto otherLock = WriteLock{other.m_mutex};

    m_wpKoala = std::move_if_noexcept(other.m_wpKoala);
    m_progressBarMap = std::move_if_noexcept(other.m_progressBarMap);
    m_progressBarNames = std::move_if_noexcept(other.m_progressBarNames);
    m_displayProgressBars = other.m_displayProgressBars;
    m_progressBarLatency = other.m_progressBarLatency;

    // Note that the future is not moved, the loop-print boolean is kept as false, and the last
    // number of bars is left as 0.
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
ProgressBarManager<TKOALA> &ProgressBarManager<TKOALA>::operator=(
    const ProgressBarManager &other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = WriteLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        auto tmp = ProgressBarManager{other};
        *this = std::move_if_noexcept(tmp);

        // Note that the future is not copied, the loop-print boolean is set as false, and the last
        // number of bars is left as 0.
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
ProgressBarManager<TKOALA> &ProgressBarManager<TKOALA>::operator=(
    ProgressBarManager &&other) noexcept
{
    if (this != &other)
    {
        auto thisLock = WriteLock{m_mutex, std::defer_lock};
        auto otherLock = WriteLock{other.m_mutex, std::defer_lock};
        Lock<>::DoLock(thisLock, otherLock);

        m_wpKoala = std::move_if_noexcept(other.m_wpKoala);
        m_progressBarMap = std::move_if_noexcept(other.m_progressBarMap);
        m_progressBarNames = std::move_if_noexcept(other.m_progressBarNames);
        m_displayProgressBars = other.m_displayProgressBars;
        m_lastNumberOfBars = SIZE_T(0UL);
        m_progressBarFuture = std::future<void>{};
        m_progressBarLatency = other.m_progressBarLatency;
        m_loopPrintProgressBars = false;

        // Note that the future is not moved, the loop-print boolean is set as false, and the last
        // number of bars is set as 0.
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
auto ProgressBarManager<TKOALA>::DeleteProgressBarImpl(const std::string &name)
{
    const auto findIter = m_progressBarMap.find(name);

    if (findIter == m_progressBarMap.end())
        KL_THROW("No such progress bar: " << KL_WHITE_BOLD << name);

    m_progressBarNames.erase(
        std::remove(m_progressBarNames.begin(), m_progressBarNames.end(), name),
        m_progressBarNames.end());
    return m_progressBarMap.erase(findIter);
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
void ProgressBarManager<TKOALA>::StreamNewlineCallback(std::streambuf *const pStreamBuf)
{
    if (m_displayProgressBars && pStreamBuf)
    {
        pStreamBuf->sputn(KL_CLEAR, sizeof(KL_CLEAR) - 1UL);
        const auto lock = ReadLock{m_mutex};
        this->PrintProgressBars(pStreamBuf);
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
ProgressBarManager<TKOALA>::ProgressBarManager(
    const std::weak_ptr<std::decay_t<TKOALA>> &wpKoala) noexcept
    : m_mutex{},
      m_wpKoala{wpKoala},
      m_progressBarMap{},
      m_progressBarNames{},
      m_displayProgressBars{true},
      m_lastNumberOfBars{SIZE_T(0UL)},
      m_progressBarFuture{},
      m_progressBarLatency{SIZE_T(100UL)},
      m_loopPrintProgressBars{false}
{
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
void ProgressBarManager<TKOALA>::DisplayProgressBars(const bool display)
{
    if (!m_displayProgressBars && display && m_loopPrintProgressBars)
    {
        m_displayProgressBars = display;
        m_progressBarFuture =
            std::async(std::launch::async, &ProgressBarManager::PrintProgressBarsOnLoop, this);
    }

    {
        {
            const auto lock = ReadLock{m_mutex};

            if (m_displayProgressBars && !display)  // if we're turning them off, erase them
            {
                m_displayProgressBars = display;
                this->GetKoala().GetStdout()
                    << OutputUtility::EraseProgressBars(m_lastNumberOfBars) << std::flush;
            }
        }

        m_displayProgressBars = display;

        // Clean up and then (re)print the bars.
        if (m_displayProgressBars)  // if we're turning them on (or leaving them on), clean up and
                                    // reprint
        {
            {
                const auto lock = WriteLock{m_mutex};
                this->DeleteDeadBars();
            }

            const auto lock = ReadLock{m_mutex};
            this->PrintProgressBars(
                this->GetKoala().GetStdout().GetBuffer().GetTerminalStreamBuf());
            this->GetKoala().GetStdout().flush();
        }
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void ProgressBarManager<TKOALA>::LoopPrintProgressBars(const bool loopPrintProgressBars)
{
    if (!m_loopPrintProgressBars && loopPrintProgressBars)  // switch on
    {
        m_loopPrintProgressBars = loopPrintProgressBars;
        m_progressBarFuture =
            std::async(std::launch::async, &ProgressBarManager::PrintProgressBarsOnLoop, this);
    }

    if (m_loopPrintProgressBars && !loopPrintProgressBars)  // switch off
    {
        m_loopPrintProgressBars = loopPrintProgressBars;
        m_progressBarFuture = {};
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
template <typename... TARGS>
auto &ProgressBarManager<TKOALA>::CreateProgressBar(std::string name, TARGS &&... args)
{
    auto spProgressBar = ProgressBar::sPtr{nullptr};

    {
        const auto lock = WriteLock{m_mutex};
        const auto findIter = m_progressBarMap.find(name);

        if (findIter != m_progressBarMap.end())
            KL_THROW("Progress bar already exists: " << KL_WHITE_BOLD << name);

        m_progressBarNames.push_back(name);

        // Return the new ProgressBar.
        spProgressBar = ProgressBar::sPtr{new ProgressBar{
            [&]() {
                const auto lock = ReadLock{m_mutex};
                this->PrintProgressBars(
                    this->GetKoala().GetStdout().GetBuffer().GetTerminalStreamBuf());
                this->GetKoala().GetStdout().flush();
            },
            std::forward<TARGS>(args)...}};

        KL_ASSERT(m_progressBarMap.emplace(std::move(name), spProgressBar).second,
                  "Failed to add new progress bar to map");
    }

    if (m_displayProgressBars)
    {
        {
            const auto lock = WriteLock{m_mutex};
            this->DeleteDeadBars();
        }

        const auto lock = ReadLock{m_mutex};
        this->PrintProgressBars(this->GetKoala().GetStdout().GetBuffer().GetTerminalStreamBuf());
        this->GetKoala().GetStdout().flush();
    }

    KL_ASSERT(spProgressBar, "Failed to create new progress bar");
    return *spProgressBar;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
void ProgressBarManager<TKOALA>::UpdateProgressBar(const std::string &name, const float progress)
{
    {
        const auto lock = WriteLock{m_mutex};
        const auto findIter = m_progressBarMap.find(name);

        if (findIter == m_progressBarMap.end())
            KL_THROW("No such progress bar: " << KL_WHITE_BOLD << name);

        findIter->second->Progress(progress);
    }

    if (m_displayProgressBars)
    {
        // Clean up and reprint.
        {
            const auto lock = WriteLock{m_mutex};
            this->DeleteDeadBars();
        }

        const auto lock = ReadLock{m_mutex};
        this->PrintProgressBars(this->GetKoala().GetStdout().GetBuffer().GetTerminalStreamBuf());
        this->GetKoala().GetStdout().flush();
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
void ProgressBarManager<TKOALA>::DeleteProgressBar(const std::string &name)
{
    {
        const auto lock = WriteLock{m_mutex};
        this->DeleteProgressBarImpl(name);
    }

    if (m_displayProgressBars)
    {
        const auto lock = ReadLock{m_mutex};
        this->PrintProgressBars(this->GetKoala().GetStdout().GetBuffer().GetTerminalStreamBuf());
        this->GetKoala().GetStdout().flush();
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline auto &ProgressBarManager<TKOALA>::GetKoala() const
{
    if (const auto spKoala = m_wpKoala.lock()) return *spKoala;

    KL_THROW("Progress bar manager could not lock its weak pointer to the Koala instance");
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
void ProgressBarManager<TKOALA>::PrintProgressBarsOnLoop()
{
    while (m_loopPrintProgressBars && m_displayProgressBars)
    {
        {
            // Clean up and reprint.
            {
                const auto lock = WriteLock{m_mutex};
                this->DeleteDeadBars();
            }

            const auto lock = ReadLock{m_mutex};
            this->PrintProgressBars(
                this->GetKoala().GetStdout().GetBuffer().GetTerminalStreamBuf());
            this->GetKoala().GetStdout().flush();
        }

        std::this_thread::sleep_for(Milliseconds(m_progressBarLatency));
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
void ProgressBarManager<TKOALA>::PrintProgressBars(std::streambuf *const pStreamBuf)
{
    if (!m_displayProgressBars || !pStreamBuf) return;

    const auto [progressString, numberOfBars] = OutputUtility::PrintProgressBars(
        m_progressBarNames, m_progressBarMap, m_lastNumberOfBars, this->GetKoala().TerminalWidth());
    m_lastNumberOfBars = numberOfBars;

    pStreamBuf->sputn(progressString.c_str(),
                      static_cast<std::streamsize>(progressString.length()));
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
void ProgressBarManager<TKOALA>::DeleteDeadBars()
{
    for (auto iter = m_progressBarMap.begin(); iter != m_progressBarMap.end(); /* no increment */)
    {
        if (const auto spProgressBar = iter->second)
        {
            if (spProgressBar->Completed() && spProgressBar->AutoDelete())
            {
                if (spProgressBar->TimeElapsedSinceCompletion() >= spProgressBar->AutoDeleteDelay())
                {
                    iter = this->DeleteProgressBarImpl(iter->first);
                    continue;
                }
            }
        }

        ++iter;
    }
}

//--------------------------------------------------------------------------------------------------

#ifdef KOALA_ENABLE_CEREAL
template <typename TKOALA>
template <typename TARCHIVE>
inline void ProgressBarManager<TKOALA>::serialize(TARCHIVE &archive)
{
    archive(m_wpKoala, m_progressBarMap, m_progressBarNames, m_progressBarLatency);

    // Note that the future is not archived, the loop-print boolean is left as false, and the last
    // number of bars is left as 0.
}
#endif  // #ifdef KOALA_ENABLE_CEREAL
}  // namespace kl

#endif  // #ifndef KL_PROGRESS_BAR_MANAGER_IMPL_H