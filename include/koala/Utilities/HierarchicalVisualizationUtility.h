/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/HierarchicalVisualizationUtility.h
 *
 * @brief Header file for the visualization utility (HierarchicalVisualizationUtility) class.
 */

#ifndef KL_HIERARCHICAL_VISUALIZATION_UTILITY_H
#define KL_HIERARCHICAL_VISUALIZATION_UTILITY_H 1

#include "koala/Definitions.h"
#include "koala/Koala/Koala.h"
#include "koala/Utilities/StringUtility.h"
#include "koala/Utilities/VisualizationUtility.h"

/**
 * @brief Definition for the graph red colour.
 */
#define KL_GRAPH_RED "#F1606D"

/**
 * @brief Definition for the graph blue colour.
 */
#define KL_GRAPH_BLUE "#53A3DA"

/**
 * @brief Definition for the graph dark green colour.
 */
#define KL_GRAPH_GREEN "#7AC994"

/**
 * @brief Definition for the graph white colour.
 */
#define KL_GRAPH_WHITE "#F7FBF1"

/**
 * @brief Definition for the graph black colour.
 */
#define KL_GRAPH_BLACK "#3A3335"

/**
 * @brief Definition for the graph grey colour.
 */
#define KL_GRAPH_GREY "#666666"

//--------------------------------------------------------------------------------------------------

namespace kl
{
/**
 * @brief HierarchicalVisualizationOptions class.
 */
class HierarchicalVisualizationOptions : public VisualizationOptions
{
public:
    /**
     * @brief Constructor.
     */
    HierarchicalVisualizationOptions() noexcept;

    bool m_recurseOverDaughters;  ///< Whether to recurse over daughters.
    bool m_recurseOverParents;    ///< Whether to recurse over parents.
    bool m_leftAlignText;         ///< Whether to left-align text inside nodes.
    std::string m_graphTitle;     ///< The graph title to use.
    bool m_displayPseudoEdges;    ///< Whether to display pseudo-edges instead of edges.
};

//--------------------------------------------------------------------------------------------------

/**
 * @brief HierarchicalVisualizationUtility class.
 */
template <typename TBASE>
class HierarchicalVisualizationUtility : public VisualizationUtility
{
private:
    using TBASE_D =
        typename std::decay_t<TBASE>::KoalaBaseType;       ///< Alias for a decayed koala base type.
    using TBASE_sPtr = std::shared_ptr<TBASE_D>;           ///< Alias for a shared pointer.
    using TBASE_sPtrSet = std::unordered_set<TBASE_sPtr>;  ///< Alias for a set of shared pointers.
    using TBASE_wPtr = std::weak_ptr<TBASE_D>;             ///< Alias for a weak pointer.
    using TBASE_wPtrSet =
        std::set<TBASE_wPtr, std::owner_less<TBASE_wPtr>>;  ///< Alias for a set of weak pointers.
    using Edge_sPtr =
        typename HierarchicalEdgeBase<TBASE_D>::sPtr;  ///< Alias for a shared pointer to an edge.
    using PseudoEdge_sPtr =
        typename HierarchicalPseudoEdgeBase<TBASE_D>::sPtr;  ///< Alias for a shared pointer to a
                                                             ///< pseudo-edge.
    using ConnectionTuple =
        std::tuple<std::uint64_t, std::uint64_t, Edge_sPtr>;  ///< Alias for connection tuple.
    using ConnectionSet = std::set<ConnectionTuple>;  ///< Alias for a set of connection tuples.

    /**
     * @brief Find out whether a given objects contains any visible objects.
     *
     * @param spSetObjects Shared pointer to the object.
     * @param setOfAllObjects The set of all objects.
     *
     * @return Whether any of the contained objects are visible in the graph.
     */
    static bool ContainsDisplayedVertex(const TBASE_sPtr &spSetObject,
                                        const TBASE_sPtrSet &setOfAllObjects) noexcept;

    /**
     * @brief Recursively expand a set by including the parents/daughters of the elements already
     * in the set.
     *
     * @param setToExpand The set to expand.
     * @param daughters Whether to expand daughters, as opposed to parents.
     */
    static void RecursivelyExpand(TBASE_sPtrSet &setToExpand, const bool daughters) noexcept;

    /**
     * @brief Recurse through the hierarchy and write dot information along the way.
     *
     * @param definitionsDotCode The dot code referring to definitions of nodes.
     * @param spSetObject Shared pointer to the current object.
     * @param connectionsDotCode The dot code referring to connections between nodes.
     * @param fontName The font name.
     * @param fontSize The font size.
     * @param leftAlignText Whether to left-align the text in the graph nodes.
     * @param setOfAllObjects The set of all objects in the graph.
     * @param completedConnections The set of completed parent-to-daughter ID connections.
     * @param colourNumber The initial colour number.
     * @param indentationLevel The indentation level of the dot code.
     * @param displayPseudoEdges Whether to display pseudo-edges instead of edges.
     */
    static void RecursivelyWriteDotInformation(
        StringVector &definitionsDotCode, const TBASE_sPtr &spSetObject,
        StringVector &connectionsDotCode, const std::string &fontName, const std::string &fontSize,
        const bool leftAlignText, const TBASE_sPtrSet &setOfAllObjects,
        ConnectionSet &completedConnections, std::size_t colourNumber,
        std::size_t &indentationLevel, const bool displayPseudoEdges) noexcept;

    /**
     * @brief Get the node and set colours from a given colour number.
     *
     * @param colourNumber The colour number to translate.
     *
     * @return The node colour and the set colour.
     */
    static std::tuple<std::string, std::string> GetSetColours(std::size_t colourNumber) noexcept;

    /**
     * @brief Recurse through the hierarchy and get the set of all objects.
     *
     * @param setOfAllObjects The set of all objects to append.
     * @param spSetObject The current object.
     */
    static void RecursivelyGetSetOfAllObjects(TBASE_sPtrSet &setOfAllObjects,
                                              const TBASE_sPtr &spSetObject) noexcept;

    /**
     * @brief Write the dot code for a given object.
     *
     * @param definitionsDotCode The dot code referring to definitions of nodes.
     * @param spSetObject Shared pointer to the current object.
     * @param fontName The font name.
     * @param fontSize The font size.
     * @param colour The colour of the node.
     * @param fontColour The colour of the font.
     * @param leftAlignText Whether to left-align the text in the graph nodes.
     * @param indentationLevel The indentation level of the dot code.
     */
    static void WriteDotCodeDefinitions(StringVector &definitionsDotCode,
                                        const TBASE_sPtr &spSetObject, const std::string &fontName,
                                        const std::string &fontSize, const std::string &colour,
                                        const std::string &fontColour, const bool leftAlignText,
                                        std::size_t &indentationLevel) noexcept;

    /**
     * @brief Write the pseudo-edges for a given object.
     *
     * @param connectionsDotCode The dot code referring to connections between nodes.
     * @param spSetObject Shared pointer to the current object.
     * @param setOfAllObjects The set of all objects in the graph.
     * @param completedConnections The set of completed parent-to-daughter ID connections.
     * @param fontName The label font name.
     * @param fontSize The label font size.
     */
    static void WritePseudoEdges(StringVector &connectionsDotCode, const TBASE_sPtr &spSetObject,
                                 const TBASE_sPtrSet &setOfAllObjects,
                                 ConnectionSet &completedConnections, const std::string &fontName,
                                 const std::string &fontSize) noexcept;

    /**
     * @brief Write the edges for a given object.
     *
     * @param connectionsDotCode The dot code referring to connections between nodes.
     * @param spSetObject Shared pointer to the current object.
     * @param setOfAllObjects The set of all objects in the graph.
     * @param completedConnections The set of completed parent-to-daughter ID connections.
     * @param fontName The label font name.
     * @param fontSize The label font size.
     */
    static void WriteEdges(StringVector &connectionsDotCode, const TBASE_sPtr &spSetObject,
                           const TBASE_sPtrSet &setOfAllObjects,
                           ConnectionSet &completedConnections, const std::string &fontName,
                           const std::string &fontSize) noexcept;

    /**
     * @brief Generate a random number between two limits.
     *
     * @param min The lower limit.
     * @param max The upper limit.
     *
     * @return The random number.
     */
    static std::size_t GenerateRandomNumber(const std::size_t min, const std::size_t max);

    /**
     * @brief Get the indentation string from the level.
     *
     * @param indentationLevel The level.
     *
     * @return The indentation string.
     */
    static std::string GetIndentation(const std::size_t indentationLevel);

    /**
     * @brief Process the object set.
     *
     * @param objectSet The object set.
     */
    static void ProcessObjectSet(TBASE_sPtrSet &objectSet);

public:
    /**
     * @brief Deleted constructor.
     */
    HierarchicalVisualizationUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    HierarchicalVisualizationUtility(const HierarchicalVisualizationUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    HierarchicalVisualizationUtility(HierarchicalVisualizationUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    HierarchicalVisualizationUtility &operator=(const HierarchicalVisualizationUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    HierarchicalVisualizationUtility &operator=(HierarchicalVisualizationUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~HierarchicalVisualizationUtility() = delete;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Draw a graph from a top-level hierarchical object.
     *
     * @param koala The instance of Koala.
     * @param object The top-level object to draw from.
     * @param options The visualization options.
     */
    template <typename TOBJECT,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value &&
                                          !std::is_same<std::decay_t<TOBJECT>, ID_t>::value>>
    static bool Visualize(const Koala &koala, const TOBJECT &object,
                          const HierarchicalVisualizationOptions &options);

    /**
     * @brief Draw a graph from the ID of a top-level hierarchical object.
     *
     * @param koala The instance of Koala.
     * @param objectId The top-level object ID to draw from.
     * @param options The visualization options.
     */
    template <typename TOBJECT,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    static bool Visualize(const Koala &koala, const ID_t objectId,
                          const HierarchicalVisualizationOptions &options);

    /**
     * @brief Draw a graph from a set of top-level hierarchical objects.
     *
     * @param koala The instance of Koala.
     * @param objectSet The top-level object set to draw from.
     * @param options The visualization options.
     */
    template <typename TOBJECT,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    static bool Visualize(const Koala &koala, const typename TOBJECT::UnorderedRefSet &objectSet,
                          const HierarchicalVisualizationOptions &options);

    /**
     * @brief Draw a graph from a set of top-level hierarchical objects.
     *
     * @param koala The instance of Koala.
     * @param objectSet The top-level object set to draw from.
     * @param options The visualization options.
     */
    template <typename TOBJECT,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    static bool Visualize(const Koala &koala, typename TOBJECT::UnorderedConstRefSet objectSet,
                          const HierarchicalVisualizationOptions &options);

    /**
     * @brief Draw a graph from a set of IDs of top-level hierarchical objects.
     *
     * @param koala The instance of Koala.
     * @param objectIdSet The top-level object ID set to draw from.
     * @param options The visualization options.
     */
    template <typename TOBJECT,
              typename = std::enable_if_t<std::is_base_of<TBASE_D, std::decay_t<TOBJECT>>::value>>
    static bool Visualize(const Koala &koala, const IdUnorderedSet &objectIdSet,
                          const HierarchicalVisualizationOptions &options);
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline HierarchicalVisualizationOptions::HierarchicalVisualizationOptions() noexcept
    : VisualizationOptions{},
      m_recurseOverDaughters{false},
      m_recurseOverParents{false},
      m_leftAlignText{false},
      m_graphTitle{},
      m_displayPseudoEdges{false}
{
}
}  // namespace kl

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

#include "koala/Utilities/HierarchicalVisualizationUtility.txx"

#endif  // #ifndef KL_HIERARCHICAL_VISUALIZATION_UTILITY_H