/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/OutputUtility.h
 *
 * @brief Header file for the output utility (OutputUtility) class.
 */

#ifndef KL_OUTPUT_UTILITY_H
#define KL_OUTPUT_UTILITY_H 1

#include "koala/Definitions/ColourDefinitions.h"
#include "koala/Definitions/CursorDefinitions.h"
#include "koala/ProgressBar.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <cmath>
#include <iomanip>
#include <iostream>

namespace kl
{
/**
 * @brief Forward declaration of the Koala class.
 */
class Koala;

/**
 * @brief Forward declaration of the FunctionRecorder class.
 */
class FunctionRecorder;

/**
 * @brief Forward declaration of the ProgressBarManager class template.
 */
template <typename T>
class ProgressBarManager;

/**
 * @brief OutputUtility class.
 */
class OutputUtility
{
public:
    /**
     * @brief Enum enumerating the different types of message.
     */
    enum class MESSAGE_TYPE
    {
        MESSAGE,            ///< A general message.
        WARNING,            ///< A warning message.
        INTERFACE_WARNING,  ///< An interface warning message.
        RECORDER,           ///< A function recorder message.
        ERROR,              ///< An error message.
        STACK_TRACE,        ///< A stack trace.
        ALGORITHM,          ///< A start-of-algorithm declaration.
        PAUSED,             ///< A paused message.
        NONE                ///< A message with no leading string.
    };

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Deleted constructor.
     */
    OutputUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    OutputUtility(const OutputUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    OutputUtility(OutputUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    OutputUtility &operator=(const OutputUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    OutputUtility &operator=(OutputUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~OutputUtility() = delete;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Print something to std::cout.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     * @param label The message label to print.
     * @param format The format string.
     * @param koala The instance of Koala.
     */
    template <typename TKOALA>
    static void Cout(const TKOALA &koala, const std::string &message,
                     const std::string &label = std::string{},
                     const std::string &format = KL_NORMAL_STR);

    /**
     * @brief Print something to std::cerr.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     * @param label The message label to print.
     * @param format The format string.
     */
    template <typename TKOALA>
    static void Cerr(const TKOALA &koala, const std::string &message,
                     const std::string &label = std::string{},
                     const std::string &format = KL_NORMAL_STR);

    /**
     * @brief Print message.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     * @param fileName The file name.
     * @param lineNumber The line number.
     * @param functionName The function name.
     * @param labelString The label.
     */
    template <typename TKOALA>
    static void PrintMessage(const TKOALA &koala, const std::string &message,
                             [[maybe_unused]] const std::string &fileName,
                             [[maybe_unused]] const std::size_t lineNumber,
                             [[maybe_unused]] const std::string &functionName,
                             const std::string &labelString = "koala");

    /**
     * @brief Print message.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     * @param labelString The label.
     */
    template <typename TKOALA>
    static void PrintMessage(const TKOALA &koala, const std::string &message,
                             const std::string &labelString = "koala");

    /**
     * @brief Print paursed message.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     * @param fileName The file name.
     * @param lineNumber The line number.
     * @param functionName The function name.
     */
    template <typename TKOALA>
    static void PrintPausedMessage(const TKOALA &koala, const std::string &message,
                                   [[maybe_unused]] const std::string &fileName,
                                   [[maybe_unused]] const std::size_t lineNumber,
                                   [[maybe_unused]] const std::string &functionName);

    /**
     * @brief Print paursed message.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     */
    template <typename TKOALA>
    static void PrintPausedMessage(const TKOALA &koala, const std::string &message);

    /**
     * @brief Print algorithm info.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     */
    template <typename TKOALA>
    static void PrintAlgorithm(const TKOALA &koala, const std::string &message);

    /**
     * @brief Print warning.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     * @param fileName The file name.
     * @param lineNumber The line number.
     * @param functionName The function name.
     */
    template <typename TKOALA>
    static void PrintWarning(const TKOALA &koala, const std::string &message,
                             [[maybe_unused]] const std::string &fileName,
                             [[maybe_unused]] const std::size_t lineNumber,
                             [[maybe_unused]] const std::string &functionName);

    /**
     * @brief Print interface warning.
     *
     * @param koala The instance of Koala.
     * @param message The message to print.
     * @param fileName The file name.
     * @param lineNumber The line number.
     * @param functionName The function name.
     */
    template <typename TKOALA>
    static void PrintInterfaceWarning(const TKOALA &koala, const std::string &message,
                                      [[maybe_unused]] const std::string &fileName,
                                      [[maybe_unused]] const std::size_t lineNumber,
                                      [[maybe_unused]] const std::string &functionName);

    /**
     * @brief Print error.
     *
     * @param message The message to print.
     * @param fileName The file name.
     * @param lineNumber The line number.
     * @param functionName The function name.
     * @param numberOfColumns The number of available columns.
     */
    template <typename TKOALA>
    static void PrintError(const std::string &message, const std::size_t numberOfColumns,
                           [[maybe_unused]] const std::string &fileName,
                           [[maybe_unused]] const std::size_t lineNumber,
                           [[maybe_unused]] const std::string &functionName);

    /**
     * @brief Print error.
     *
     * @param koalaException The KoalaException object.
     * @param numberOfColumns The number of available columns.
     */
    template <typename TKOALA>
    static void PrintError(const KoalaException &koalaException, const std::size_t numberOfColumns);

protected:
    /**
     * @brief Print the progress bars.
     *
     * @param progressBarNames The ordered list of progress bar names.
     * @param progressBarMap The map from names to progress bars.
     * @param numberOfBars The last number of bars printed.
     * @param numberOfColumns The number of available columns.
     *
     * @return The progress bar string and the number of bars printed.
     */
    static std::tuple<std::string, std::size_t> PrintProgressBars(
        const std::vector<std::string> &progressBarNames,
        const ProgressBar::NameMap &progressBarMap, const std::size_t numberOfBars,
        const std::size_t numberOfColumns);

    /**
     * @brief Print recorder start info.
     *
     * @param koala The instance of Koala.
     * @param indentationLevel The indentation level.
     * @param message The message to print.
     */
    template <typename TKOALA>
    static void PrintRecorderStart(const TKOALA &koala, const std::size_t indentationLevel,
                                   const std::string &message);

    /**
     * @brief Print recorder end info.
     *
     * @param koala The instance of Koala.
     * @param indentationLevel The indentation level.
     * @param message The message to print.
     * @param elapsedString The time elapsed string.
     */
    template <typename TKOALA>
    static void PrintRecorderEnd(const TKOALA &koala, const std::size_t indentationLevel,
                                 const std::string &message, const std::string &elapsedString);

    /**
     * @brief Erase the progress bars.
     *
     * @param numberOfBars The number of bars.
     *
     * @return The erasing string.
     */
    static std::string EraseProgressBars(const std::size_t numberOfBars);

    friend class Koala;
    friend class FunctionRecorder;

    template <typename T>
    friend class ProgressBarManager;

private:
    /**
     * @brief Get the origin string.
     *
     * @param fileName The file name.
     * @param lineNumber The line number.
     * @param functionName The function name.
     * @param format The format string.
     * @param secondaryFormat The secondary format string.
     *
     * @return The number of printed visible characters.
     */
    static std::string GetOriginString(const std::string &fileName, const std::size_t lineNumber,
                                       const std::string &functionName, const std::string &format,
                                       const std::string &secondaryFormat);

    /**
     * @brief Print something.
     *
     * @param messageType The message type.
     * @param indentation The indentation.
     * @param message The message to print.
     * @param outputObject The output object.
     * @param debugMode Whether we are in debug mode.
     * @param format The format string.
     * @param endOfLine Whether to print a new line at the end of the message.
     * @param algoCallDepth The algorithm call depth.
     * @param extraIndentation Extra indentation for the wrapping.
     * @param numberOfColumns The number of available columns.
     *
     * @return The number of printed visible characters.
     */
    template <typename TKOALA, typename TOUTPUT>
    static std::size_t Print(const MESSAGE_TYPE messageType, const std::size_t indentation,
                             const std::string &message, TOUTPUT &outputObject,
                             const bool debugMode, const std::string &format, const bool endOfLine,
                             const std::size_t algoCallDepth, const std::size_t extraIndentation,
                             const std::size_t numberOfColumns);

    /**
     * @brief Print something.
     *
     * @param messageType The message type.
     * @param indentation The indentation.
     * @param messages The message to print.
     * @param outputObject The output object.
     * @param debugMode Whether we are in debug mode.
     * @param format The format string.
     * @param algoCallDepth The algorithm call depth.
     * @param numberMessages Whether to number the messages.
     * @param extraIndentation Extra indentation for the wrapping.
     * @param numberOfColumns The number of available columns.
     *
     * @return The number of printed visible characters.
     */
    template <typename TKOALA, typename TOUTPUT>
    static std::size_t Print(const MESSAGE_TYPE messageType, const std::size_t indentation,
                             const KoalaException::StringList &messages, TOUTPUT &outputObject,
                             const bool debugMode, const std::string &format,
                             const std::size_t algoCallDepth, const bool numberMessages,
                             const std::size_t extraIndentation, const std::size_t numberOfColumns);

    /**
     * @brief Print the leading string to a string stream object.
     *
     * @param messageType The message type.
     * @param indentation The indentation.
     * @param stringStream The string stream to which to print.
     *
     * @return The number of printed visible characters.
     */
    static std::size_t InsertLeadingString(const MESSAGE_TYPE messageType,
                                           const std::size_t indentation,
                                           std::ostringstream &stringStream);

    /**
     * @brief Get the indentation.
     *
     * @param algorithmIndentation The algorithm indentation level.
     * @param functionIndentation The function indentation level.
     * @param debugMode Whether we are in debug mode.
     * @param bump Whether to bump the indentation to take bars into account.
     *
     * @return The number of printed visible characters.
     */
    static std::size_t GetIndentation(const std::size_t algorithmIndentation,
                                      const std::size_t functionIndentation, const bool debugMode,
                                      const bool bump = true) noexcept;

    /**
     * @brief Print a line with wrapping.
     *
     * @param outputObject The output object.
     * @param stringStream The string stream to print.
     * @param debugMode Whether we are in debug mode.
     * @param endOfLine Whether to print a new line at the end of the message.
     * @param indentation The number of indentation characters.
     * @param algoCallDepth The algorithm call depth.
     * @param numberOfColumns The number of available columns.
     *
     * @return The number of printed visible characters.
     */
    template <typename TKOALA, typename TOUTPUT>
    static void WrapPrint(TOUTPUT &outputObject, const std::ostringstream &stringStream,
                          const bool debugMode, const bool endOfLine, const std::size_t indentation,
                          const std::size_t algoCallDepth, const std::size_t numberOfColumns);

    /**
     * @brief Turn a number of seconds into hours, minutes, seconds.
     *
     * @param totalSeconds The total time elapsed in seconds.
     *
     * @return Number of hours, minutes and seconds.
     */
    static std::tuple<std::size_t, std::size_t, std::size_t> GetTimeElapsed(
        const std::size_t totalSeconds);

    /**
     * @brief Print a progress bar.
     *
     * @param progressBar The progress bar.
     * @param longestLabelLength The longest label length.
     * @param numberOfColumns The number of available columns.
     *
     * @return Whether the progress bar is complete.
     */
    static std::string GetProgressBarString(const ProgressBar &progressBar,
                                            const std::size_t longestLabelLength,
                                            const std::size_t numberOfColumns);

    /**
     * @brief Write the progress bar time elapsed string.
     *
     * @param progressBar The progress bar.
     *
     * @return The time elapsed string.
     */
    static std::ostringstream WriteTimeElapsedString(const ProgressBar &progressBar);

    /**
     * @brief Write the progress bar ETA string.
     *
     * @param progressBar The progress bar.
     *
     * @return The ETA string.
     */
    static std::ostringstream WriteEtaString(const ProgressBar &progressBar);
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::Cout(const TKOALA &koala, const std::string &message,
                                const std::string &label, const std::string &format)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};
    auto stringStream = std::ostringstream{};

    if (!label.empty())
    {
        // Print the preceding label.
        Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), std::string{"["} + label + std::string{"] "},
                      stringStream, koala.DebugMode(), KL_NORMAL_STR, false, SIZE_T(0UL),
                      SIZE_T(0UL), koala.TerminalWidth());
    }

    Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), message, stringStream, koala.DebugMode(), format,
                  false, SIZE_T(0UL), SIZE_T(0UL), koala.TerminalWidth());

    Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), stringStream.str(), koala.GetStdout(),
                  koala.DebugMode(), KL_NORMAL_STR, true, SIZE_T(0UL), SIZE_T(0UL),
                  koala.TerminalWidth());
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::Cerr(const TKOALA &koala, const std::string &message,
                                const std::string &label, const std::string &format)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};
    auto stringStream = std::ostringstream{};

    if (!label.empty())
    {
        // Print the preceding label.
        Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), std::string{"["} + label + std::string{"] "},
                      stringStream, koala.DebugMode(), KL_NORMAL_STR, SIZE_T(0UL), false,
                      SIZE_T(0UL), SIZE_T(0UL), koala.TerminalWidth());
    }

    Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), message, stringStream, koala.DebugMode(), format,
                  false, SIZE_T(0UL), SIZE_T(0UL), koala.TerminalWidth());

    Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), stringStream.str(), koala.GetStderr(),
                  koala.DebugMode(), format, true, SIZE_T(0UL), SIZE_T(0UL), koala.TerminalWidth());
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintMessage(const TKOALA &koala, const std::string &message,
                                        [[maybe_unused]] const std::string &fileName,
                                        [[maybe_unused]] const std::size_t lineNumber,
                                        [[maybe_unused]] const std::string &functionName,
                                        const std::string &labelString)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};
    auto &outputObject = koala.GetStdout();

    const auto messageWithLabel = "[" + labelString + "] " + message;
    const auto indentation =
        GetIndentation(koala.CallLevel() + SIZE_T(1UL), SIZE_T(0UL), koala.DebugMode());

    [[maybe_unused]] const auto visibleChars = Print<TKOALA>(
        MESSAGE_TYPE::MESSAGE, indentation, messageWithLabel, outputObject, koala.DebugMode(),
        KL_NORMAL, true, koala.CallLevel(), SIZE_T(0UL), koala.TerminalWidth());

#ifdef KOALA_DEBUG
    // Print the origin string.
    const auto originString =
        GetOriginString(fileName, lineNumber, functionName, KL_DARK_GREY_STR, KL_DARK_GREY_STR);
    Print<TKOALA>(MESSAGE_TYPE::NONE, visibleChars, originString, outputObject, koala.DebugMode(),
                  KL_DARK_GREY_STR, true, koala.CallLevel(), SIZE_T(0UL), koala.TerminalWidth());
#endif  // #ifdef KOALA_DEBUG
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintMessage(const TKOALA &koala, const std::string &message,
                                        const std::string &labelString)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};
    auto &outputObject = koala.GetStdout();

    const auto messageWithLabel = "[" + labelString + "] " + message;
    const auto indentation =
        GetIndentation(koala.CallLevel() + SIZE_T(1UL), SIZE_T(0UL), koala.DebugMode());

    Print<TKOALA>(MESSAGE_TYPE::MESSAGE, indentation, messageWithLabel, outputObject,
                  koala.DebugMode(), KL_NORMAL, true, koala.CallLevel(), SIZE_T(0UL),
                  koala.TerminalWidth());
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintPausedMessage(const TKOALA &koala, const std::string &message,
                                              [[maybe_unused]] const std::string &fileName,
                                              [[maybe_unused]] const std::size_t lineNumber,
                                              [[maybe_unused]] const std::string &functionName)
{
    auto visibleChars = SIZE_T(0UL);
    auto &outputObject = koala.GetStdout();

    {
        const auto lock = WriteLock{koala.GetGlobalMutex()};
        const auto indentation =
            GetIndentation(koala.CallLevel() + SIZE_T(1UL), SIZE_T(0UL), koala.DebugMode());

        visibleChars = Print<TKOALA>(MESSAGE_TYPE::PAUSED, indentation, message, outputObject,
                                     koala.DebugMode(), KL_WHITE_BOLD_STR, true, koala.CallLevel(),
                                     SIZE_T(0UL), koala.TerminalWidth());
    }

#ifdef KOALA_DEBUG
    // Print the origin string.
    const auto originString =
        GetOriginString(fileName, lineNumber, functionName, KL_YELLOW_STR, KL_NORMAL_STR);
    Print<TKOALA>(MESSAGE_TYPE::NONE, visibleChars, originString, outputObject, koala.DebugMode(),
                  KL_YELLOW_STR, true, koala.CallLevel(), SIZE_T(0UL), koala.TerminalWidth());
#endif  // #ifdef KOALA_DEBUG

    outputObject << std::flush;
    std::cin.ignore();  // wait for keystroke
    outputObject << KL_UP(1) << std::flush;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintPausedMessage(const TKOALA &koala, const std::string &message)
{
    auto &outputObject = koala.GetStdout();

    {
        const auto lock = WriteLock{koala.GetGlobalMutex()};
        const auto indentation =
            GetIndentation(koala.CallLevel() + SIZE_T(1UL), SIZE_T(0UL), koala.DebugMode());

        Print<TKOALA>(MESSAGE_TYPE::PAUSED, indentation, message, outputObject, koala.DebugMode(),
                      KL_WHITE_BOLD_STR, true, koala.CallLevel(), SIZE_T(0UL),
                      koala.TerminalWidth());
    }

    outputObject << std::flush;
    std::cin.ignore();  // wait for keystroke
    outputObject << KL_UP(1) << std::flush;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintAlgorithm(const TKOALA &koala, const std::string &message)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};
    const auto indentation = GetIndentation(koala.CallLevel(), SIZE_T(0UL), koala.DebugMode());

    Print<TKOALA>(MESSAGE_TYPE::ALGORITHM, indentation, message, koala.GetStdout(),
                  koala.DebugMode(), KL_GREEN_STR, true,
                  (koala.CallLevel() > SIZE_T(0UL)) ? koala.CallLevel() - SIZE_T(1UL) : SIZE_T(0UL),
                  SIZE_T(0UL), koala.TerminalWidth());
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintWarning(const TKOALA &koala, const std::string &message,
                                        [[maybe_unused]] const std::string &fileName,
                                        [[maybe_unused]] const std::size_t lineNumber,
                                        [[maybe_unused]] const std::string &functionName)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};

    auto &outputObject = koala.GetStderr();
    const auto indentation =
        GetIndentation(koala.CallLevel() + SIZE_T(1UL), SIZE_T(0UL), koala.DebugMode());

    [[maybe_unused]] const auto visibleChars = Print<TKOALA>(
        MESSAGE_TYPE::WARNING, indentation, message, outputObject, koala.DebugMode(),
        KL_YELLOW_BOLD_STR, true, koala.CallLevel(), SIZE_T(0UL), koala.TerminalWidth());

#ifdef KOALA_DEBUG
    // Print the origin string.
    const auto originString =
        GetOriginString(fileName, lineNumber, functionName, KL_YELLOW_STR, KL_NORMAL_STR);
    Print<TKOALA>(MESSAGE_TYPE::NONE, visibleChars, originString, outputObject, koala.DebugMode(),
                  KL_YELLOW_STR, true, koala.CallLevel(), SIZE_T(0UL), koala.TerminalWidth());
#endif  // #ifdef KOALA_DEBUG
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintInterfaceWarning(const TKOALA &koala, const std::string &message,
                                                 [[maybe_unused]] const std::string &fileName,
                                                 [[maybe_unused]] const std::size_t lineNumber,
                                                 [[maybe_unused]] const std::string &functionName)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};

    auto &outputObject = koala.GetStderr();
    [[maybe_unused]] const auto visibleChars =
        Print<TKOALA>(MESSAGE_TYPE::INTERFACE_WARNING, SIZE_T(0UL), message, outputObject,
                      koala.DebugMode(), KL_NORMAL_STR, true, SIZE_T(0UL), SIZE_T(0UL),

                      koala.TerminalWidth());

#ifdef KOALA_DEBUG
    // Print the origin string.
    const auto originString =
        GetOriginString(fileName, lineNumber, functionName, KL_YELLOW_STR, KL_NORMAL_STR);
    Print<TKOALA>(MESSAGE_TYPE::NONE, visibleChars, originString, outputObject, false,
                  KL_YELLOW_STR, true, SIZE_T(0UL), SIZE_T(0UL), koala.TerminalWidth());
#endif  // #ifdef KOALA_DEBUG
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintError(const std::string &message, const std::size_t numberOfColumns,
                                      [[maybe_unused]] const std::string &fileName,
                                      [[maybe_unused]] const std::size_t lineNumber,
                                      [[maybe_unused]] const std::string &functionName)
{
    auto &outputObject = std::cerr;
    [[maybe_unused]] const auto visibleChars =
        Print<TKOALA>(MESSAGE_TYPE::ERROR, SIZE_T(0UL), message, outputObject, false, KL_NORMAL_STR,
                      true, SIZE_T(0UL), SIZE_T(0UL), numberOfColumns);

#ifdef KOALA_DEBUG
    // Print the origin string.
    const auto originString =
        GetOriginString(fileName, lineNumber, functionName, KL_YELLOW_STR, KL_NORMAL_STR);
    Print<TKOALA>(MESSAGE_TYPE::NONE, visibleChars, originString, outputObject, false,
                  KL_YELLOW_STR, true, SIZE_T(0UL), SIZE_T(0UL), numberOfColumns);
#endif  // #ifdef KOALA_DEBUG
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintError(const KoalaException &koalaException,
                                      const std::size_t numberOfColumns)
{
    auto &outputObject = std::cerr;
    [[maybe_unused]] const auto visibleChars = Print<TKOALA>(
        MESSAGE_TYPE::ERROR, SIZE_T(0UL), koalaException.ErrorMessages(), outputObject, false,
        KL_NORMAL_STR, true, SIZE_T(0UL), SIZE_T(0UL), numberOfColumns);

#ifdef KOALA_DEBUG
    // Print the origin string and the stack trace.
    const auto originString =
        GetOriginString(koalaException.FileName(), koalaException.LineNumber(),
                        koalaException.FunctionName(), KL_YELLOW_STR, KL_NORMAL_STR);

    Print<TKOALA>(MESSAGE_TYPE::NONE, visibleChars, originString, outputObject, false,
                  KL_YELLOW_STR, true, SIZE_T(0UL), SIZE_T(0UL), numberOfColumns);

    Print<TKOALA>(MESSAGE_TYPE::STACK_TRACE, SIZE_T(0UL), std::string{}, outputObject, false,
                  KL_NORMAL_STR, true, SIZE_T(0UL), SIZE_T(0UL), numberOfColumns);

    Print<TKOALA>(MESSAGE_TYPE::NONE, visibleChars, koalaException.StackTrace(), outputObject,
                  false, KL_NORMAL_STR, SIZE_T(0UL), true, SIZE_T(0UL), numberOfColumns);
#endif  // #ifdef KOALA_DEBUG
}

//--------------------------------------------------------------------------------------------------

inline std::tuple<std::string, std::size_t> OutputUtility::PrintProgressBars(
    const std::vector<std::string> &progressBarNames, const ProgressBar::NameMap &progressBarMap,
    const std::size_t numberOfBars, const std::size_t numberOfColumns)
{
    // First get the number of bars to print and the longest label length.
    auto numberOfBarsToPrint = SIZE_T(0UL);
    auto longestLabelLength = SIZE_T(0UL);

    for (const auto &name : progressBarNames)
    {
        const auto findIter = progressBarMap.find(name);

        if (findIter == progressBarMap.end()) continue;

        const auto labelLength = findIter->second->Label().length();

        if (labelLength > longestLabelLength) longestLabelLength = labelLength;

        ++numberOfBarsToPrint;
    }

    auto reprintString = OutputUtility::EraseProgressBars(numberOfBars);

    // Print the progress bars backwards, moving down.
    for (auto riter = progressBarNames.rbegin(); riter != progressBarNames.rend(); ++riter)
    {
        const auto findIter = progressBarMap.find(*riter);

        if (findIter == progressBarMap.end()) continue;

        reprintString += OutputUtility::GetProgressBarString(*findIter->second, longestLabelLength,
                                                             numberOfColumns);
    }

    if (numberOfBarsToPrint > 1UL) reprintString += KL_UP_STR(numberOfBarsToPrint - 1UL);

    reprintString += '\r';

    return std::make_tuple((numberOfBarsToPrint > SIZE_T(0UL)) ? reprintString : std::string{},
                           numberOfBarsToPrint);
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintRecorderStart(const TKOALA &koala,
                                              const std::size_t indentationLevel,
                                              const std::string &message)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};

    const auto indentation = GetIndentation(koala.CallLevel(), indentationLevel, koala.DebugMode());
    Print<TKOALA>(MESSAGE_TYPE::RECORDER, indentation, message, koala.GetStdout(),
                  koala.DebugMode(), KL_YELLOW_STR, true, koala.CallLevel(), SIZE_T(0UL),
                  koala.TerminalWidth());
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA>
inline void OutputUtility::PrintRecorderEnd(const TKOALA &koala, const std::size_t indentationLevel,
                                            const std::string &message,
                                            const std::string &elapsedString)
{
    const auto lock = WriteLock{koala.GetGlobalMutex()};

    auto stringStream = std::ostringstream{};
    const auto indentation = GetIndentation(koala.CallLevel(), indentationLevel, koala.DebugMode());

    const auto visibleChars =
        Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), message, stringStream, koala.DebugMode(),
                      KL_RED_STR, false, koala.CallLevel(), SIZE_T(0UL), koala.TerminalWidth());

    Print<TKOALA>(MESSAGE_TYPE::NONE, SIZE_T(0UL), elapsedString, stringStream, koala.DebugMode(),
                  KL_RED_BOLD_STR, false, SIZE_T(0UL), SIZE_T(0UL), koala.TerminalWidth());

    Print<TKOALA>(MESSAGE_TYPE::RECORDER, indentation, stringStream.str(), koala.GetStdout(),
                  koala.DebugMode(), KL_NORMAL_STR, true, koala.CallLevel(), visibleChars,
                  koala.TerminalWidth());
}

//--------------------------------------------------------------------------------------------------

inline std::string OutputUtility::EraseProgressBars(const std::size_t numberOfBars)
{
    auto eraseString = std::string{};
    auto firstTime = true;

    for (auto i = SIZE_T(0UL); i < numberOfBars; ++i)
    {
        if (firstTime)
        {
            eraseString += KL_CLEAR_STR;
            firstTime = false;
        }

        else
            eraseString += '\n' + KL_CLEAR_STR;
    }

    if (numberOfBars > 1UL) eraseString += KL_UP_STR(numberOfBars - 1UL);

    return eraseString;
}

//--------------------------------------------------------------------------------------------------

inline std::string OutputUtility::GetOriginString(const std::string &fileName,
                                                  const std::size_t lineNumber,
                                                  const std::string &functionName,
                                                  const std::string &format,
                                                  const std::string &secondaryFormat)
{
    return secondaryFormat + "@ " + format + fileName + secondaryFormat + ":" + format +
           std::to_string(lineNumber) + secondaryFormat + " ~ " + format + functionName;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA, typename TOUTPUT>
inline std::size_t OutputUtility::Print(const MESSAGE_TYPE messageType,
                                        const std::size_t indentation, const std::string &message,
                                        TOUTPUT &outputObject, const bool debugMode,
                                        const std::string &format, const bool endOfLine,
                                        const std::size_t algoCallDepth,
                                        const std::size_t extraIndentation,
                                        const std::size_t numberOfColumns)
{
    auto stringStream = std::ostringstream{};
    const auto visibleChars = InsertLeadingString(messageType, indentation, stringStream);
    stringStream << std::dec << format << message;

    WrapPrint<TKOALA>(outputObject, stringStream, debugMode, endOfLine,
                      visibleChars + extraIndentation, algoCallDepth, numberOfColumns);

    outputObject << std::flush;
    return visibleChars;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA, typename TOUTPUT>
inline std::size_t OutputUtility::Print(
    const MESSAGE_TYPE messageType, const std::size_t indentation,
    const KoalaException::StringList &messages, TOUTPUT &outputObject, const bool debugMode,
    const std::string &format, const std::size_t algoCallDepth, const bool numberMessages,
    const std::size_t extraIndentation, const std::size_t numberOfColumns)
{
    auto stringStream = std::ostringstream{};
    const auto visibleChars = InsertLeadingString(messageType, indentation, stringStream);

    auto firstTime = true;

    const auto maxSpacing = std::to_string(messages.size() - SIZE_T(1UL)).length();
    auto messageCount = SIZE_T(0UL);

    for (const auto &message : messages)
    {
        const auto numberString =
            numberMessages
                ? std::string{"("} + std::to_string(messageCount) + std::string{") "} +
                      std::string(maxSpacing - std::to_string(messageCount).length(), ' ')
                : std::string{};
        ++messageCount;

        if (firstTime)
        {
            stringStream << std::dec << KL_NORMAL << numberString << format << message << KL_NORMAL;
            firstTime = false;
        }

        else  // the remaining lines are indented and without the leading characters
        {
            stringStream << std::string(visibleChars, ' ') << std::dec << KL_NORMAL << numberString
                         << format << message << KL_NORMAL;
        }

        WrapPrint<TKOALA>(outputObject, stringStream, debugMode, true,
                          visibleChars + maxSpacing + SIZE_T(3UL) + extraIndentation, algoCallDepth,
                          numberOfColumns);

        stringStream.str(std::string{});
    }

    outputObject << std::flush;
    return visibleChars;
}

//--------------------------------------------------------------------------------------------------

inline std::size_t OutputUtility::InsertLeadingString(const MESSAGE_TYPE messageType,
                                                      const std::size_t indentation,
                                                      std::ostringstream &stringStream)
{
    stringStream << std::string(indentation, ' ');

    auto visibleLeadingChars = SIZE_T(0UL);

    switch (messageType)
    {
        case MESSAGE_TYPE::MESSAGE:
        {
            break;
        }

        case MESSAGE_TYPE::WARNING:
        {
            const auto leadingString = std::string{"[warning] "};
            stringStream << KL_NORMAL << leadingString;
            visibleLeadingChars = leadingString.length();
            break;
        }

        case MESSAGE_TYPE::RECORDER:
        {
            const auto leadingString =
                std::string{"            "} + KL_NORMAL_STR + std::string{"- "};
            stringStream << leadingString;
            visibleLeadingChars = leadingString.length() - KL_NORMAL_STR.length();
            break;
        }

        case MESSAGE_TYPE::PAUSED:
        {
            const auto leadingString = std::string{"[pausing] "};
            stringStream << KL_NORMAL << leadingString;
            visibleLeadingChars = leadingString.length();
            break;
        }

        case MESSAGE_TYPE::INTERFACE_WARNING:
        {
            const auto leadingString = std::string{"Warning: "};
            stringStream << KL_YELLOW_BOLD << leadingString;
            visibleLeadingChars = leadingString.length();
            break;
        }

        case MESSAGE_TYPE::ERROR:
        {
            const auto leadingString = std::string{"Error: "};
            stringStream << KL_RTN_CLEAR_STR << KL_RED_BOLD << leadingString;
            visibleLeadingChars = leadingString.length();
            break;
        }

        case MESSAGE_TYPE::STACK_TRACE:
        {
            const auto leadingString = std::string{"Stack trace: "};
            stringStream << KL_RED_BOLD << leadingString;
            visibleLeadingChars = leadingString.length();
            break;
        }

        case MESSAGE_TYPE::ALGORITHM:
        {
            const auto leadingString = std::string{"[running] "};
            stringStream << KL_NORMAL << leadingString;
            visibleLeadingChars = leadingString.length();
            break;
        }

        case MESSAGE_TYPE::NONE:
        default:
            break;
    }

    return indentation + visibleLeadingChars;
}

//--------------------------------------------------------------------------------------------------

inline std::size_t OutputUtility::GetIndentation(const std::size_t algorithmIndentation,
                                                 const std::size_t functionIndentation,
                                                 const bool debugMode, const bool bump) noexcept
{
    if (!debugMode) return SIZE_T(0UL);

    auto bumpIndentation = SIZE_T(0UL);

    if (algorithmIndentation > SIZE_T(0UL))
        bumpIndentation += SIZE_T(2UL) * (algorithmIndentation - SIZE_T(1UL));

    if (!bump && bumpIndentation >= SIZE_T(2UL)) bumpIndentation -= SIZE_T(2UL);

    auto indentation = bumpIndentation;

    if (algorithmIndentation > SIZE_T(0UL))
        indentation += SIZE_T(10UL) * (algorithmIndentation - SIZE_T(1UL));

    if (functionIndentation > SIZE_T(0UL))
        indentation += SIZE_T(4UL) * (functionIndentation - SIZE_T(1UL));

    return indentation;
}

//--------------------------------------------------------------------------------------------------

template <typename TKOALA, typename TOUTPUT>
inline void OutputUtility::WrapPrint(TOUTPUT &outputObject, const std::ostringstream &stringStream,
                                     const bool debugMode, const bool endOfLine,
                                     const std::size_t indentation, const std::size_t algoCallDepth,
                                     const std::size_t numberOfColumns)
{
    auto outputString = stringStream.str();
    auto numberOfLines = SIZE_T(1UL);

    auto barNumber = SIZE_T(1UL);
    auto barIndentation = SIZE_T(0UL);

    if (algoCallDepth > SIZE_T(0UL))
        barIndentation = GetIndentation(barNumber + SIZE_T(1UL), SIZE_T(0UL), debugMode, false);

    if (endOfLine)
    {
        auto currentColumn = SIZE_T(0UL);
        auto resetColumnNumber = false;

        for (auto i = SIZE_T(0UL); i < outputString.length(); ++i)
        {
            if (resetColumnNumber || outputString[i] == '\r')
            {
                currentColumn = SIZE_T(0UL);
                barNumber = SIZE_T(1UL);
                barIndentation = SIZE_T(0UL);

                if (algoCallDepth > SIZE_T(0UL))
                    barIndentation =
                        GetIndentation(barNumber + SIZE_T(1UL), SIZE_T(0UL), debugMode, false);

                resetColumnNumber = false;
            }

            if (currentColumn > SIZE_T(0UL) && currentColumn == barIndentation)
            {
                outputString[i] = '|';
                ++barNumber;

                if (barNumber <= algoCallDepth)
                    barIndentation =
                        GetIndentation(barNumber + SIZE_T(1UL), SIZE_T(0UL), debugMode, false);
            }

            if (numberOfColumns >= indentation + SIZE_T(20UL))
            {
                // If we hit a user-defined newline, then reset everything.
                if (outputString[i] == '\n')
                {
                    outputString.insert(i + SIZE_T(1UL), std::string(indentation, ' '));
                    resetColumnNumber = true;

                    if (algoCallDepth > SIZE_T(0UL))
                        barIndentation =
                            GetIndentation(barNumber + SIZE_T(1UL), SIZE_T(0UL), debugMode, false);

                    ++numberOfLines;
                }

                if (outputString[i] != '\n' && outputString[i] != '\r') ++currentColumn;
            }

            else
                ++currentColumn;
        }

        outputObject << std::dec << KL_RTN_CLEAR << outputString << '\n' << KL_NORMAL << std::flush;
    }

    else
        outputObject << outputString << KL_NORMAL;
}

//--------------------------------------------------------------------------------------------------

inline std::tuple<std::size_t, std::size_t, std::size_t> OutputUtility::GetTimeElapsed(
    const std::size_t totalSeconds)
{
    auto minutes = totalSeconds / 60;
    const auto hours = minutes / 60;
    const auto seconds = totalSeconds % 60;
    minutes %= 60;

    return {hours, minutes, seconds};
}

//--------------------------------------------------------------------------------------------------

inline std::string OutputUtility::GetProgressBarString(const ProgressBar &progressBar,
                                                       const std::size_t longestLabelLength,
                                                       const std::size_t numberOfColumns)
{
    // First write out the percentage string.
    auto stringStream = std::stringstream{};
    auto percentagePaddingString = std::string{};

    auto progress = progressBar.Progress();

    if (progress < 0.f)
        progress = 0.f;

    else if (progress > 1.f)
        progress = 1.f;

    if (progress < 0.1f) percentagePaddingString += ' ';

    if (progress < 1.f) percentagePaddingString += ' ';

    stringStream << std::fixed << std::setprecision(1) << (progress * 100.f);

    const auto elapsedStringStream = OutputUtility::WriteTimeElapsedString(progressBar);
    const auto etaStringStream = OutputUtility::WriteEtaString(progressBar);

    const auto percentageString = stringStream.str() + std::string{'%'};
    const auto statusString = KL_DARK_GREY_BOLD_STR + percentageString + KL_DARK_GREY_STR +
                              std::string{' '} + elapsedStringStream.str() + etaStringStream.str() +
                              KL_NORMAL_STR;

    const auto statusStringLength = percentageString.length() + elapsedStringStream.str().length() +
                                    etaStringStream.str().length() + SIZE_T(1UL);

    // Pad the label so that the label lengths are the same.
    auto label = progressBar.Label();

    if (!label.empty())
    {
        if (label.length() < longestLabelLength)
            label += std::string(longestLabelLength - label.length(), ' ');

        label += std::string{' '};
    }

    // If not enough room, write whatever will fit.
    if (numberOfColumns <=
        percentagePaddingString.length() + statusStringLength + SIZE_T(4UL) + label.length())
    {
        if (numberOfColumns < percentageString.length())
            return KL_DARK_GREY_BOLD_STR + std::string{'?'} + KL_NORMAL_STR;

        if (numberOfColumns <
            label.length() + percentagePaddingString.length() + percentageString.length())
            return KL_DARK_GREY_BOLD_STR + percentageString + KL_NORMAL_STR;

        if (numberOfColumns <
            label.length() + statusStringLength + percentagePaddingString.length())
            return label + percentagePaddingString + KL_DARK_GREY_BOLD_STR + percentageString +
                   KL_NORMAL_STR;

        return label + percentagePaddingString + statusString;
    }

    // Write the colour string.
    auto colourString = KL_GREEN_STR;

    switch (progressBar.Colour())
    {
        case ProgressBar::COLOUR::GREEN:
            colourString = KL_GREEN_STR;
            break;
        case ProgressBar::COLOUR::RED:
            colourString = KL_RED_STR;
            break;
        case ProgressBar::COLOUR::YELLOW:
            colourString = KL_YELLOW_STR;
            break;
        case ProgressBar::COLOUR::MAGENTA:
            colourString = KL_MAGENTA_STR;
            break;
        case ProgressBar::COLOUR::BLUE:
            colourString = KL_BLUE_STR;
            break;
        case ProgressBar::COLOUR::CYAN:
            colourString = KL_CYAN_STR;
            break;
        default:
            break;
    }

    // Work out the bar size.
    const auto barWidth = numberOfColumns - statusStringLength - label.length() - SIZE_T(3UL) -
                          percentagePaddingString.length();

    const auto numFilled = SIZE_T(std::round(progress * static_cast<float>(barWidth)));
    const auto numEmpty = barWidth - numFilled;

    // Put it all together.
    auto progressString =
        KL_NORMAL_STR + label + KL_WHITE_BOLD_STR + std::string{'['} + colourString;

    progressString += std::string(numFilled, '|');
    progressString += std::string(numEmpty, ' ');
    progressString += KL_WHITE_BOLD_STR + std::string{']'} + KL_NORMAL_STR;

    progressString += std::string{' '} + percentagePaddingString + statusString;

    return progressString;
}

//--------------------------------------------------------------------------------------------------

inline std::ostringstream OutputUtility::WriteTimeElapsedString(const ProgressBar &progressBar)
{
    // Now write the time elapsed string.
    auto elapsedStringStream = std::ostringstream{};

    if (progressBar.DisplayTimeElapsed())
    {
        const auto [hours, minutes, seconds] = GetTimeElapsed(progressBar.SecondsElapsed());

        if (hours < SIZE_T(100UL))
        {
            elapsedStringStream << std::setw(2) << std::setfill('0') << hours << ':' << std::setw(2)
                                << std::setfill('0') << minutes << ':' << std::setw(2)
                                << std::setfill('0') << seconds;
        }

        else
            elapsedStringStream << ">100hrs ";
    }

    else
        elapsedStringStream << "        ";

    return elapsedStringStream;
}

//--------------------------------------------------------------------------------------------------

inline std::ostringstream OutputUtility::WriteEtaString(const ProgressBar &progressBar)
{
    auto etaStringStream = std::ostringstream{};

    if (progressBar.DisplayEta())
    {
        etaStringStream << ", ETA ";

        if (progressBar.Progress() > std::numeric_limits<float>::epsilon())
        {
            const auto predictedSeconds = SIZE_T(std::round(
                static_cast<float>(progressBar.SecondsElapsed()) / progressBar.Progress()));
            const auto [etaHours, etaMinutes, etaSeconds] =
                GetTimeElapsed(predictedSeconds - progressBar.SecondsElapsed());

            if (etaHours < SIZE_T(100UL))
            {
                etaStringStream << std::setw(2) << std::setfill('0') << etaHours << ':'
                                << std::setw(2) << std::setfill('0') << etaMinutes << ':'
                                << std::setw(2) << std::setfill('0') << etaSeconds;
            }

            else
                etaStringStream << ">100hrs ";
        }

        else
            etaStringStream << "??:??:??";
    }

    else
        etaStringStream << "              ";

    return etaStringStream;
}
}  // namespace kl

#endif  // #ifndef KL_OUTPUT_UTILITY_H