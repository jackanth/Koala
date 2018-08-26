/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/VisualizationUtility.h
 *
 * @brief Header file for the visualization utility (VisualizationUtility) class.
 */

#ifndef KL_VISUALIZATION_UTILITY_H
#define KL_VISUALIZATION_UTILITY_H 1

#include "koala/Definitions.h"
#include "koala/Koala/Koala.h"
#include "koala/Utilities/ExecUtility.h"
#include "koala/Utilities/FilesystemUtility.h"
#include "koala/Utilities/StringUtility.h"

namespace kl
{
/**
 * @brief Forward declaration of the Koala class.
 */
class Koala;

/**
 * @brief VisualizationOptions class.
 */
class VisualizationOptions
{
public:
    /**
     * @brief Constructor.
     */
    VisualizationOptions() noexcept;

    std::string m_dotLocation;  ///< Location of the dot executable.
    bool m_displayGraph;        ///< Whether to display the graph.
    bool m_saveSvgToFile;       ///< Whether to save an SVG rendering of the graph to file.
    std::string m_svgFilePath;  ///< The path to save the SVG rendering.
    bool m_saveDotToFile;       ///< Whether to save the dot code to file.
    std::string m_dotFilePath;  ///< The path to save the dot code.
};

//--------------------------------------------------------------------------------------------------

/**
 * @brief VisualizationUtility class.
 */
class VisualizationUtility
{
public:
    /**
     * @brief Deleted constructor.
     */
    VisualizationUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    VisualizationUtility(const VisualizationUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    VisualizationUtility(VisualizationUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    VisualizationUtility &operator=(const VisualizationUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    VisualizationUtility &operator=(VisualizationUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~VisualizationUtility() = delete;

protected:
    /**
     * @brief Save the .dot file, run dot and display the resulting graph.
     *
     * @param koala The instance of Koala.
     * @param rawDotCode The dot file StringVector object to save, run and display.
     * @param options The visualization options.
     */
    static void PrintDotGraph(const Koala &koala, const StringVector &rawDotCode,
                              const VisualizationOptions &options);

private:
    /**
     * @brief Get the dot file path.
     *
     * @param koala The instance of Koala.
     * @param options The visualization options.
     *
     * @return The dot file path.
     */
    static std::string GetDotFilePath(const Koala &koala, const VisualizationOptions &options);

    /**
     * @brief Display the dot graph.
     *
     * @param koala The instance of Koala.
     * @param options The visualization options.
     * @param dotFilePath The dot file path.
     */
    static void DisplayDotGraph(const Koala &koala, const VisualizationOptions &options,
                                const std::string &dotFilePath);

    /**
     * @brief Save the dot graph.
     *
     * @param koala The instance of Koala.
     * @param options The visualization options.
     * @param dotFilePath The dot file path.
     */
    static void SaveDotGraph(const Koala &koala, const VisualizationOptions &options,
                             const std::string &dotFilePath);
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline VisualizationOptions::VisualizationOptions() noexcept
    : m_dotLocation{"dot"},
      m_displayGraph{true},
      m_saveSvgToFile{false},
      m_svgFilePath{"graph.svg"},
      m_saveDotToFile{false},
      m_dotFilePath{"graph.dot"}
{
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline void VisualizationUtility::PrintDotGraph(const Koala &koala, const StringVector &rawDotCode,
                                                const VisualizationOptions &options)
{
    if (options.m_dotLocation.empty())
        KL_THROW(
            "The location of the dot executable must be provided to use the visualization utility");

    // Save the dot file.
    const auto dotFilePath = VisualizationUtility::GetDotFilePath(koala, options);
    kl::FilesystemUtility::WriteToFile(dotFilePath, rawDotCode);

    if (options.m_displayGraph) VisualizationUtility::DisplayDotGraph(koala, options, dotFilePath);

    if (options.m_saveSvgToFile) VisualizationUtility::SaveDotGraph(koala, options, dotFilePath);

    if (!options.m_saveDotToFile) std::remove(dotFilePath.c_str());
}

//--------------------------------------------------------------------------------------------------

inline std::string VisualizationUtility::GetDotFilePath(const Koala &koala,
                                                        const VisualizationOptions &options)
{
    auto dotFilePath = kl::Path{};

    if (options.m_saveDotToFile)
    {
        if (options.m_dotFilePath.empty())
        {
            dotFilePath = kl::FilesystemUtility::GetUniquePath("graph.dot");

            if (koala.DebugMode())
                KL_WARN_INST(koala, "No path specified for saving dot code, so saving to "
                                        << dotFilePath.string());
        }

        else
            dotFilePath = kl::FilesystemUtility::GetUniquePath(options.m_dotFilePath);
    }

    else
        dotFilePath =
            kl::FilesystemUtility::GetUniquePath(fs::temp_directory_path() / kl::Path{"graph.dot"})
                .string();

    if (dotFilePath.has_parent_path())
    {
        const auto parentPath = dotFilePath.parent_path();

        if (!fs::exists(parentPath)) fs::create_directories(parentPath);
    }

    return dotFilePath.string();
}

//--------------------------------------------------------------------------------------------------

inline void VisualizationUtility::DisplayDotGraph(const Koala &koala,
                                                  const VisualizationOptions &options,
                                                  const std::string &dotFilePath)
{
    const auto dotCommandLineArguments = StringVector{"-Tx11", dotFilePath};

    auto execOptions = ExecOptions{};
    execOptions.m_waitTillDone = false;
    execOptions.m_waitForKeyPress = true;
    execOptions.m_killSignal = SIGKILL;

    if (!std::get<SIZE_T(0UL)>(
            ExecUtility::Exec(koala, options.m_dotLocation, dotCommandLineArguments, execOptions)))
        KL_THROW("Failed to create child process for dot");
}

//--------------------------------------------------------------------------------------------------

inline void VisualizationUtility::SaveDotGraph(const Koala &koala,
                                               const VisualizationOptions &options,
                                               const std::string &dotFilePath)
{
    auto svgFilePath = kl::Path{};

    if (options.m_svgFilePath.empty())
    {
        svgFilePath = kl::FilesystemUtility::GetUniquePath("graph.svg");

        if (koala.DebugMode())
            KL_WARN_INST(koala, "No path specified for saving svg file, so saving to "
                                    << svgFilePath.string());
    }

    else
        svgFilePath = kl::FilesystemUtility::GetUniquePath(options.m_svgFilePath);

    if (svgFilePath.has_parent_path())
    {
        const auto parentPath = svgFilePath.parent_path();

        if (!fs::exists(parentPath)) fs::create_directories(parentPath);
    }

    const auto dotCommandLineArguments =
        StringVector{"-Tsvg:cairo", dotFilePath, "-o", svgFilePath.string()};

    auto execOptions = ExecOptions{};
    execOptions.m_waitTillDone = true;
    execOptions.m_waitForKeyPress = false;

    if (!std::get<SIZE_T(0UL)>(
            ExecUtility::Exec(koala, options.m_dotLocation, dotCommandLineArguments, execOptions)))
        KL_THROW("Failed to create child process for dot");
}
}  // namespace kl

#endif  // #ifndef KL_VISUALIZATION_UTILITY_H
