/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/KoalaException.h
 *
 * @brief Header file for the koala exception (KoalaException) class.
 */

#ifndef KL_KOALA_EXCEPTION_H
#define KL_KOALA_EXCEPTION_H 1

#include "koala/Definitions/ColourDefinitions.h"
#include "koala/Definitions/GetterSetterDefinitions.h"

#include <list>

#if defined(KOALA_DEBUG)
#include <cxxabi.h>
#include <libunwind.h>
#include <stdio.h>
#define UNW_LOCAL_ONLY
#endif  // #if defined(KOALA_DEBUG)

/**
 * @brief Macro for printing a value in hex.
 *
 * @param value The value to print.
 */
#define HEX(value) "0x" << std::hex << value << std::dec

namespace kl
{
/**
 * @brief KoalaException class.
 */
class KoalaException : public std::runtime_error
{
public:
    using StringList = std::list<std::string>;  ///< Alias for a list of strings.

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Constructor.
     *
     * @param errorMessage The error message.
     * @param functionName The function name.
     * @param fileName The file name.
     * @param lineNumber The line number.
     */
    KoalaException(std::string errorMessage, std::string functionName, std::string fileName,
                   const std::size_t lineNumber);

    /**
     * @brief Default copy constructor.
     */
    KoalaException(const KoalaException &) = default;

    /**
     * @brief Default move constructor.
     */
    KoalaException(KoalaException &&) = default;

    /**
     * @brief Default copy assignment operator.
     */
    KoalaException &operator=(const KoalaException &) = default;

    /**
     * @brief Default move assignment operator.
     */
    KoalaException &operator=(KoalaException &&) = default;

    /**
     * @brief Default destructor.
     */
    ~KoalaException() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Return a string to explain the error.
     *
     * @return A message explaining the error.
     */
    const char *what() const noexcept;

    /**
     * @brief Concatenate the current message with another.
     *
     * @param message The new message.
     */
    void Concatenate(std::string message) noexcept;

    /**
     * @brief Get the error messages.
     *
     * @return The error message strings.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, ErrorMessages, m_errorMessages, const);

    /**
     * @brief Get the function name.
     *
     * @return The function name.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, FunctionName, m_functionName, const);

    /**
     * @brief Get the file name.
     *
     * @return The file name.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, FileName, m_fileName, const);

    /**
     * @brief Get the line number.
     *
     * @return The line number.
     */
    KL_SIMPLE_GETTER(, LineNumber, m_lineNumber, const);

    /**
     * @brief Get the stack trace.
     *
     * @return The stack trace strings.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(const, StackTrace, m_stackTrace, const);

private:
    StringList m_errorMessages;         ///< The error messages.
    mutable std::string m_errorString;  ///< The error string.
    std::string m_functionName;         ///< The name of the throwing function.
    std::string m_fileName;             ///< The file name.
    std::size_t m_lineNumber;           ///< The line number.
    StringList m_stackTrace;            ///< The stack trace information.

    /**
     * @brief Write the GCC stack trace.
     *
     * @return The stack trace strings.
     */
    StringList WriteStackTrace() const;
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline KoalaException::KoalaException(std::string errorMessage, std::string functionName,
                                      std::string fileName, const std::size_t lineNumber)
    : std::runtime_error{errorMessage.c_str()},
      m_errorMessages{{std::move(errorMessage)}},
      m_errorString{},
      m_functionName{std::move(functionName)},
      m_fileName{std::move(fileName)},
      m_lineNumber{lineNumber},
      m_stackTrace{this->WriteStackTrace()}
{
}

//--------------------------------------------------------------------------------------------------

inline const char *KoalaException::what() const noexcept
{
    m_errorString = m_errorMessages.back() + KL_NORMAL_STR;
    return m_errorString.c_str();
}

//--------------------------------------------------------------------------------------------------

inline void KoalaException::Concatenate(std::string message) noexcept
{
    m_errorMessages.push_front(std::move(message));
}

//--------------------------------------------------------------------------------------------------

inline KoalaException::StringList KoalaException::WriteStackTrace() const
{
    auto stackTrace = StringList{};

#if defined(KOALA_DEBUG) && defined(USE_LIBUNWIND)
    unw_cursor_t cursor;
    unw_context_t context;

    // Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // Unwind frames one by one, going up the frame stack.
    while (unw_step(&cursor) > 0)
    {
        auto lineStringStream = std::stringstream{};
        unw_word_t offset, pc;
        unw_get_reg(&cursor, UNW_REG_IP, &pc);

        if (pc == 0ULL) break;

        lineStringStream << "[" << KL_YELLOW << HEX(pc) << KL_NORMAL << "] ";
        char sym[256UL];

        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0)
        {
            // Use the demangled name if we can.
            int status;
            char *demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);

            if (status == 0)
                lineStringStream << demangled << " (" << KL_DARK_GREY << sym << "+" << HEX(offset)
                                 << KL_NORMAL << ")";

            else
                lineStringStream << sym << "+" << HEX(offset);

            std::free(demangled);
        }

        else
            lineStringStream << "?";

        stackTrace.push_back(lineStringStream.str());
    }

#endif  // #if defined(KOALA_DEBUG) && defined(USE_LIBUNWIND)

    return stackTrace;
}
}  // namespace kl

#endif  // #ifndef KL_KOALA_EXCEPTION_H