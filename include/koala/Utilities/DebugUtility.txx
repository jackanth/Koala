/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/DebugUtility.txx
 *
 * @brief Template implementation header file for the debug utility (DebugUtility) class.
 */

#ifndef KOALA_DEBUG_UTILITY_IMPL_H
#define KOALA_DEBUG_UTILITY_IMPL_H 1

namespace kl
{
template <typename TOBJECT, typename... TOBJECTS>
auto DebugUtility::GetObjectAssociationInformation(const bool colourize, TOBJECT &&object,
                                                   TOBJECTS &&... objects)
{
    auto objectInformation =
        GetObjectAssociationInformation(colourize, std::forward<TOBJECT>(object));
    auto otherObjectsInfo =
        GetObjectAssociationInformation(colourize, std::forward<TOBJECTS>(objects)...);
    objectInformation.insert(objectInformation.cend(),
                             std::make_move_iterator(otherObjectsInfo.begin()),
                             std::make_move_iterator(otherObjectsInfo.end()));
    return objectInformation;
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
auto DebugUtility::GetObjectAssociationInformation(const bool colourize, TOBJECT &&object)
{
    // First get the longest names so the spacing can be improved.
    const auto [longestTypeName, longestId, longestRegistryName, longestIdentifier] =
        DebugUtility::GetLongestNames(object);

    // Set spacings for intro string.
    auto typeNameSpacing = SIZE_T(0UL);
    if (longestTypeName >= object.PrintableName().size())
        typeNameSpacing = longestTypeName + SIZE_T(5UL) - object.PrintableName().size();
    else
        typeNameSpacing = SIZE_T(5UL) + object.PrintableName().size();

    auto idSpacing = SIZE_T(0UL);
    if (longestId >= std::to_string(object.ID()).size())
        idSpacing = longestId - std::to_string(object.ID()).size();
    else
        idSpacing = std::to_string(object.ID()).size();

    auto registryNameSpacing = SIZE_T(0UL);
    if (longestRegistryName >= object.GetRegistryName().size())
        registryNameSpacing = longestRegistryName - object.GetRegistryName().size();
    else
        registryNameSpacing = object.GetRegistryName().size();

    // Write intro string.
    using namespace std::string_literals;
    auto introString = "> "s;

    introString += colourize ? std::string{KL_GREEN_BOLD} + object.PrintableName() +
                                   std::string(typeNameSpacing, ' ') + std::string{KL_NORMAL} +
                                   std::to_string(object.ID()) + std::string(idSpacing, ' ') +
                                   " ["s + std::string{KL_GREEN_BOLD} + object.GetRegistryName() +
                                   std::string{KL_NORMAL} + "]"s
                             : object.PrintableName() + std::string(typeNameSpacing, ' ') +
                                   std::to_string(object.ID()) + std::string(idSpacing, ' ') +
                                   " ["s + object.GetRegistryName() + "]"s;

    const auto objectIdentifier = object.GetIdentifierString();
    if (!objectIdentifier.empty())
    {
        introString += colourize
                           ? std::string(registryNameSpacing, ' ') + " -> "s +
                                 std::string{KL_GREEN} + objectIdentifier + std::string{KL_NORMAL}
                           : std::string(registryNameSpacing, ' ') + " -> "s + objectIdentifier;
    }

    auto informationVector = StringVector{introString};
    DebugUtility::PopulateInformationVector(informationVector, object, longestTypeName, longestId,
                                            longestRegistryName, longestIdentifier, colourize);

    return informationVector;
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECTA, typename TOBJECTB, typename... TOBJECTS>
auto DebugUtility::GetRegistryAssociationInformation(const bool colourize, const Koala &koala)
{
    auto objectInformation = GetRegistryAssociationInformation<TOBJECTA>(colourize, koala);
    auto otherObjectsInfo =
        GetRegistryAssociationInformation<TOBJECTB, TOBJECTS...>(colourize, koala);
    objectInformation.insert(objectInformation.cend(),
                             std::make_move_iterator(otherObjectsInfo.begin()),
                             std::make_move_iterator(otherObjectsInfo.end()));
    return objectInformation;
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
auto DebugUtility::GetObjectAssociationInformation(const bool colourize, const Koala &koala)
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    auto informationVector = StringVector{};

    // Check registry exists.
    if (!koala.HasRegistry<TOBJECT_D>())
        KL_THROW(
            "Could not get association information for given object type because no such registry "
            "exists");

    auto &objectRegistry = koala.FetchRegistry<TOBJECT_D>();

    using namespace std::string_literals;
    auto introString =
        colourize
            ? "> "s + std::string{KL_WHITE_BOLD} + "Object"s + std::string{KL_NORMAL} + ": "s +
                  std::string{KL_GREEN_BOLD} + TOBJECT_D::StaticPrintableName() +
                  std::string{KL_NORMAL} + " ["s + std::string{KL_BOLD} +
                  std::to_string(objectRegistry.template Count<TOBJECT_D>()) +
                  std::string{KL_NORMAL} + " objects from registry "s + std::string{KL_GREEN_BOLD} +
                  objectRegistry.PrintableBaseName() + std::string{KL_NORMAL} + "]"s
            : "> Object: "s + TOBJECT_D::StaticPrintableName() + " ["s +
                  std::to_string(objectRegistry.template Count<TOBJECT_D>()) +
                  " objects from registry "s + objectRegistry.PrintableBaseName() + "]"s;

    informationVector.push_back(introString);

    for (const auto &object : objectRegistry.template RangeBasedGetAll<TOBJECT_D>())
    {
        auto associationInfoVector = GetObjectAssociationInformation(colourize, object);
        informationVector.insert(informationVector.end(),
                                 std::make_move_iterator(associationInfoVector.begin()),
                                 std::make_move_iterator(associationInfoVector.end()));
    }

    return informationVector;
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
auto DebugUtility::GetRegistryAssociationInformation(const bool colourize, const Koala &koala)
{
    using TOBJECT_D = std::decay_t<TOBJECT>;
    auto informationVector = StringVector{};

    // Check registry exists.
    if (!koala.HasRegistry<TOBJECT_D>())
        KL_THROW(
            "Could not get association information for given object type because no such registry "
            "exists");

    auto &objectRegistry = koala.FetchRegistry<TOBJECT_D>();

    using namespace std::string_literals;
    auto introString =
        colourize
            ? "> "s + std::string{KL_WHITE_BOLD} + "Registry"s + std::string{KL_NORMAL} + ": "s +
                  std::string{KL_GREEN_BOLD} + objectRegistry.PrintableBaseName() +
                  std::string{KL_NORMAL} + " ["s + std::string{KL_BOLD} +
                  std::to_string(objectRegistry.CountAll()) + std::string{KL_NORMAL} + " objects]"s
            : "> Registry: "s + objectRegistry.PrintableBaseName() + " ["s +
                  std::to_string(objectRegistry.CountAll()) + " objects]"s;

    informationVector.push_back(introString);

    for (const auto &object : objectRegistry.template RangeBasedGetAll())
    {
        auto associationInfoVector = GetObjectAssociationInformation(colourize, object);
        informationVector.insert(informationVector.end(),
                                 std::make_move_iterator(associationInfoVector.begin()),
                                 std::make_move_iterator(associationInfoVector.end()));
    }

    return informationVector;
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
std::tuple<std::size_t, std::size_t, std::size_t, std::size_t> DebugUtility::GetLongestNames(
    TOBJECT &&object)
{
    auto longestTypeName = object.PrintableName().size();
    auto longestId = std::to_string(object.ID()).size();
    auto longestRegistryName = object.GetRegistryName().size();
    auto longestIdentifier = object.GetIdentifierString().size();

    for (const auto associationInformation : object.GetAssociationInformation())
    {
        auto typeNameSize = associationInformation.TypeName().size();
        auto idSize = std::to_string(associationInformation.ID()).size();
        auto registryNameSize = associationInformation.RegistryName().size();
        auto identifierSize = associationInformation.IdentifierString().size();

        if (typeNameSize > longestTypeName) longestTypeName = typeNameSize;

        if (idSize > longestId) longestId = idSize;

        if (registryNameSize > longestRegistryName) longestRegistryName = registryNameSize;

        if (identifierSize > longestIdentifier) longestIdentifier = identifierSize;
    }

    // Enforce max sizes.
    longestTypeName = std::min(longestTypeName, SIZE_T(50UL));
    longestId = std::min(longestId, SIZE_T(10UL));
    longestRegistryName = std::min(longestRegistryName, SIZE_T(50UL));
    longestIdentifier = std::min(longestIdentifier, SIZE_T(80UL));

    return {longestTypeName, longestId, longestRegistryName, longestIdentifier};
}

//--------------------------------------------------------------------------------------------------

template <typename TOBJECT>
void DebugUtility::PopulateInformationVector(StringVector &informationVector, TOBJECT &&object,
                                             const std::size_t longestTypeName,
                                             const std::size_t longestId,
                                             const std::size_t longestRegistryName,
                                             const std::size_t longestIdentifier,
                                             const bool colourize)
{
    // Sort association information alphabetically by typename, then by ID ascending.
    auto associationInformationVector = object.GetAssociationInformation();
    std::sort(associationInformationVector.begin(), associationInformationVector.end(),
              [](const auto &lhs, const auto &rhs) {
                  return ((lhs.TypeName() < rhs.TypeName()) ||
                          ((lhs.TypeName() == rhs.TypeName()) && (lhs.ID() < rhs.ID())));
              });

    for (const auto &associationInformation : associationInformationVector)
    {
        auto typeNameSpacing = SIZE_T(0UL);
        auto idSpacing = SIZE_T(0UL);
        auto registryNameSpacing = SIZE_T(0UL);
        auto identifierSpacing = SIZE_T(0UL);

        // Set the spacings for the association string.
        if (longestTypeName >= associationInformation.TypeName().size())
            typeNameSpacing = longestTypeName - associationInformation.TypeName().size();
        else
            typeNameSpacing = associationInformation.TypeName().size();

        if (longestId >= std::to_string(associationInformation.ID()).size())
            idSpacing = longestId - std::to_string(associationInformation.ID()).size();
        else
            idSpacing = std::to_string(associationInformation.ID()).size();

        if (longestRegistryName >= associationInformation.RegistryName().size())
            registryNameSpacing =
                longestRegistryName - associationInformation.RegistryName().size();
        else
            registryNameSpacing = associationInformation.RegistryName().size();

        if (longestIdentifier >= associationInformation.IdentifierString().size())
            identifierSpacing =
                longestIdentifier - associationInformation.IdentifierString().size();
        else
            identifierSpacing = associationInformation.IdentifierString().size();

        // Write the association string.
        using namespace std::string_literals;

        auto assocString = std::string{"    > "};

        assocString +=
            colourize
                ? std::string{KL_MAGENTA} + associationInformation.TypeName() +
                      std::string(typeNameSpacing + SIZE_T(1UL), ' ') + std::string{KL_NORMAL} +
                      std::to_string(associationInformation.ID()) + std::string(idSpacing, ' ') +
                      " ["s + std::string{KL_MAGENTA} + associationInformation.RegistryName() +
                      std::string{KL_NORMAL} + "]"s
                : associationInformation.TypeName() +
                      std::string(typeNameSpacing + SIZE_T(1UL), ' ') +
                      std::to_string(associationInformation.ID()) + std::string(idSpacing, ' ') +
                      " ["s + associationInformation.RegistryName() + "]"s;

        const auto assocIdentifier = associationInformation.IdentifierString();
        if (!assocIdentifier.empty())
        {
            assocString +=
                colourize ? std::string(registryNameSpacing, ' ') + " -> "s +
                                std::string{KL_GREEN} + assocIdentifier + std::string{KL_NORMAL}
                          : std::string(registryNameSpacing, ' ') + " -> "s + assocIdentifier;

            if (associationInformation.HasIndicator() || associationInformation.IsAlive() ||
                associationInformation.IsCerealSerializable())
                assocString += std::string(identifierSpacing, ' ');
        }

        else if (associationInformation.HasIndicator() || associationInformation.IsAlive() ||
                 associationInformation.IsCerealSerializable())
            assocString += "    "s + std::string(registryNameSpacing, ' ') +
                           std::string(identifierSpacing, ' ');

        // Hereafter are flags which can pile up and don't need to be vertically aligned.
        if (associationInformation.HasIndicator())
        {
            auto indicator = associationInformation.IndicatorString();
            indicator = indicator.empty() ? "?"s : indicator;

            assocString += colourize ? " [indicator: "s + std::string{KL_WHITE_BOLD} + indicator +
                                           std::string{KL_NORMAL} + "]"s
                                     : " [indicator: "s + indicator + "]"s;
        }

        if (!associationInformation.IsAlive())
        {
            assocString += colourize
                               ? std::string{KL_RED_BOLD} + " [dead]"s + std::string{KL_NORMAL}
                               : " [dead]"s;
        }

        if (!associationInformation.IsCerealSerializable())
        {
            assocString += colourize ? std::string{KL_RED_BOLD} + " [not serializable]"s +
                                           std::string{KL_NORMAL}
                                     : " [not serializable]"s;
        }

        informationVector.push_back(assocString);
    }
}
}  // namespace kl

#endif  // #ifndef KOALA_DEBUG_UTILITY_IMPL_H