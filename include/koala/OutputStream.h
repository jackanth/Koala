/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/OutputStream.h
 *
 * @brief Header file for the output stream (OutputStream) class and the output buffer
 * (OutputBuffer) class.
 */

#ifndef KL_OUTPUT_STREAM_H
#define KL_OUTPUT_STREAM_H 1

#include "koala/Definitions.h"

#include <ctime>
#include <iomanip>

namespace kl
{
/**
 * @brief OutputBuffer class.
 */
class OutputBuffer : public std::streambuf
{
public:
    /**
     * @brief Constructor.
     *
     * @param ostreamTerminal The ostream object for printing to the terminal.
     * @param ostreamLog The ostream object for printing to the log.
     * @param newlineCallback The callback upon a newline character in the overflow.
     */
    OutputBuffer(std::ostream &ostreamTerminal, std::ostream &ostreamLog,
                 std::function<void(std::streambuf *const)> newlineCallback =
                     [](std::streambuf *const) {}) noexcept;

    /**
     * @brief Constructor.
     *
     * @param ostreamTerminal The ostream object for printing to the terminal.
     * @param newlineCallback The callback upon a newline character in the overflow.
     */
    OutputBuffer(std::ostream &ostreamTerminal,
                 std::function<void(std::streambuf *const)> newlineCallback =
                     [](std::streambuf *const) {}) noexcept;

    /**
     * @brief Deleted copy constructor.
     */
    OutputBuffer(const OutputBuffer &) = delete;

    /**
     * @brief Default move constructor.
     */
    OutputBuffer(OutputBuffer &&) = default;

    /**
     * @brief Deleted copy assignment operator.
     */
    OutputBuffer &operator=(const OutputBuffer &) = delete;

    /**
     * @brief Default move assignment operator.
     */
    OutputBuffer &operator=(OutputBuffer &&) = default;

    /**
     * @brief Default destructor.
     */
    ~OutputBuffer() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the terminal streambuf.
     *
     * @return The terminal streambuf.
     */
    KL_SIMPLE_GETTER(, GetTerminalStreamBuf, m_pStreamBufTerminal, const);

    /**
     * @brief Get the log streambuf.
     *
     * @return The log streambuf.
     */
    KL_SIMPLE_GETTER(, GetLogStreamBuf, m_pStreamBufLog, const);

private:
    /**
     * @brief Put character on overflow.
     *
     * @param c The character.
     *
     * @return Success.
     */
    virtual int overflow(int c);

    /**
     * @brief Synchronize the stream buffer.
     *
     * @return Success.
     */
    virtual int sync();

    /**
     * @brief Get a timestamp string.
     *
     * @return The timestamp string.
     */
    std::string GetTimestamp() const;

    std::streambuf *m_pStreamBufTerminal;  ///< Pointer to the terminal streambuf.
    std::streambuf *m_pStreamBufLog;       ///< Pointer to the log streambuf.
    std::function<void(std::streambuf *const)>
        m_newlineCallback;  ///< The callback upon a newline character in the overflow.
    bool m_inEscape;        ///< Whether we are currently in an escape sequence.
    bool m_wasNewLine;      ///< Whether the last 'visible' character, including \n, was \n.
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * @brief OutputStream class.
 */
class OutputStream : public std::ostream
{
public:
    using sPtr = std::shared_ptr<OutputStream>;  ///< Alias for a shared pointer to this object.

    /**
     * @brief Constructor.
     *
     * @param ostreamTerminal The ostream object for printing to the terminal.
     * @param ostreamLog The ostream object for printing to the log.
     * @param newlineCallback The callback upon a newline character in the overflow.
     */
    OutputStream(std::ostream &ostreamTerminal, std::ostream &ostreamLog,
                 std::function<void(std::streambuf *const)> newlineCallback =
                     [](std::streambuf *const) {});

    /**
     * @brief Constructor.
     *
     * @param ostreamTerminal The ostream object for printing to the terminal.
     * @param newlineCallback The callback upon a newline character in the overflow.
     */
    OutputStream(std::ostream &ostreamTerminal,
                 std::function<void(std::streambuf *const)> newlineCallback =
                     [](std::streambuf *const) {});

    /**
     * @brief Deleted copy constructor.
     */
    OutputStream(const OutputStream &) = delete;

    /**
     * @brief Default move constructor.
     */
    OutputStream(OutputStream &&) = default;

    /**
     * @brief Deleted copy assignment operator.
     */
    OutputStream &operator=(const OutputStream &) = delete;

    /**
     * @brief Default move assignment operator.
     */
    OutputStream &operator=(OutputStream &&) = default;

    /**
     * @brief Default destructor.
     */
    ~OutputStream() = default;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the output buffer.
     *
     * @return The output buffer.
     */
    KL_SIMPLE_GETTER_LVAL_RVAL(, GetBuffer, m_outputBuffer, );

private:
    OutputBuffer m_outputBuffer;  ///< The output buffer object.
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline OutputBuffer::OutputBuffer(
    std::ostream &ostreamTerminal, std::ostream &ostreamLog,
    std::function<void(std::streambuf *const)> newlineCallback) noexcept
    : std::streambuf{},
      m_pStreamBufTerminal{ostreamTerminal.rdbuf()},
      m_pStreamBufLog{ostreamLog.rdbuf()},
      m_newlineCallback{std::move_if_noexcept(newlineCallback)},
      m_inEscape{false},
      m_wasNewLine{true}
{
}

//--------------------------------------------------------------------------------------------------

inline OutputBuffer::OutputBuffer(
    std::ostream &ostreamTerminal,
    std::function<void(std::streambuf *const)> newlineCallback) noexcept
    : std::streambuf{},
      m_pStreamBufTerminal{ostreamTerminal.rdbuf()},
      m_pStreamBufLog{nullptr},
      m_newlineCallback{std::move_if_noexcept(newlineCallback)},
      m_inEscape{false},
      m_wasNewLine{true}
{
}

//--------------------------------------------------------------------------------------------------

inline int OutputBuffer::overflow(int c)
{
    if (c == EOF)
        return !EOF;

    else
    {
        auto resultLog = 0;

        if (m_pStreamBufLog)
        {
            if (!m_inEscape)
            {
                if (m_wasNewLine && (c > 31))  // if visible
                {
                    m_wasNewLine = false;

                    const auto timeStamp = this->GetTimestamp();
                    m_pStreamBufLog->sputn(timeStamp.c_str(),
                                           static_cast<long>(timeStamp.length()));
                }

                if (c == static_cast<int>('\033'))
                    m_inEscape = true;

                else if (c != static_cast<int>('\r'))
                    resultLog = m_pStreamBufLog->sputc(static_cast<char>(c));
            }

            if (m_inEscape && ((c == static_cast<int>('m')) || (c == static_cast<int>('A')) ||
                               (c == static_cast<int>('B')) || (c == static_cast<int>('C')) ||
                               (c == static_cast<int>('D')) || (c == static_cast<int>('K'))))
            {
                m_inEscape = false;
            }
        }

        if (c == static_cast<int>('\n'))
            m_pStreamBufTerminal->sputn(KL_CLEAR, sizeof(KL_CLEAR) - 1UL);

        const auto resultTerminal = m_pStreamBufTerminal->sputc(static_cast<char>(c));

        if (c == static_cast<int>('\n'))
        {
            m_newlineCallback(m_pStreamBufTerminal);
            m_wasNewLine = true;
        }

        return ((resultTerminal == EOF) || (resultLog == EOF)) ? EOF : c;
    }
}

//--------------------------------------------------------------------------------------------------

inline int OutputBuffer::sync()
{
    const auto resultTerminal = m_pStreamBufTerminal->pubsync();
    const auto resultLog = (m_pStreamBufLog && !m_inEscape) ? m_pStreamBufLog->pubsync() : 0;

    return ((resultTerminal == 0) && (resultLog == 0)) ? 0 : -1;
}

//--------------------------------------------------------------------------------------------------

inline std::string OutputBuffer::GetTimestamp() const
{
    const auto timeObj = std::time(nullptr);
    const auto tm = *std::localtime(&timeObj);

    auto stringStream = std::ostringstream{};
    stringStream << "[ " << std::put_time(&tm, "%a %d-%m-%Y %H:%M:%S") << " ] ";
    return stringStream.str();
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline OutputStream::OutputStream(std::ostream &ostreamTerminal, std::ostream &ostreamLog,
                                  std::function<void(std::streambuf *const)> newlineCallback)
    : std::ostream{&m_outputBuffer},
      m_outputBuffer{ostreamTerminal, ostreamLog, std::move(newlineCallback)}
{
}

//--------------------------------------------------------------------------------------------------

inline OutputStream::OutputStream(std::ostream &ostreamTerminal,
                                  std::function<void(std::streambuf *const)> newlineCallback)
    : std::ostream{&m_outputBuffer}, m_outputBuffer{ostreamTerminal, std::move(newlineCallback)}
{
}
}  // namespace kl

#endif  // #ifndef KL_OUTPUT_STREAM_H