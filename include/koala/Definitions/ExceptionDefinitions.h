/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Definitions/ExceptionDefinitions.h
 *
 * @brief Header file for koala exception definitions.
 */

#ifndef KL_EXCEPTION_DEFINITIONS_H
#define KL_EXCEPTION_DEFINITIONS_H 1

#include "koala/Definitions/CommonDefinitions.h"
#include "koala/KoalaException.h"
#include "koala/Utilities/OutputUtility.h"

/**
 * @brief Macro for getting the terminal width.
 */
#define GET_TERMINAL_WIDTH()                     \
    []() {                                       \
        struct winsize wins;                     \
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &wins); \
        return SIZE_T(wins.ws_col);              \
    }()

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
#ifdef __GNUC__
#define KL_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define KL_FUNCTION_NAME __func__
#endif  // #ifdef __GNUC__

/**
 * @brief Macro for getting the current function's name.
 */
#define KL_FUNCTION_NAME_STR \
    std::string { KL_FUNCTION_NAME }

/**
 * @brief Print text to std::cout of a given text format, then revert to normal format and end the
 * line.
 *
 * @param label The label to use.
 * @param colourText The text (or macro) to set the colour/style of the message.
 * @param message The message.
 */
#define KL_COUT(label, colourText, message)                                                    \
    ::kl::OutputUtility::Cout(this->GetKoala(), STREAM_TO_STRING(message), std::string{label}, \
                              std::string{colourText})

/**
 * @brief Print text to std::cerr of a given text format, then revert to normal format and end the
 * line.
 *
 * @param label The label to use.
 * @param colourText The text (or macro) to set the colour/style of the message.
 * @param message The message.
 */
#define KL_CERR(label, colourText, message)                                                    \
    ::kl::OutputUtility::Cerr(this->GetKoala(), STREAM_TO_STRING(message), std::string{label}, \
                              std::string{colourText})

/**
 * @brief Print text to std::cout of a given text format, then revert to normal format and end the
 * line.
 *
 * @param koala The instance of Koala.
 * @param label The label to use.
 * @param colourText The text (or macro) to set the colour/style of the message.
 * @param message The message.
 */
#define KL_COUT_INST(koala, label, colourText, message)                             \
    ::kl::OutputUtility::Cout(koala, STREAM_TO_STRING(message), std::string{label}, \
                              std::string{colourText})

/**
 * @brief Print text to std::cerr of a given text format, then revert to normal format and end the
 * line.
 *
 * @param koala The instance of Koala.
 * @param label The label to use.
 * @param colourText The text (or macro) to set the colour/style of the message.
 * @param message The message.
 */
#define KL_CERR_INST(koala, label, colourText, message)                             \
    ::kl::OutputUtility::Cerr(koala, STREAM_TO_STRING(message), std::string{label}, \
                              std::string{colourText})

/**
 * @brief Macro for printing a warning if in debug mode.
 *
 * @param message The warning message.
 */
#if defined(KOALA_DEBUG)
#define KL_IF_DEBUG_WARN(message)                                                              \
    static_cast<void>(!this->GetKoala().DebugMode() ||                                         \
                      (::kl::OutputUtility::PrintWarning(                                      \
                           this->GetKoala(), STREAM_TO_STRING(message), std::string{__FILE__}, \
                           SIZE_T(__LINE__), KL_FUNCTION_NAME_STR),                            \
                       false))
#else
#define KL_IF_DEBUG_WARN(message)
#endif  // #if defined(KOALA_DEBUG)

/**
 * @brief Macro for printing a warning if in debug mode.
 *
 * @param koala The instance of Koala.
 * @param message The warning message.
 */
#if defined(KOALA_DEBUG)
#define KL_IF_DEBUG_WARN_INST(koala, message)                                                     \
    static_cast<void>(!koala.DebugMode() ||                                                       \
                      (::kl::OutputUtility::PrintWarning(koala, STREAM_TO_STRING(message),        \
                                                         std::string{__FILE__}, SIZE_T(__LINE__), \
                                                         KL_FUNCTION_NAME_STR),                   \
                       false))
#else
#define KL_IF_DEBUG_WARN_INST(koala, message)
#endif  // #if defined(KOALA_DEBUG)

/**
 * @brief Macro for printing a warning.
 *
 * @param message The warning message.
 */
#define KL_WARN(message)                                                           \
    ::kl::OutputUtility::PrintWarning(this->GetKoala(), STREAM_TO_STRING(message), \
                                      std::string{__FILE__}, SIZE_T(__LINE__),     \
                                      KL_FUNCTION_NAME_STR)

/**
 * @brief Macro for printing a warning.
 *
 * @param koala The instance of Koala.
 * @param message The warning message.
 */
#define KL_WARN_INST(koala, message)                                                           \
    ::kl::OutputUtility::PrintWarning(koala, STREAM_TO_STRING(message), std::string{__FILE__}, \
                                      SIZE_T(__LINE__), KL_FUNCTION_NAME_STR)

/**
 * @brief Macro for printing an interface-style warning.
 *
 * @param message The warning message.
 */
#define KL_INTERFACE_WARN(message)                                                          \
    ::kl::OutputUtility::PrintInterfaceWarning(this->GetKoala(), STREAM_TO_STRING(message), \
                                               std::string{__FILE__}, SIZE_T(__LINE__),     \
                                               KL_FUNCTION_NAME_STR)

/**
 * @brief Macro for printing a KoalaException and doing quick_exit if an assertion fails.
 *
 * @param condition The condition.
 * @param message The error message.
 */
#define KL_ASSERT(condition, message)                                                            \
    static_cast<void>(condition ||                                                               \
                      (::kl::OutputUtility::PrintError<::kl::Koala>(                             \
                           ::kl::KoalaException{STREAM_TO_STRING(message), KL_FUNCTION_NAME_STR, \
                                                std::string{__FILE__}, SIZE_T(__LINE__)},        \
                           GET_TERMINAL_WIDTH()),                                                \
                       false) ||                                                                 \
                      (std::quick_exit(1), false))

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

/**
 * @brief Macro for concatenating a new message with an existing KoalaException.
 *
 * @param koalaException The KoalaException.
 * @param message The error message.
 */
#define KL_CONCATENATE_EXCEPTION(koalaException, message) \
    koalaException.Concatenate(STREAM_TO_STRING(message))

/**
 * @brief Macro for concatenating a new message with an existing KoalaException then throw it
 * again.
 *
 * @param koalaException The KoalaException.
 * @param message The error message.
 */
#define KL_CONCATENATE_EXCEPTION_THROW(koalaException, message) \
    koalaException.Concatenate(STREAM_TO_STRING(message));      \
    throw koalaException

/**
 * @brief Macro for printing the message associated with a KoalaException.
 *
 * @param koalaException The KoalaException.
 */
#define KL_PRINT_EXCEPTION(koalaException) \
    ::kl::OutputUtility::PrintError<::kl::Koala>(koalaException, GET_TERMINAL_WIDTH())

/**
 * @brief Macro for printing the message associated with a KoalaException, specifying the terminal
 * width.
 *
 * @param koalaException The KoalaException.
 * @param width The terminal width.
 */
#define KL_PRINT_EXCEPTION_WIDTH(koalaException, width) \
    ::kl::OutputUtility::PrintError<::kl::Koala>(koalaException, width)

/**
 * @brief Macro for printing an exception message in the same style as a KoalaException.
 *
 * @param message The message.
 */
#define KL_PRINT_EXCEPTION_MESSAGE(message)                                                       \
    ::kl::OutputUtility::PrintError<::kl::Koala>(STREAM_TO_STRING(message), GET_TERMINAL_WIDTH(), \
                                                 std::string{__FILE__}, SIZE_T(__LINE__),         \
                                                 KL_FUNCTION_NAME_STR)

/**
 * @brief Macro for printing an exception message in the same style as a KoalaException,
 * specifying the terminal width.
 *
 * @param message The message.
 * @param width The terminal width.
 */
#define KL_PRINT_EXCEPTION_MESSAGE_WIDTH(message, width)                                  \
    ::kl::OutputUtility::PrintError<::kl::Koala>(STREAM_TO_STRING(message), width,        \
                                                 std::string{__FILE__}, SIZE_T(__LINE__), \
                                                 KL_FUNCTION_NAME_STR)

/**
 * @brief Macro for printing a message if in debug mode.
 *
 * @param message The message.
 */
#define KL_IF_DEBUG_MESSAGE(message)     \
    static_cast<void>(                   \
        !this->GetKoala().DebugMode() || \
        (::kl::OutputUtility::PrintMessage(this->GetKoala(), STREAM_TO_STRING(message)), false))

/**
 * @brief Macro for printing a message if in debug mode.
 *
 * @param koala The instance of Koala.
 * @param message The message.
 */
#define KL_IF_DEBUG_MESSAGE_INST(koala, message) \
    static_cast<void>(                           \
        !koala.DebugMode() ||                    \
        (::kl::OutputUtility::PrintMessage(koala, STREAM_TO_STRING(message)), false))

/**
 * @brief Macro for printing a message.
 *
 * @param message The message.
 */
#define KL_MESSAGE(message) \
    ::kl::OutputUtility::PrintMessage(this->GetKoala(), STREAM_TO_STRING(message))

/**
 * @brief Macro for printing a message.
 *
 * @param koala The instance of Koala.
 * @param message The message.
 */
#define KL_MESSAGE_INST(koala, message) \
    ::kl::OutputUtility::PrintMessage(koala, STREAM_TO_STRING(message))

/**
 * @brief Macro for waiting for a user's key press before continuing.
 */
#define KL_WAIT_FOR_KEYPRESS()                                                              \
    ::kl::OutputUtility::PrintPausedMessage(                                                \
        this->GetKoala(), std::string{"Press enter to continue..."}, std::string{__FILE__}, \
        SIZE_T(__LINE__), KL_FUNCTION_NAME_STR)

/**
 * @brief Macro for waiting for a user's key press before continuing.
 *
 * @param koala The instance of Koala.
 */
#define KL_WAIT_FOR_KEYPRESS_INST(koala)                                                      \
    ::kl::OutputUtility::PrintPausedMessage(koala, std::string{"Press enter to continue..."}, \
                                            std::string{__FILE__}, SIZE_T(__LINE__),          \
                                            KL_FUNCTION_NAME_STR)

#endif  // #ifndef KL_EXCEPTION_DEFINITIONS_H