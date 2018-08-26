/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/FilesystemUtility.h
 *
 * @brief Header file for the filesystem utility (FilesystemUtility) class.
 */

#ifndef KL_FILESYSTEM_UTILITY_H
#define KL_FILESYSTEM_UTILITY_H 1

#include "koala/Utilities/StringUtility.h"

#include <pwd.h>
#include <wordexp.h>
#include <fstream>
#include <random>

namespace fs = std::experimental::filesystem;

namespace kl
{
/**
 * @brief FilesystemUtility class.
 */
class FilesystemUtility
{
public:
    /**
     * @brief Deleted constructor.
     */
    FilesystemUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    FilesystemUtility(const FilesystemUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    FilesystemUtility(FilesystemUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    FilesystemUtility &operator=(const FilesystemUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    FilesystemUtility &operator=(FilesystemUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~FilesystemUtility() = delete;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Get the home directory.
     *
     * @return The home directory.
     */
    static fs::path GetHomeDirectory();

    /**
     * @brief Get unique filename by appending an underscore and an integer before the first file
     * extension.
     *
     * @param initialPath The initial file path.
     * @param maxTries The maximum number of tries.
     *
     * @return A unique filename.
     */
    static fs::path GetUniquePath(const fs::path &initialPath,
                                  const std::size_t maxTries = SIZE_T(1'000'000UL));

    /**
     * @brief Read a file into a string vector.
     *
     * @param filePath The file path.
     * @param trimWhitespace Whether to trim the whitespace.
     * @param binaryMode Whether to open the file in binary mode.
     *
     * @return The string vector.
     */
    static StringVector ReadFile(const fs::path &filePath, const bool trimWhitespace = false,
                                 const bool binaryMode = false);

    /**
     * @brief Read a file into a string.
     *
     * @param filePath The file path.
     * @param trimWhitespace Whether to trim the whitespace.
     * @param binaryMode Whether to open the file in binary mode.
     *
     * @return The string.
     */
    static std::string ReadFileToString(const fs::path &filePath, const bool trimWhitespace = false,
                                        const bool binaryMode = false);

    /**
     * @brief Read a binary file into a string of bytes.
     *
     * @param filePath The file path.
     *
     * @return The string of bytes.
     */
    template <typename TBYTE = char>
    static std::basic_string<TBYTE> ReadBinaryFile(const fs::path &filePath);

    /**
     * @brief Write a string vector to file.
     *
     * @param filePath The file path.
     * @param stringVector The string vector.
     * @param append Whether to append to the file.
     */
    static void WriteToFile(const fs::path &filePath, const StringVector &stringVector,
                            const bool append = true);

    /**
     * @brief Write a string to file.
     *
     * @param filePath The file path.
     * @param line The string to write.
     * @param append Whether to append to the file.
     */
    static void WriteToFile(const fs::path &filePath, const std::string &line,
                            const bool append = true);

    /**
     * @brief Generate a fix-length hexadecimal postfix.
     *
     * @param maxPostfix The maximum postfix number.
     *
     * @return The postfix.
     */
    static std::string GenerateHexPostfix(const int maxPostfix);

    /**
     * @brief Relativize a path.
     *
     * @param filePath The file path.
     * @param directory The directory relative to which to relativize.
     *
     * @return The relativized path.
     */
    static fs::path RelativizePath(const fs::path &filePath,
                                   const fs::path &directory = fs::current_path());

    /**
     * @brief Absolutize a path.
     *
     * @param filePath The file path.
     * @param basePath The base path.
     *
     * @return The absolutized path.
     */
    static fs::path AbsolutizePath(const fs::path &filePath,
                                   const fs::path &basePath = fs::current_path());

    /**
     * @brief Canonicalize a path.
     *
     * @param filePath The file path.
     * @param basePath The base path.
     *
     * @return The canonicalized path.
     */
    static fs::path CanonicalizePath(const fs::path &filePath,
                                     fs::path basePath = fs::current_path());

    /**
     * @brief Standardize a path.
     *
     * @param filePath The file path.
     * @param relativeTo The path relative to which to standardize.
     * @param fileBasePath The base path of the file path.
     * @param relativeToBasePath The base path of the relative-to path.
     *
     * @return The standardized path.
     */
    static fs::path StandardizePath(const fs::path &filePath, const fs::path &relativeTo,
                                    const fs::path &fileBasePath,
                                    const fs::path &relativeToBasePath);

    /**
     * @brief Standardize a path.
     *
     * @param filePath The file path.
     * @param relativeTo The path relative to which to standardize.
     * @param fileBasePath The base path of the file path.
     *
     * @return The standardized path.
     */
    static fs::path StandardizePath(const fs::path &filePath,
                                    const fs::path &relativeTo = fs::current_path(),
                                    const fs::path &fileBasePath = fs::current_path());

    /**
     * @brief Standardize a path that does not exist yet.
     *
     * @param filePath The file path.
     * @param relativeTo The path relative to which to standardize.
     * @param fileBasePath The base path of the file path.
     * @param relativeToBasePath The base path of the relative-to path.
     *
     * @return The standardized path.
     */
    static fs::path StandardizeNewPath(const fs::path &filePath, const fs::path &relativeTo,
                                       const fs::path &fileBasePath,
                                       const fs::path &relativeToBasePath);

    /**
     * @brief Standardize a path that does not exist yet.
     *
     * @param filePath The file path.
     * @param relativeTo The path relative to which to standardize.
     * @param fileBasePath The base path of the file path.
     *
     * @return The standardized path.
     */
    static fs::path StandardizeNewPath(const fs::path &filePath,
                                       const fs::path &relativeTo = fs::current_path(),
                                       const fs::path &fileBasePath = fs::current_path());

    /**
     * @brief Get an environment variable.
     *
     * @param variableName The name of the variable.
     *
     * @return The value of the variable, if any.
     */
    static std::string GetEnvironmentVariable(const std::string &variableName);

    /**
     * @brief Set an environment variable.
     *
     * @param variableName The variable name.
     * @param variableValue The variable value.
     */
    static void SetEnvironmentVariable(const std::string &variableName,
                                       const std::string &variableValue);

    /**
     * @brief Find out whether an environment variable is set.
     *
     * @param variableName The name of the variable.
     *
     * @return Whether the variable is set.
     */
    static bool IsEnvironmentVariableSet(const std::string &variableName);

    /**
     * @brief Unset an environment variable.
     *
     * @param variableName The name of the variable.
     */
    static void UnsetEnvironmentVariable(const std::string &variableName);

    /**
     * @brief Get a set of environment variables.
     *
     * @param variableNames The names of the variables.
     *
     * @return A map from variable names to values.
     */
    static StringUnorderedMap GetEnvironmentVariables(const StringVector &variableNames);

    /**
     * @brief Set a number of environment variables.
     *
     * @param environmentVariables The set of environment variables.
     */
    static void SetEnvironmentVariables(const StringUnorderedMap &environmentVariables);

    /**
     * @brief Expand a string in the style of a shell.
     *
     * @param inputString The input string.
     *
     * @return The expanded string.
     */
    static std::string ExpandString(const std::string &inputString);

    /**
     * @brief Expand a path in the style of a shell.
     *
     * @param inputPath The input path.
     *
     * @return The expanded path.
     */
    static PathVector ExpandPath(const fs::path &inputPath);

    /**
     * @brief Get the current working directory.
     *
     * @return The current working directory.
     */
    static fs::path GetCurrentWorkingDirectory();

    /**
     * @brief Set the current working directory.
     *
     * @param directory The directory to set.
     */
    static void SetCurrentWorkingDirectory(const fs::path &directory);

    /**
     * @brief Find out whether a path is in a subdirectory of another path.
     *
     * @param path The path.
     * @param subPath The potential subpath of the path.
     * @param canonicalize Whether to canonicalize the paths.
     *
     * @return Whether it is in a subdirectory.
     */
    static bool IsInSubdirectory(fs::path path, fs::path subPath, const bool canonicalize = true);

private:
    /**
     * @brief Canonicalize a path (implementation method).
     *
     * @param currentPath The current file path.
     * @param pathToAppend The path to append at the end.
     *
     * @return The canonicalized path.
     */
    static fs::path CanonicalizePathImpl(const fs::path &currentPath, fs::path pathToAppend = {});
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::GetHomeDirectory()
{
    // Get the home directory, either through the environment variable or, failing that, getpwuid.
    auto pHomeDir = std::getenv("HOME");

    if (!pHomeDir) pHomeDir = getpwuid(getuid())->pw_dir;

    if (!pHomeDir) KL_THROW("Could not get the home directory");

    return {pHomeDir};
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::GetUniquePath(const fs::path &initialPath,
                                                 const std::size_t maxTries)
{
    if (!fs::exists(initialPath)) return initialPath;

    const auto parentPath = initialPath.parent_path();
    const auto bareFileName = initialPath.filename().replace_extension();
    const auto fileExtension = initialPath.extension();
    auto path = fs::path{};

    // Inrease the file number till the file doesn't exist or we exceed the max tries.
    auto fileNumber = SIZE_T(0UL);
    do
    {
        if (++fileNumber > maxTries)
            KL_THROW("Could not find a unique path from the initial path " << KL_WHITE_BOLD
                                                                           << initialPath.string());

        path = parentPath / bareFileName + fs::path{std::to_string(fileNumber)} + fileExtension;

    } while (fs::exists(path));

    return path;
}

//--------------------------------------------------------------------------------------------------

inline StringVector FilesystemUtility::ReadFile(const fs::path &filePath, const bool trimWhitespace,
                                                const bool binaryMode)
{
    if (!fs::exists(filePath) || fs::is_directory(filePath))
        KL_THROW("Could not read file at " << KL_WHITE_BOLD << filePath.string());

    auto inputFile =
        binaryMode ? std::ifstream{filePath, std::ios_base::binary} : std::ifstream{filePath};

    if (!inputFile.is_open())
        KL_THROW("Could not open the file at " << KL_WHITE_BOLD << filePath.string());

    auto lineVector = StringVector{};
    for (auto line = std::string{}; getline(inputFile, line); /* no iterator */)
    {
        if (trimWhitespace) StringUtility::TrimWhitespace(line);

        lineVector.push_back(std::move(line));
    }

    return lineVector;
}

//--------------------------------------------------------------------------------------------------

inline std::string FilesystemUtility::ReadFileToString(const fs::path &filePath,
                                                       const bool trimWhitespace,
                                                       const bool binaryMode)
{
    auto lineVector = FilesystemUtility::ReadFile(filePath, trimWhitespace, binaryMode);
    auto lineString = std::string{};

    if (!lineVector.empty())
    {
        for (auto &line : lineVector) lineString += std::move(line) + "\n";

        lineString.pop_back();
    }

    return lineString;
}

//--------------------------------------------------------------------------------------------------

template <typename TBYTE>
inline std::basic_string<TBYTE> FilesystemUtility::ReadBinaryFile(const fs::path &filePath)
{
    auto inputFile = std::ifstream{filePath, std::ios::binary};
    inputFile.unsetf(std::ios::skipws);

    auto fileSize = std::streampos{};
    inputFile.seekg(SIZE_T(0UL), std::ios::end);
    fileSize = inputFile.tellg();
    inputFile.seekg(SIZE_T(0UL), std::ios::beg);

    auto byteString = std::basic_string<TBYTE>{};
    byteString.reserve(SIZE_T(fileSize));

    byteString.insert(byteString.begin(), std::istream_iterator<TBYTE>(inputFile),
                      std::istream_iterator<TBYTE>());

    return byteString;
}

//--------------------------------------------------------------------------------------------------

inline void FilesystemUtility::WriteToFile(const fs::path &filePath,
                                           const StringVector &stringVector, const bool append)
{
    auto outputFile = std::ofstream{filePath, (append ? std::ios::app : std::ios::out)};

    if (!outputFile.is_open())
        KL_THROW("Could not open file at " << KL_WHITE_BOLD << filePath.string());

    for (const auto &line : stringVector) outputFile << line << '\n';

    outputFile.close();
}

//--------------------------------------------------------------------------------------------------

inline void FilesystemUtility::WriteToFile(const fs::path &filePath, const std::string &line,
                                           const bool append)
{
    auto outputFile = std::ofstream{filePath, (append ? std::ios::app : std::ios::out)};

    if (!outputFile.is_open())
        KL_THROW("Could not open file at " << KL_WHITE_BOLD << filePath.string());

    outputFile << line << '\n';
    outputFile.close();
}

//--------------------------------------------------------------------------------------------------

inline std::string FilesystemUtility::GenerateHexPostfix(const int maxPostfix)
{
    auto lengthStrStream = std::stringstream{};
    lengthStrStream << std::hex << maxPostfix;
    const auto length = lengthStrStream.str().length();

    // Generate the number.
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> distribution(0x0, maxPostfix);

    auto stringStream = std::stringstream{};
    stringStream << std::hex << std::setfill('0') << std::setw(static_cast<int>(length))
                 << distribution(generator);
    return stringStream.str();
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::RelativizePath(const fs::path &path, const fs::path &directory)
{
    auto parentPath = path;
    auto daughterPath = fs::path{};

    while (!fs::equivalent(parentPath, directory))
    {
        if (parentPath.empty())
        {
            KL_THROW("Failed to relativize path: " << KL_LIGHT_GREY << path.string() << KL_NORMAL
                                                   << " with respect to " << KL_LIGHT_GREY
                                                   << directory.string());
        }

        daughterPath = parentPath.filename() / daughterPath;
        parentPath = parentPath.parent_path();
    }

    return daughterPath;
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::AbsolutizePath(const fs::path &filePath,
                                                  const fs::path &basePath)
{
    if (filePath.is_absolute()) return filePath;

    return fs::absolute(filePath, basePath);
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::CanonicalizePath(const fs::path &filePath, fs::path basePath)
{
    basePath = basePath.empty() ? fs::current_path() : basePath;
    return FilesystemUtility::CanonicalizePathImpl(filePath.is_absolute() ? filePath
                                                                          : basePath / filePath);
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::StandardizePath(const fs::path &filePath,
                                                   const fs::path &relativeTo,
                                                   const fs::path &fileBasePath,
                                                   const fs::path &relativeToBasePath)
{
    const auto newFilePath = FilesystemUtility::CanonicalizePath(filePath, fileBasePath);
    const auto newRelativeTo = FilesystemUtility::CanonicalizePath(relativeTo, relativeToBasePath);

    return FilesystemUtility::RelativizePath(newFilePath, newRelativeTo);
}
//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::StandardizePath(const fs::path &filePath,
                                                   const fs::path &relativeTo,
                                                   const fs::path &basePath)
{
    return FilesystemUtility::StandardizePath(filePath, relativeTo, basePath, basePath);
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::StandardizeNewPath(const fs::path &filePath,
                                                      const fs::path &relativeTo,
                                                      const fs::path &fileBasePath,
                                                      const fs::path &relativeToBasePath)
{
    const auto newFilePath = FilesystemUtility::AbsolutizePath(filePath, fileBasePath);
    const auto newRelativeTo = FilesystemUtility::AbsolutizePath(relativeTo, relativeToBasePath);

    return FilesystemUtility::RelativizePath(newFilePath, newRelativeTo);
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::StandardizeNewPath(const fs::path &filePath,
                                                      const fs::path &relativeTo,
                                                      const fs::path &basePath)
{
    return FilesystemUtility::StandardizeNewPath(filePath, relativeTo, basePath, basePath);
}

//--------------------------------------------------------------------------------------------------

inline std::string FilesystemUtility::GetEnvironmentVariable(const std::string &variableName)
{
    if (const char *const pEnvVar = getenv(variableName.c_str())) return {pEnvVar};

    return {};
}

//--------------------------------------------------------------------------------------------------

inline void FilesystemUtility::SetEnvironmentVariable(const std::string &variableName,
                                                      const std::string &variableValue)
{
    if (variableValue.empty()) FilesystemUtility::UnsetEnvironmentVariable(variableName);

    if (setenv(variableName.c_str(), variableValue.c_str(), 1) != 0)
    {
        switch (errno)
        {
            case EINVAL:
                KL_THROW("Could not set environment variable because of invalid variable name: "
                         << variableName);
            case ENOMEM:
                KL_THROW("Could not set environment variable because ran out of memory: "
                         << variableName);
            default:
                KL_THROW("Could not set environment variable because of unknown error: "
                         << variableName);
        }
    }
}

//--------------------------------------------------------------------------------------------------

inline bool FilesystemUtility::IsEnvironmentVariableSet(const std::string &variableName)
{
    return (getenv(variableName.c_str()) != nullptr);
}

//--------------------------------------------------------------------------------------------------

inline void FilesystemUtility::UnsetEnvironmentVariable(const std::string &variableName)
{
    if (unsetenv(variableName.c_str()) != 0)
    {
        switch (errno)
        {
            case EINVAL:
                KL_THROW("Could not unset environment variable because of invalid variable name: "
                         << variableName);
            case ENOMEM:
                KL_THROW("Could not unset environment variable because ran out of memory: "
                         << variableName);
            default:
                KL_THROW("Could not unset environment variable because of unknown error: "
                         << variableName);
        }
    }
}

//--------------------------------------------------------------------------------------------------

inline StringUnorderedMap FilesystemUtility::GetEnvironmentVariables(
    const StringVector &variableNames)
{
    auto environmentVariables = StringUnorderedMap{};

    for (const auto &variableName : variableNames)
        environmentVariables.emplace(variableName,
                                     FilesystemUtility::GetEnvironmentVariable(variableName));

    return environmentVariables;
}

//--------------------------------------------------------------------------------------------------

inline void FilesystemUtility::SetEnvironmentVariables(
    const StringUnorderedMap &environmentVariables)
{
    for (const auto &envVarPair : environmentVariables)
        FilesystemUtility::SetEnvironmentVariable(envVarPair.first, envVarPair.second);
}

//--------------------------------------------------------------------------------------------------

inline std::string FilesystemUtility::ExpandString(const std::string &inputString)
{
    wordexp_t wordExp;
    switch (wordexp(inputString.c_str(), &wordExp, WRDE_UNDEF))
    {
        case 0:
            break;
        case WRDE_BADCHAR:
            KL_THROW("Could not expand string due to bad character: " << inputString);
        case WRDE_BADVAL:
            KL_THROW("Could not expand string due to undefined shell variable being referenced: "
                     << inputString);
        case WRDE_CMDSUB:
            KL_THROW(
                "Could not expand string due to illegal command substitution: " << inputString);
        case WRDE_NOSPACE:
            KL_THROW("Could not expand string because ran out of memory: " << inputString);
        case WRDE_SYNTAX:
            KL_THROW("Could not expand string because of shell syntax error: " << inputString);
        default:
            KL_THROW("Could not expand string due to unknown error: " << inputString);
    }

    auto outputString = std::string{};

    for (auto i = 0U; i < wordExp.we_wordc; i++)
    {
        if (i != 0U) outputString += " ";

        outputString += wordExp.we_wordv[i];
    }

    wordfree(&wordExp);

    return outputString;
}

//--------------------------------------------------------------------------------------------------

inline PathVector FilesystemUtility::ExpandPath(const fs::path &inputPath)
{
    auto pathVector = PathVector{};

    if (inputPath.empty()) KL_THROW("Could not expand path because input path was empty");

    for (const auto &pathString : StringUtility::SplitStringAt(
             FilesystemUtility::ExpandString(inputPath.string()), ' ', true))
        pathVector.emplace_back(pathString);

    if (pathVector.empty()) KL_THROW("Failed to expand path");

    return pathVector;
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::GetCurrentWorkingDirectory()
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) return {cwd};

    KL_THROW("Failed to get current working directory");
}

//--------------------------------------------------------------------------------------------------

inline void FilesystemUtility::SetCurrentWorkingDirectory(const fs::path &directory)
{
    switch (chdir(directory.c_str()))
    {
        case 0:
            break;
        case EACCES:
            KL_THROW("Could not change directory because permission was denied");
        case EFAULT:
            KL_THROW(
                "Could not change directory because the path points outside of accessible access "
                "space");
        case EIO:
            KL_THROW("Could not change directory because an I/O error occurred");
        case ELOOP:
            KL_THROW("Could not change directory because too many symbolic links were encountered");
        case ENAMETOOLONG:
            KL_THROW("Could not change directory because the path name was too long");
        case ENOENT:
            KL_THROW("Could not change directory because the directory does not exist");
        case ENOMEM:
            KL_THROW("Could not change directory because there was insufficient memory available");
        case ENOTDIR:
            KL_THROW("Could not change directory because the path is not a directory");
        default:
            KL_THROW("Could not change directory because of an unknown reason");
    }
}

//--------------------------------------------------------------------------------------------------

inline bool FilesystemUtility::IsInSubdirectory(fs::path path, fs::path subPath,
                                                const bool canonicalize)
{
    if (canonicalize)
    {
        path = kl::FilesystemUtility::CanonicalizePath(path);
        subPath = kl::FilesystemUtility::CanonicalizePath(subPath);
    }

    while (subPath.has_parent_path())
    {
        if (fs::equivalent(subPath, path)) return true;

        subPath = subPath.parent_path();
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

inline fs::path FilesystemUtility::CanonicalizePathImpl(const fs::path &currentPath,
                                                        fs::path pathToAppend)
{
    if (fs::exists(currentPath))
        return (pathToAppend.empty() ? fs::canonical(currentPath)
                                     : fs::canonical(currentPath) / pathToAppend);

    // Otherwise chop off the filenames until we get a match upstream, canonicalize that, then add
    // the chopped-off bits back on.
    if (!currentPath.has_parent_path() || !currentPath.has_filename())
    {
        KL_THROW("File path did not exist and had no parent path ("
                 << KL_WHITE_BOLD << currentPath << KL_NORMAL << ") when trying "
                 << "to canonicalize path " << KL_WHITE_BOLD << currentPath / pathToAppend);
    }

    if (pathToAppend.empty())
        pathToAppend = currentPath.filename();

    else
        pathToAppend = currentPath.filename() / pathToAppend;

    return CanonicalizePathImpl(currentPath.parent_path(), pathToAppend);
}
}  // namespace kl

#endif  // #ifndef KL_FILESYSTEM_UTILITY_H
