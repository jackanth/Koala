/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/ExecUtility.h
 *
 * @brief Header file for the exec utility (ExecUtility) class.
 */

#ifndef KL_EXEC_UTILITY_H
#define KL_EXEC_UTILITY_H 1

#include "koala/Definitions.h"
#include "koala/Koala/Koala.h"

#include <fcntl.h>
#include <linux/limits.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

namespace kl
{
/**
 * @brief ExecOptions class.
 */
class ExecOptions
{
public:
    /**
     * @brief Default constructor.
     */
    ExecOptions() noexcept;

    std::string m_stdinString;  ///< The stdin input to pass to the program. A stdin pipe is set up
                                ///< iff this is not empty.
    bool m_waitTillDone;        ///< Whether to wait till the program is done before returning.
    bool m_waitForKeyPress;     ///< Whether to wait for a keypress before returning (overrides
                                ///< waitTillDone).
    bool m_traceChild;          ///< Whether to tell the child to be traced.
    StringVector m_environmentVariables;  ///< The environment variables to pass to the program.
    int m_killSignal;  ///< The kill signal to use upon keypress, if we wait for a keypress.
};

//--------------------------------------------------------------------------------------------------

/**
 * @brief ExecUtility class.
 */
class ExecUtility
{
public:
    /**
     * @brief Deleted constructor.
     */
    ExecUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    ExecUtility(const ExecUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    ExecUtility(ExecUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    ExecUtility &operator=(const ExecUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    ExecUtility &operator=(ExecUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~ExecUtility() = delete;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Wrapper for the execvp/execvpe functions and associated mechanics.
     *
     * @param koala The instance of Koala.
     * @param programLocation The program to exec.
     * @param arguments The command line arguments.
     * @param options The options.
     *
     * @return A tuple of whether the program exited normally (or `true` if !waitTillDone or
     * waitForKeyPress) and the process PID.
     */
    static std::tuple<bool, std::intmax_t> Exec(const Koala &koala,
                                                const std::string &programLocation,
                                                const StringVector &arguments = {},
                                                const ExecOptions &options = {});

    /**
     * @brief Wrapper for the execvp/execvpe functions and associated mechanics.
     *
     * @param koala The instance of Koala.
     * @param stdoutStream The stream to which to send stdout.
     * @param stderrStream The stream to which to send stderr.
     * @param programLocation The program to exec.
     * @param arguments The command line arguments.
     * @param options The options.
     *
     * @return A tuple of whether the program exited normally (or `true` if !waitTillDone or
     * waitForKeyPress) and the process PID.
     */
    static std::tuple<bool, std::intmax_t> Exec(const Koala &koala, std::ostream &stdoutStream,
                                                std::ostream &stderrStream,
                                                const std::string &programLocation,
                                                const StringVector &arguments = {},
                                                const ExecOptions &options = {});

    /**
     * @brief Implementation of something resembling popen.
     *
     * @param koala The instance of Koala.
     * @param command The command to execute.
     * @param options The options.
     *
     * @return A tuple of whether the program exited normally (or `true` if !waitTillDone or
     * waitForKeyPress) and the process PID.
     */
    static std::tuple<bool, std::intmax_t> Popen(const Koala &koala, const std::string &command,
                                                 const ExecOptions &options = {});

    /**
     * @brief Implementation of something resembling popen.
     *
     * @param koala The instance of Koala.
     * @param stdoutStream The stream to which to send stdout.
     * @param stderrStream The stream to which to send stderr.
     * @param command The command to execute.
     * @param options The options.
     *
     * @return A tuple of whether the program exited normally (or `true` if !waitTillDone or
     * waitForKeyPress) and the process PID.
     */
    static std::tuple<bool, std::intmax_t> Popen(const Koala &koala, std::ostream &stdoutStream,
                                                 std::ostream &stderrStream,
                                                 const std::string &command,
                                                 const ExecOptions &options = {});

private:
    using Pipe = std::array<int, SIZE_T(2UL)>;  ///< Alias for a pipe.

    /**
     * @brief Wrapper for the execvp/execvpe functions and associated mechanics (implementation
     * method).
     *
     * @param koala The instance of Koala.
     * @param programLocation The program to exec.
     * @param arguments The command line arguments.
     * @param options The options.
     *
     * @return A tuple of the pid, the stdin pipe, the stdout pipe and the stderr pipe.
     */
    static std::tuple<int, Pipe, Pipe, Pipe> ExecImpl(const Koala &koala,
                                                      const std::string &programLocation,
                                                      const StringVector &arguments,
                                                      const ExecOptions &options);
    /**
     * @brief Populate arguments in the correct format.
     *
     * @param programLocation The program location.
     * @param args The arguments to populate.
     * @param arguments The arguments to use.
     * @param maxArguments The aximum number of allowed arguments.
     */
    static void PopulateArguments(const std::string &programLocation, char **args,
                                  const StringVector &arguments, const std::ptrdiff_t maxArguments);

    /**
     * @brief Clear arguments.
     *
     * @param args The arguments to clear.
     * @param maxArguments The aximum number of allowed arguments.
     */
    static void ClearArguments(char **args, const std::ptrdiff_t maxArguments);

    /**
     * @brief Set up the pipes.
     *
     * @param options The options.
     * @param stdinPipe The stdin pipe.
     * @param stdoutPipe The stdout pipe.
     * @param stderrPipe The stderr pipe.
     */
    static void SetUpPipes(const ExecOptions &options, Pipe &stdinPipe, Pipe &stdoutPipe,
                           Pipe &stderrPipe);

    /**
     * @brief Process the child process after the fork.
     *
     * @param stdinPipe The stdin pipe.
     * @param stdoutPipe The stdout pipe.
     * @param stderrPipe The stderr pipe.
     * @param options The options.
     * @param args The arguments to use with exec.
     * @param envVarStrings The environment variables to use with exec.
     */
    static void ProcessChild(const Pipe &stdinPipe, const Pipe &stdoutPipe, const Pipe &stderrPipe,
                             const ExecOptions &options, const char *const *const args,
                             const char *const *const envVarStrings);

    /**
     * @brief Process the parent process after the fork.
     *
     * @param stdinPipe The stdin pipe.
     * @param stdoutPipe The stdout pipe.
     * @param stderrPipe The stderr pipe.
     * @param stdoutStream The stream to which to send stdout.
     * @param stderrStream The stream to which to send stderr.
     * @param options The options.
     * @param pid The process PID.
     *
     * @return If we used waitpid, then the last status.
     */
    static std::optional<int> ProcessParent(const Pipe &stdinPipe, const Pipe &stdoutPipe,
                                            const Pipe &stderrPipe, std::ostream &stdoutStream,
                                            std::ostream &stderrStream, const ExecOptions &options,
                                            const int pid);

    /**
     * @brief Process a selected stream.
     *
     * @param pid The PID.
     * @param fd The file descriptor.
     * @param oReturnStatus The return status if applicable.
     * @param outputStream The output stream..
     *
     * @return Whether we should keep selecting.
     */
    static bool ProcessSelectedStream(const int pid, const int fd,
                                      std::optional<int> &oReturnStatus,
                                      std::ostream &outputStream);

    /**
     * @brief Clean up the pipes.
     *
     * @param stdinPipe The stdin pipe.
     * @param stdoutPipe The stdout pipe.
     * @param stderrPipe The stderr pipe.
     */
    static void CleanUpPipes(const Pipe &stdinPipe, const Pipe &stdoutPipe, const Pipe &stderrPipe);
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline ExecOptions::ExecOptions() noexcept
    : m_stdinString{},
      m_waitTillDone{true},
      m_waitForKeyPress{false},
      m_traceChild{false},
      m_environmentVariables{},
      m_killSignal{SIGTERM}
{
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline std::tuple<bool, std::intmax_t> ExecUtility::Exec(const Koala &koala,
                                                         const std::string &programLocation,
                                                         const StringVector &arguments,
                                                         const ExecOptions &options)
{
    return ExecUtility::Exec(koala, koala.GetStdout(), koala.GetStderr(), programLocation,
                             arguments, options);
}

//--------------------------------------------------------------------------------------------------

inline std::tuple<bool, std::intmax_t> ExecUtility::Exec(
    const Koala &koala, std::ostream &stdoutStream, std::ostream &stderrStream,
    const std::string &programLocation, const StringVector &arguments, const ExecOptions &options)
{
    const auto [pid, stdinPipe, stdoutPipe, stderrPipe] =
        ExecImpl(koala, programLocation, arguments, options);
    const auto thePid = pid;

    const auto oStatus = ExecUtility::ProcessParent(stdinPipe, stdoutPipe, stderrPipe, stdoutStream,
                                                    stderrStream, options, pid);

    // Wait for program to finish or for keypress if necessary.
    if (!options.m_waitTillDone && !options.m_waitForKeyPress)
        return std::make_tuple(true, static_cast<std::intmax_t>(pid));

    if (options.m_waitForKeyPress)
    {
        KL_WAIT_FOR_KEYPRESS_INST(koala);

        kill(pid, options.m_killSignal);
        KL_IF_DEBUG_MESSAGE_INST(koala, "Sent kill signal to process with pid "
                                            << thePid << " using signal " << options.m_killSignal
                                            << " (\"" << strsignal(options.m_killSignal) << "\")");

        return std::make_tuple(true, static_cast<std::intmax_t>(pid));
    }

    // Get the status.
    auto status = 0;

    if (waitpid(pid, &status, WUNTRACED) == -1)
    {
        if (oStatus)
            status = *oStatus;

        else
            KL_THROW("Call to waitpid failed for process with pid "
                     << KL_WHITE_BOLD << thePid << KL_NORMAL << ": " << strerror(errno));
    }

    if (WIFEXITED(status))
    {
        const auto exitStatus = WEXITSTATUS(status);

        if (exitStatus != 0)
        {
            KL_IF_DEBUG_MESSAGE_INST(
                koala, "Process with pid " << thePid << " exited with status " << exitStatus);
            return std::make_tuple(
                false,
                static_cast<std::intmax_t>(pid));  // exited normally with status != 0
        }
    }

    else
    {
        KL_IF_DEBUG_MESSAGE_INST(koala, "Process with pid " << thePid << " exited abnormally");
        return std::make_tuple(false, static_cast<std::intmax_t>(pid));  // exited abnormally
    }

    KL_IF_DEBUG_MESSAGE_INST(koala, "Process with pid " << thePid << " exited normally");
    return std::make_tuple(true, static_cast<std::intmax_t>(pid));  // exited normally
}

//--------------------------------------------------------------------------------------------------

inline std::tuple<bool, std::intmax_t> ExecUtility::Popen(const Koala &koala,
                                                          const std::string &command,
                                                          const ExecOptions &options)
{
    return ExecUtility::Popen(koala, koala.GetStdout(), koala.GetStderr(), command, options);
}

//--------------------------------------------------------------------------------------------------

inline std::tuple<bool, std::intmax_t> ExecUtility::Popen(const Koala &koala,
                                                          std::ostream &stdoutStream,
                                                          std::ostream &stderrStream,
                                                          const std::string &command,
                                                          const ExecOptions &options)
{
    return ExecUtility::Exec(koala, stdoutStream, stderrStream, "sh", {"-e", "-c", command},
                             options);
}

//--------------------------------------------------------------------------------------------------

inline void ExecUtility::PopulateArguments(const std::string &programLocation, char **args,
                                           const StringVector &arguments,
                                           const std::ptrdiff_t maxArguments)
{
    if (arguments.size() > static_cast<decltype(arguments.size())>(maxArguments))
        KL_THROW("Could not exec" << KL_WHITE_BOLD << programLocation << KL_NORMAL
                                  << ": too many command line arguments");

    auto maxSize = 0UL;  // make sure we're always appending to the array

    for (const auto &flag : arguments)
    {
        if (!flag.empty())
        {
            args[maxSize] = new char[strlen(flag.c_str()) + SIZE_T(1UL)];
            strcpy(args[maxSize++], flag.c_str());
        }
    }

    args[maxSize] = NULL;  // command is NULL-terminated
}

//--------------------------------------------------------------------------------------------------

inline void ExecUtility::ClearArguments(char **args, const std::ptrdiff_t maxArguments)
{
    for (auto i = static_cast<std::ptrdiff_t>(0L); i < maxArguments; ++i)
    {
        char *pArgument = args[i];

        if (!pArgument)  // args are NULL-terminated
            break;

        delete[] pArgument;
    }
}

//--------------------------------------------------------------------------------------------------

inline std::tuple<int, ExecUtility::Pipe, ExecUtility::Pipe, ExecUtility::Pipe>
ExecUtility::ExecImpl(const Koala &koala, const std::string &programLocation,
                      const StringVector &arguments, const ExecOptions &options)
{
    // Display some info if in debug mode.
    if (koala.DebugMode())
    {
        auto argumentString = std::string{};

        for (const auto &arg : arguments) argumentString += " " + arg;

        KL_IF_DEBUG_MESSAGE_INST(koala, "Exec command: " << programLocation << argumentString);
    }

    // Get the list of exec arguments in the correct format.
    constexpr auto maxArguments = static_cast<std::ptrdiff_t>(100'000UL);

    auto execArguments =
        StringVector{programLocation};  // first part of exec command is the executable location.
    execArguments.insert(execArguments.end(), arguments.begin(), arguments.end());

    char *args[static_cast<std::ptrdiff_t>(maxArguments + 1UL)];
    ExecUtility::PopulateArguments(programLocation, args, execArguments, maxArguments);

    // Get the list of environment variables in the correct format.
    char *envVarStrings[static_cast<std::ptrdiff_t>(maxArguments + 1UL)];
    ExecUtility::PopulateArguments(programLocation, envVarStrings, options.m_environmentVariables,
                                   maxArguments);

    // Prepare the pipes and strings.
    auto stdinPipe = Pipe{{0, 0}};
    auto stdoutPipe = Pipe{{0, 0}};
    auto stderrPipe = Pipe{{0, 0}};
    ExecUtility::SetUpPipes(options, stdinPipe, stdoutPipe, stderrPipe);

    // In case some other process has mucked around with the signal blocking, pass empty mask here./
    // ATTN Do we need to handle SIGCHLD specially?
    auto emptyMask = sigset_t{};
    auto originalMask = sigset_t{};

    if (sigemptyset(&emptyMask) != 0) KL_THROW("Failed to initialize empty signal set");

    if (sigprocmask(SIG_SETMASK, &emptyMask, &originalMask) != 0)
        KL_THROW("Call to sigprocmask failed: " << strerror(errno));

    const auto pid = fork();

    if (pid == 0)  // child process
    {
        ExecUtility::ProcessChild(stdinPipe, stdoutPipe, stderrPipe, options,
                                  const_cast<char **>(args), const_cast<char **>(envVarStrings));
    }

    else if (pid > 0)  // parent process
    {
        KL_IF_DEBUG_MESSAGE_INST(koala, "Spawned process with pid " << pid);

        ExecUtility::ClearArguments(args, maxArguments);
        ExecUtility::ClearArguments(envVarStrings, maxArguments);

        if (sigprocmask(SIG_SETMASK, &originalMask, nullptr) != 0)
            KL_THROW("Call to sigprocmask failed: " << strerror(errno));

        return {pid, stdinPipe, stdoutPipe, stderrPipe};
    }

    // Failed to fork
    ExecUtility::CleanUpPipes(stdinPipe, stdoutPipe, stderrPipe);
    ExecUtility::ClearArguments(args, maxArguments);
    ExecUtility::ClearArguments(envVarStrings, maxArguments);

    KL_THROW("Exec utility failed to fork");
}

//--------------------------------------------------------------------------------------------------

inline void ExecUtility::SetUpPipes(const ExecOptions &options, Pipe &stdinPipe, Pipe &stdoutPipe,
                                    Pipe &stderrPipe)
{
    if (strlen(options.m_stdinString.c_str()) >= PIPE_BUF)
    {
        KL_THROW("Could not exec because the stdin string was over the pipe buffer size limit ("
                 << KL_WHITE_BOLD << strlen(options.m_stdinString.c_str()) << KL_NORMAL << "/"
                 << PIPE_BUF << " bytes)");
    }

    int stdinPipeTmp[2UL], stdoutPipeTmp[2UL], stderrPipeTmp[2UL];

    // Open the pipes.
    if (pipe(stdinPipeTmp) != 0) KL_THROW("Exec utility failed to open stdin pipe");

    if (pipe(stdoutPipeTmp) != 0)
    {
        close(stdinPipeTmp[0UL]);
        close(stdinPipeTmp[1UL]);

        KL_THROW("Exec utility failed to open stdout pipe");
    }

    if (pipe(stderrPipeTmp) != 0)
    {
        close(stdinPipeTmp[0UL]);
        close(stdinPipeTmp[1UL]);

        close(stdoutPipeTmp[0UL]);
        close(stdoutPipeTmp[1UL]);

        KL_THROW("Exec utility failed to open stderr pipe");
    }

    // Write the stdin string to stdin pipe.
    if (write(stdinPipeTmp[1UL], options.m_stdinString.c_str(),
              strlen(options.m_stdinString.c_str())) !=
        static_cast<std::uint32_t>(strlen(options.m_stdinString.c_str())))
    {
        KL_THROW("Exec utility failed to write stdin to stdin pipe");
    }

    // Put the fds into the pipe array objects.
    stdinPipe[0UL] = stdinPipeTmp[0UL];
    stdinPipe[1UL] = stdinPipeTmp[1UL];

    stdoutPipe[0UL] = stdoutPipeTmp[0UL];
    stdoutPipe[1UL] = stdoutPipeTmp[1UL];

    stderrPipe[0UL] = stderrPipeTmp[0UL];
    stderrPipe[1UL] = stderrPipeTmp[1UL];
}

//--------------------------------------------------------------------------------------------------

inline void ExecUtility::ProcessChild(const Pipe &stdinPipe, const Pipe &stdoutPipe,
                                      const Pipe &stderrPipe, const ExecOptions &options,
                                      const char *const *const args,
                                      const char *const *const envVarStrings)
{
    try
    {
        close(stdinPipe[1UL]);

        if (!options.m_stdinString
                 .empty())  // only set up the stdin pipe if we have something to pass in
        {
            if (dup2(stdinPipe[0UL], STDIN_FILENO) == -1)
                KL_THROW("Exec utility failed to copy stdin to child process");
        }

        close(stdinPipe[0UL]);
        close(stdoutPipe[0UL]);

        if (dup2(stdoutPipe[1UL], STDOUT_FILENO) == -1)
            KL_THROW("Exec utility failed to copy stdout to child process");

        close(stdoutPipe[1UL]);
        close(stderrPipe[0UL]);

        if (dup2(stderrPipe[1UL], STDERR_FILENO) == -1)
            KL_THROW("Exec utility failed to copy sterr to child process");

        close(stderrPipe[1UL]);

        const char *programLocationCStr = args[0];

        if (options.m_traceChild) ptrace(PTRACE_TRACEME, nullptr, nullptr, nullptr);

        if (!options.m_environmentVariables.empty())
            execvpe(programLocationCStr, const_cast<char **>(args),
                    const_cast<char **>(envVarStrings));

        else
            execvp(programLocationCStr, const_cast<char **>(args));

        KL_THROW("Child process failed to exec");  // note that exec should not return
    }

    catch (...)
    {
        exit(1);
    }
}

//--------------------------------------------------------------------------------------------------

inline std::optional<int> ExecUtility::ProcessParent(const Pipe &stdinPipe, const Pipe &stdoutPipe,
                                                     const Pipe &stderrPipe,
                                                     std::ostream &stdoutStream,
                                                     std::ostream &stderrStream,
                                                     const ExecOptions &options, const int pid)
{
    close(stdinPipe[0UL]);
    close(stdinPipe[1UL]);

    close(stdoutPipe[1UL]);
    close(stderrPipe[1UL]);

    auto oReturnStatus = std::optional<int>{};

    if (options.m_waitTillDone)
    {
        while (true)
        {
            fd_set fds;
            int maxfd;

            FD_ZERO(&fds);
            FD_SET(stdoutPipe[0UL], &fds);
            FD_SET(stderrPipe[0UL], &fds);

            maxfd = stdoutPipe[0UL] > stderrPipe[0UL] ? stdoutPipe[0UL] : stderrPipe[0UL];

            const auto numChanged = select(maxfd + 1, &fds, NULL, NULL, NULL);

            if (numChanged > 0L)
            {
                if (FD_ISSET(stdoutPipe[0UL], &fds))
                {
                    if (!ExecUtility::ProcessSelectedStream(pid, stdoutPipe[0UL], oReturnStatus,
                                                            stdoutStream))
                        break;

                    stdoutStream.flush();
                    fflush(stdout);
                }

                if (FD_ISSET(stderrPipe[0UL], &fds))
                {
                    if (!ExecUtility::ProcessSelectedStream(pid, stderrPipe[0UL], oReturnStatus,
                                                            stderrStream))
                        break;

                    stderrStream.flush();
                    fflush(stderr);
                }
            }

            else if ((numChanged < 0) && (errno != EINTR))
                KL_THROW("Error reading from stdout/stderr pipes: " << strerror(errno));
        }
    }

    close(stdoutPipe[0UL]);
    close(stderrPipe[0UL]);

    return oReturnStatus;
}

//--------------------------------------------------------------------------------------------------

inline bool ExecUtility::ProcessSelectedStream(const int pid, const int fd,
                                               std::optional<int> &oReturnStatus,
                                               std::ostream &outputStream)
{
    char buffer[1024UL];
    const auto nBytes = read(fd, buffer, sizeof(buffer));

    if (nBytes > 0L)
        outputStream.write(buffer, nBytes);

    else if (nBytes == 0L)  // this is our stop signal
    {
        auto status = 0;

        if (waitpid(pid, &status, WNOHANG | WUNTRACED) == -1)
        {
            KL_THROW("Call to waitpid failed for process with pid "
                     << KL_WHITE_BOLD << pid << KL_NORMAL << ": " << strerror(errno));
        }

        if (WIFEXITED(status) || WIFSIGNALED(status) || WIFSTOPPED(status)) oReturnStatus = status;

        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

inline void ExecUtility::CleanUpPipes(const Pipe &stdinPipe, const Pipe &stdoutPipe,
                                      const Pipe &stderrPipe)
{
    close(stdinPipe[0UL]);
    close(stdinPipe[1UL]);

    close(stdoutPipe[0UL]);
    close(stdoutPipe[1UL]);

    close(stderrPipe[0UL]);
    close(stderrPipe[1UL]);
}
}  // namespace kl

#endif  // #ifndef KL_EXEC_UTILITY_H