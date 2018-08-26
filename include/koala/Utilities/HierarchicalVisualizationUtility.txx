/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/HierarchicalVisualizationUtility.h
 *
 * @brief Template implementation header file for the visualization utility
 * (HierarchicalVisualizationUtility) class template.
 */

#ifndef KL_HIERARCHICAL_VISUALIZATION_UTILITY_IMPL_H
#define KL_HIERARCHICAL_VISUALIZATION_UTILITY_IMPL_H 1

namespace kl
{
template <typename TBASE>
template <typename TOBJECT, typename>
inline bool HierarchicalVisualizationUtility<TBASE>::Visualize(
    const Koala &koala, const TOBJECT &object, const HierarchicalVisualizationOptions &options)
{
    typename TOBJECT::UnorderedConstRefSet objectSet = {object};
    return Visualize<TOBJECT>(koala, objectSet, options);
}
//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT, typename>
bool HierarchicalVisualizationUtility<TBASE>::Visualize(
    const Koala &koala, const typename TOBJECT::UnorderedRefSet &objectSet,
    const HierarchicalVisualizationOptions &options)
{
    return Visualize<TOBJECT>(koala, ToConstRef<TOBJECT>(objectSet), options);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT, typename>
bool HierarchicalVisualizationUtility<TBASE>::Visualize(
    const Koala &koala, typename TOBJECT::UnorderedConstRefSet objectSet,
    const HierarchicalVisualizationOptions &options)
{
    const auto lock = ReadLock{objectSet.begin()->get().GetRegistry().Mutex()};

    using namespace std::string_literals;

    // Set the font properties.
    const auto fontSize = "10"s;
    const auto titleFontSize = "12"s;
    const auto titleFontName = "Lato Semibold"s;
    const auto fontName = "Monospace"s;

    auto rawDotCode = StringVector{};
    auto definitionsDotCode = StringVector{};
    auto connectionsDotCode = StringVector{};
    auto indentationLevel = SIZE_T(0UL);

    // Write the string for defining the graph.
    rawDotCode.push_back(GetIndentation(indentationLevel) + "digraph"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "{"s);
    ++indentationLevel;
    rawDotCode.push_back(GetIndentation(indentationLevel) + "newrank=true;"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "compound=true;"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "labelloc=t;"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "label=\""s + options.m_graphTitle +
                         "\";"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "bgcolor=\""s +
                         std::string{KL_GRAPH_BLACK} + "\";"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "fontcolor=\""s +
                         std::string{KL_GRAPH_WHITE} + "\";");
    rawDotCode.push_back(GetIndentation(indentationLevel) + "dpi=100;"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "fontname=\""s + titleFontName +
                         "\";"s);
    rawDotCode.push_back(GetIndentation(indentationLevel) + "fontsize="s + titleFontSize + ";"s);

    // From the initial object set, get the set of pointers to the base type, expanding over
    // daughters and parents if required.
    auto expandedSet = TBASE_sPtrSet{};

    for (const auto &wObject : objectSet)
        expandedSet.insert(
            static_cast<TBASE_sPtr>(wObject.get().GetRegistry().GetSharedPointer(wObject.get())));

    if (options.m_recurseOverDaughters) RecursivelyExpand(expandedSet, true);

    if (options.m_recurseOverParents) RecursivelyExpand(expandedSet, false);

    ProcessObjectSet(expandedSet);

    if (objectSet.empty()) KL_THROW("Could not visualize because object set was empty");

    // Now make the set of all objects.
    auto setOfAllObjects = TBASE_sPtrSet{};
    for (const auto &spSetObject : expandedSet)
        RecursivelyGetSetOfAllObjects(setOfAllObjects, spSetObject);

    // Recurse through the hierarchy and append to the dot code.
    const auto initialColourNumber = GenerateRandomNumber(SIZE_T(0UL), SIZE_T(2UL));

    auto completedConnections = ConnectionSet{};
    auto colourIncrement = SIZE_T(1UL);

    for (const auto &spSetObject : expandedSet)
    {
        auto colourNumber = initialColourNumber;

        if (ContainsDisplayedVertex(spSetObject, setOfAllObjects))
            colourNumber = initialColourNumber + colourIncrement++;

        RecursivelyWriteDotInformation(rawDotCode, spSetObject, connectionsDotCode, fontName,
                                       fontSize, options.m_leftAlignText, setOfAllObjects,
                                       completedConnections, colourNumber, indentationLevel,
                                       options.m_displayPseudoEdges);
    }

    // Add the definitions and connections to the dot code.
    rawDotCode.insert(rawDotCode.cend(), std::make_move_iterator(definitionsDotCode.begin()),
                      std::make_move_iterator(definitionsDotCode.end()));

    rawDotCode.insert(rawDotCode.cend(), std::make_move_iterator(connectionsDotCode.begin()),
                      std::make_move_iterator(connectionsDotCode.end()));

    if (indentationLevel > SIZE_T(0UL)) --indentationLevel;

    rawDotCode.push_back(GetIndentation(indentationLevel) + "}");

    VisualizationUtility::PrintDotGraph(koala, rawDotCode, options);
    return true;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT, typename>
inline bool HierarchicalVisualizationUtility<TBASE>::Visualize(
    const Koala &koala, const ID_t objectId, const HierarchicalVisualizationOptions &options)
{
    typename TOBJECT::UnorderedConstRefSet objectSet = {
        koala.FetchRegistry<TOBJECT>().template Get<TOBJECT>(objectId)};
    return Visualize<TOBJECT>(koala, objectSet, options);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
template <typename TOBJECT, typename>
inline bool HierarchicalVisualizationUtility<TBASE>::Visualize(
    const Koala &koala, const IdUnorderedSet &objectIdSet,
    const HierarchicalVisualizationOptions &options)
{
    // Populate a set of objects.
    auto objectSet = typename TOBJECT::UnorderedConstRefSet{};

    for (auto objectId : objectIdSet)
        objectSet.insert(koala.FetchRegistry<TOBJECT>().template Get<TOBJECT>(objectId));

    return Visualize<TOBJECT>(koala, objectSet, options);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline bool HierarchicalVisualizationUtility<TBASE>::ContainsDisplayedVertex(
    const TBASE_sPtr &spSetObject, const TBASE_sPtrSet &setOfAllObjects) noexcept
{
    for (const auto &wpContained : spSetObject->ContainedWeakPointers())
    {
        if (const auto spContained = wpContained.lock())
        {
            if (setOfAllObjects.find(spContained) != setOfAllObjects.end()) return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
void HierarchicalVisualizationUtility<TBASE>::RecursivelyExpand(TBASE_sPtrSet &setToExpand,
                                                                const bool daughters) noexcept
{
    // Recurse over the relative and add any new ones to the set.
    auto changeMade = false;
    do
    {
        changeMade = false;
        for (const auto &spObject : setToExpand)
        {
            for (const auto &wpEdge : (daughters ? spObject->DaughterEdgeWeakPointers()
                                                 : spObject->ParentEdgeWeakPointers()))
            {
                if (const auto spEdge = wpEdge.lock())
                {
                    if (const auto spRelative = spEdge->ObjectWeakPointer().lock())
                        changeMade |= setToExpand.insert(spRelative).second;
                }
            }
        }

    } while (changeMade);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
void HierarchicalVisualizationUtility<TBASE>::RecursivelyWriteDotInformation(
    StringVector &definitionsDotCode, const TBASE_sPtr &spSetObject,
    StringVector &connectionsDotCode, const std::string &fontName, const std::string &fontSize,
    const bool leftAlignText, const TBASE_sPtrSet &setOfAllObjects,
    ConnectionSet &completedConnections, std::size_t colourNumber, std::size_t &indentationLevel,
    const bool displayPseudoEdges) noexcept
{
    using namespace std::string_literals;

    // If there are no contained objects, we print the node.
    if (!ContainsDisplayedVertex(spSetObject, setOfAllObjects))
    {
        const auto [colour, fontColour] = GetSetColours(colourNumber);

        WriteDotCodeDefinitions(definitionsDotCode, spSetObject, fontName, fontSize, colour,
                                fontColour, leftAlignText, indentationLevel);

        if (displayPseudoEdges)
            WritePseudoEdges(connectionsDotCode, spSetObject, setOfAllObjects, completedConnections,
                             fontName, fontSize);

        else
            WriteEdges(connectionsDotCode, spSetObject, setOfAllObjects, completedConnections,
                       fontName, fontSize);
    }

    // If there are contained objects, we print a subgraph and recurse through it.
    else
    {
        auto nodeLabel = spSetObject->GetGraphNodeLabel();

        if (leftAlignText)
        {
            // In dot, having \l at the end of a line instead of \n indicates that the current line
            // is to be left-aligned.
            StringUtility::ReplaceInString(nodeLabel, "\n"s, "\\l"s);
            nodeLabel += "\\l"s;
        }

        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "subgraph cluster"s +
                                     std::to_string(spSetObject->ID()));
        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "{"s);

        ++indentationLevel;

        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "label=\""s + nodeLabel +
                                     "\";"s);
        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "fontcolor=\""s +
                                     std::string{KL_GRAPH_WHITE} + "\";"s);
        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "fontname=\""s + fontName +
                                     "\";"s);
        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "labeljust=l;"s);
        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "fontsize="s + fontSize +
                                     ";"s);
        definitionsDotCode.push_back(
            GetIndentation(indentationLevel) + "color=\""s +
            std::get<SIZE_T(0UL)>(GetSetColours(colourNumber + SIZE_T(1UL))) + "\";"s);
        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "node"s +
                                     std::to_string(spSetObject->ID()) +
                                     "[shape=point, style=invis];"s);

        // If the contained object is a node, it should be coloured the same as the subgraph. If it
        // is its own subgraph, we should increment the colour number.
        auto colourIncrement = SIZE_T(1UL);

        for (const auto &wpContainedObject : spSetObject->ContainedWeakPointers())
        {
            if (const auto spContainedObject = wpContainedObject.lock())
            {
                auto containedContained = TBASE_sPtrSet{};
                for (const auto &wpContainedContained : spContainedObject->ContainedWeakPointers())
                {
                    if (const auto spContainedContained = wpContainedContained.lock())
                        containedContained.insert(spContainedContained);
                }

                auto newColourNumber = SIZE_T(0UL);

                if (containedContained.empty())
                    newColourNumber = colourNumber + SIZE_T(1UL);

                else
                    newColourNumber = colourNumber + colourIncrement++;

                RecursivelyWriteDotInformation(
                    definitionsDotCode, spContainedObject, connectionsDotCode, fontName, fontSize,
                    leftAlignText, setOfAllObjects, completedConnections, newColourNumber,
                    indentationLevel, displayPseudoEdges);
            }
        }

        if (indentationLevel > SIZE_T(0UL)) --indentationLevel;

        definitionsDotCode.push_back(GetIndentation(indentationLevel) + "}"s);

        if (displayPseudoEdges)
            WritePseudoEdges(connectionsDotCode, spSetObject, setOfAllObjects, completedConnections,
                             fontName, fontSize);

        else
            WriteEdges(connectionsDotCode, spSetObject, setOfAllObjects, completedConnections,
                       fontName, fontSize);
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline std::tuple<std::string, std::string> HierarchicalVisualizationUtility<TBASE>::GetSetColours(
    std::size_t colourNumber) noexcept
{
    auto setColour = std::string{};

    while (colourNumber > SIZE_T(2UL)) colourNumber -= SIZE_T(3UL);

    switch (colourNumber)
    {
        case SIZE_T(0UL):
            return {std::string{KL_GRAPH_BLUE}, std::string{KL_GRAPH_WHITE}};
        case SIZE_T(1UL):
            return {std::string{KL_GRAPH_RED}, std::string{KL_GRAPH_WHITE}};
        case SIZE_T(2UL):
        default:
            break;
    }

    return {std::string{KL_GRAPH_GREEN}, std::string{KL_GRAPH_BLACK}};
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
void HierarchicalVisualizationUtility<TBASE>::RecursivelyGetSetOfAllObjects(
    TBASE_sPtrSet &setOfAllObjects, const TBASE_sPtr &spSetObject) noexcept
{
    setOfAllObjects.insert(spSetObject);

    for (const auto &wpContainedObject : spSetObject->ContainedWeakPointers())
    {
        if (const auto spContainedObject = wpContainedObject.lock())
            RecursivelyGetSetOfAllObjects(setOfAllObjects, spContainedObject);
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
void HierarchicalVisualizationUtility<TBASE>::WriteDotCodeDefinitions(
    StringVector &definitionsDotCode, const TBASE_sPtr &spSetObject, const std::string &fontName,
    const std::string &fontSize, const std::string &colour, const std::string &fontColour,
    const bool leftAlignText, std::size_t &indentationLevel) noexcept
{
    using namespace std::string_literals;

    // Get and format the node label.
    auto nodeLabel = spSetObject->GetGraphNodeLabel();

    // Escape any bad characters in the label.
    StringUtility::ReplaceInString(nodeLabel, "\\"s, "\\\\"s);
    StringUtility::ReplaceInString(nodeLabel, "\""s, "\\\""s);

    if (leftAlignText)
    {
        // In dot, having \l at the end of a line instead of \n indicates that the current line is
        // to be left-aligned.
        StringUtility::ReplaceInString(nodeLabel, "\n"s, "\\l"s);
        nodeLabel += "\\l"s;
    }

    // Define the object at hand.
    definitionsDotCode.push_back(
        GetIndentation(indentationLevel) + "node"s + std::to_string(spSetObject->ID()) +
        "[shape=box, "s + "label=\""s + nodeLabel + "\", color=\""s + colour + "\", fontcolor=\""s +
        fontColour + "\", "s + "fontname=\""s + fontName + "\", fontsize="s + fontSize +
        ", fillcolor=\""s + colour + "\", "s + "style=\"filled,rounded\"];"s);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
void HierarchicalVisualizationUtility<TBASE>::WritePseudoEdges(StringVector &connectionsDotCode,
                                                               const TBASE_sPtr &spSetObject,
                                                               const TBASE_sPtrSet &setOfAllObjects,
                                                               ConnectionSet &completedConnections,
                                                               const std::string &fontName,
                                                               const std::string &fontSize) noexcept
{
    using namespace std::string_literals;

    // Write the daughter connections.
    for (const auto &wpEdge : spSetObject->DaughterEdgeWeakPointers())
    {
        if (const auto spEdge = wpEdge.lock())
        {
            const auto spDaughter = spEdge->ObjectWeakPointer().lock();

            if (!spDaughter) continue;

            if (setOfAllObjects.find(spDaughter) == setOfAllObjects.cend()) continue;

            if (completedConnections.find(std::make_tuple(spSetObject->ID(), spDaughter->ID(),
                                                          spEdge->UnderlyingEdgeSharedPointer())) !=
                completedConnections.cend())
            {
                continue;
            }

            // Write the connection.
            auto fullWhite = false;

            for (const auto &wpParentEdge : spDaughter->ParentEdgeWeakPointers())
            {
                if (const auto spParentEdge = wpParentEdge.lock())
                {
                    if (spParentEdge->ObjectWeakPointer() == static_cast<TBASE_wPtr>(spSetObject))
                    {
                        fullWhite = true;
                        break;
                    }
                }
            }

            const auto colourString = fullWhite ? "color=\""s + std::string{KL_GRAPH_WHITE} + "\""
                                                : "color=\""s + std::string{KL_GRAPH_WHITE} +
                                                      ";0.5:"s + std::string{KL_GRAPH_GREY} + "\"";

            auto edgeLabel = spEdge->UnderlyingEdgeSharedPointer()->GetGraphEdgeLabel();
            StringUtility::ReplaceInString(edgeLabel, "\n"s, "\\l"s);
            edgeLabel += "\\l"s;

            const auto labelString = ", label=\"" + edgeLabel + "\", fontcolor=\"" +
                                     std::string{KL_GRAPH_WHITE} + "\", fontname=\"" + fontName +
                                     "\", fontsize=" + fontSize;

            auto styleString = std::string{};

            switch (spEdge->UnderlyingEdgeSharedPointer()->GetGraphEdgeStyle())
            {
                case HierarchicalEdgeBase<TBASE_D>::STYLE::DASHED:
                    styleString = ", style=\"dashed\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::DOTTED:
                    styleString = ", style=\"dotted\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::BOLD:
                    styleString = ", style=\"bold\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::SOLID:
                default:
                    styleString = ", style=\"solid\"";
                    break;
            }

            auto headTailString = std::string{};

            if (ContainsDisplayedVertex(spSetObject, setOfAllObjects))
            {
                const auto &daughterContaining = spDaughter->ContainingWeakPointers();
                const auto &parentContaining = spSetObject->ContainingWeakPointers();

                if ((spSetObject != spDaughter) &&
                    (daughterContaining.find(spSetObject) == daughterContaining.end()) &&
                    (parentContaining.find(spDaughter) == parentContaining.end()))
                {
                    headTailString += ", ltail=cluster"s + std::to_string(spSetObject->ID());
                }
            }

            if (ContainsDisplayedVertex(spDaughter, setOfAllObjects))
            {
                const auto &daughterContaining = spDaughter->ContainingWeakPointers();
                const auto &parentContaining = spSetObject->ContainingWeakPointers();

                if ((spSetObject != spDaughter) &&
                    (daughterContaining.find(spSetObject) == daughterContaining.end()) &&
                    (parentContaining.find(spDaughter) == parentContaining.end()))
                {
                    headTailString += ", lhead=cluster"s + std::to_string(spDaughter->ID());
                }
            }

            connectionsDotCode.push_back(GetIndentation(SIZE_T(1UL)) + "node"s +
                                         std::to_string(spSetObject->ID()) + "->node"s +
                                         std::to_string(spDaughter->ID()) + " ["s + colourString +
                                         labelString + styleString + headTailString + "];"s);

            // Put it in the list.
            completedConnections.emplace(spSetObject->ID(), spDaughter->ID(),
                                         spEdge->UnderlyingEdgeSharedPointer());
        }
    }

    // Write the parent connections.
    for (const auto &wpEdge : spSetObject->ParentEdgeWeakPointers())
    {
        if (const auto spEdge = wpEdge.lock())
        {
            const auto spParent = spEdge->ObjectWeakPointer().lock();

            if (!spParent) continue;

            if (setOfAllObjects.find(spParent) == setOfAllObjects.cend()) continue;

            if (completedConnections.find(std::make_tuple(spParent->ID(), spSetObject->ID(),
                                                          spEdge->UnderlyingEdgeSharedPointer())) !=
                completedConnections.cend())
            {
                continue;
            }

            // Write the connection.
            auto fullWhite = false;

            for (const auto &wpDaughterEdge : spParent->DaughterEdgeWeakPointers())
            {
                if (const auto spDaughterEdge = wpDaughterEdge.lock())
                {
                    if (spDaughterEdge->ObjectWeakPointer() == static_cast<TBASE_wPtr>(spSetObject))
                    {
                        fullWhite = true;
                        break;
                    }
                }
            }

            if (fullWhite) continue;

            const auto colourString = fullWhite ? "color=\""s + std::string{KL_GRAPH_WHITE} + "\""
                                                : "color=\""s + std::string{KL_GRAPH_GREY} +
                                                      ";0.5:"s + std::string{KL_GRAPH_WHITE} + "\"";

            auto edgeLabel = spEdge->UnderlyingEdgeSharedPointer()->GetGraphEdgeLabel();
            StringUtility::ReplaceInString(edgeLabel, "\n"s, "\\l"s);
            edgeLabel += "\\l"s;

            const auto labelString = ", label=\"" + edgeLabel + "\", fontcolor=\"" +
                                     std::string{KL_GRAPH_WHITE} + "\", fontname=\"" + fontName +
                                     "\", fontsize=" + fontSize;

            auto styleString = std::string{};

            switch (spEdge->UnderlyingEdgeSharedPointer()->GetGraphEdgeStyle())
            {
                case HierarchicalEdgeBase<TBASE_D>::STYLE::DASHED:
                    styleString = ", style=\"dashed\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::DOTTED:
                    styleString = ", style=\"dotted\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::BOLD:
                    styleString = ", style=\"bold\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::SOLID:
                default:
                    styleString = ", style=\"solid\"";
                    break;
            }

            auto headTailString = std::string{};

            if (ContainsDisplayedVertex(spSetObject, setOfAllObjects))
            {
                const auto &daughterContaining = spSetObject->ContainingWeakPointers();
                const auto &parentContaining = spParent->ContainingWeakPointers();

                if ((spSetObject != spParent) &&
                    (daughterContaining.find(spParent) == daughterContaining.end()) &&
                    (parentContaining.find(spSetObject) == parentContaining.end()))
                {
                    headTailString += ", lhead=cluster"s + std::to_string(spSetObject->ID());
                }
            }

            if (ContainsDisplayedVertex(spParent, setOfAllObjects))
            {
                const auto &daughterContaining = spSetObject->ContainingWeakPointers();
                const auto &parentContaining = spParent->ContainingWeakPointers();

                if ((spSetObject != spParent) &&
                    (daughterContaining.find(spParent) == daughterContaining.end()) &&
                    (parentContaining.find(spSetObject) == parentContaining.end()))
                {
                    headTailString += ", ltail=cluster"s + std::to_string(spParent->ID());
                }
            }

            connectionsDotCode.push_back(GetIndentation(SIZE_T(1UL)) + "node"s +
                                         std::to_string(spParent->ID()) + "->node"s +
                                         std::to_string(spSetObject->ID()) + " ["s + colourString +
                                         labelString + styleString + headTailString + "];"s);

            // Put it in the list.
            completedConnections.emplace(spParent->ID(), spSetObject->ID(),
                                         spEdge->UnderlyingEdgeSharedPointer());
        }
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
void HierarchicalVisualizationUtility<TBASE>::WriteEdges(StringVector &connectionsDotCode,
                                                         const TBASE_sPtr &spSetObject,
                                                         const TBASE_sPtrSet &setOfAllObjects,
                                                         ConnectionSet &completedConnections,
                                                         const std::string &fontName,
                                                         const std::string &fontSize) noexcept
{
    using namespace std::string_literals;

    // Write the daughter connections.
    for (const auto &wpPseudoEdge : spSetObject->DaughterEdgeWeakPointers())
    {
        if (const auto spPseudoEdge = wpPseudoEdge.lock())
        {
            const auto &spEdge = spPseudoEdge->UnderlyingEdgeSharedPointer();
            const auto spDaughter = spEdge->DaughterWeakPointer().lock();
            const auto spParent = spEdge->ParentWeakPointer().lock();

            if (!spDaughter || !spParent) continue;

            if (setOfAllObjects.find(spDaughter) == setOfAllObjects.cend()) continue;

            if (completedConnections.find(std::make_tuple(spParent->ID(), spDaughter->ID(),
                                                          spEdge)) != completedConnections.cend())
                continue;

            // Write the connection.
            const auto colourString = spEdge->IsInheritable()
                                          ? "color=\""s + std::string{KL_GRAPH_WHITE} + "\""
                                          : "color=\""s + std::string{KL_GRAPH_GREY} + "\"";

            auto edgeLabel = spEdge->GetGraphEdgeLabel();
            StringUtility::ReplaceInString(edgeLabel, "\n"s, "\\l"s);
            edgeLabel += "\\l"s;

            const auto labelString = ", label=\"" + edgeLabel + "\", fontcolor=\"" +
                                     std::string{KL_GRAPH_WHITE} + "\", fontname=\"" + fontName +
                                     "\", fontsize=" + fontSize;

            auto styleString = std::string{};

            switch (spEdge->GetGraphEdgeStyle())
            {
                case HierarchicalEdgeBase<TBASE_D>::STYLE::DASHED:
                    styleString = ", style=\"dashed\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::DOTTED:
                    styleString = ", style=\"dotted\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::BOLD:
                    styleString = ", style=\"bold\"";
                    break;
                case HierarchicalEdgeBase<TBASE_D>::STYLE::SOLID:
                default:
                    styleString = ", style=\"solid\"";
                    break;
            }

            // Work out whether the heads/tails of connectors are in subgraphs.
            auto headTailString = std::string{};

            if (ContainsDisplayedVertex(spDaughter, setOfAllObjects))
            {
                const auto &daughterContaining = spDaughter->ContainingWeakPointers();
                const auto &parentContaining = spParent->ContainingWeakPointers();

                if ((spDaughter != spParent) &&
                    (daughterContaining.find(spParent) == daughterContaining.end()) &&
                    (parentContaining.find(spDaughter) == parentContaining.end()))
                {
                    headTailString += ", lhead=cluster"s + std::to_string(spDaughter->ID());
                }
            }

            if (ContainsDisplayedVertex(spParent, setOfAllObjects))
            {
                const auto &daughterContaining = spDaughter->ContainingWeakPointers();
                const auto &parentContaining = spParent->ContainingWeakPointers();

                if ((spDaughter != spParent) &&
                    (daughterContaining.find(spParent) == daughterContaining.end()) &&
                    (parentContaining.find(spDaughter) == parentContaining.end()))
                {
                    headTailString += ", ltail=cluster"s + std::to_string(spParent->ID());
                }
            }

            connectionsDotCode.push_back(GetIndentation(SIZE_T(1UL)) + "node"s +
                                         std::to_string(spParent->ID()) + "->node"s +
                                         std::to_string(spDaughter->ID()) + " ["s + colourString +
                                         labelString + styleString + headTailString + "];"s);

            // Put it in the list.
            completedConnections.emplace(spSetObject->ID(), spDaughter->ID(), spEdge);
        }
    }
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline std::size_t HierarchicalVisualizationUtility<TBASE>::GenerateRandomNumber(
    const std::size_t min, const std::size_t max)
{
    auto generator = std::default_random_engine{};
    generator.seed(SIZE_T(Clock::now().time_since_epoch().count()));

    auto distribution = std::uniform_int_distribution<std::size_t>{min, max};
    return distribution(generator);
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
inline std::string HierarchicalVisualizationUtility<TBASE>::GetIndentation(
    const std::size_t indentationLevel)
{
    return std::string(indentationLevel * SIZE_T(4UL), ' ');
}

//--------------------------------------------------------------------------------------------------

template <typename TBASE>
void HierarchicalVisualizationUtility<TBASE>::ProcessObjectSet(TBASE_sPtrSet &objectSet)
{
    auto objectWeakPointers = TBASE_wPtrSet{};

    for (const auto &spObject : objectSet)
    {
        if (spObject) objectWeakPointers.emplace(static_cast<TBASE_wPtr>(spObject));
    }

    for (auto iter = objectSet.begin(); iter != objectSet.end(); /* no increment */)
    {
        auto toDelete = false;

        for (const auto &wpContaining : (*iter)->ContainingWeakPointers())
        {
            if (objectWeakPointers.find(wpContaining) != objectWeakPointers.end())
            {
                toDelete = true;
                break;
            }
        }

        if (toDelete)
            iter = objectSet.erase(iter);

        else
            ++iter;
    }
}
}  // namespace kl

#endif  // #ifndef KL_HIERARCHICAL_VISUALIZATION_UTILITY_IMPL_H