/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Utilities/StringUtility.h
 *
 * @brief Header file for the string utility (StringUtility) class.
 */

#ifndef KL_STRING_UTILITY_H
#define KL_STRING_UTILITY_H 1

#include <regex>
#include "koala/Definitions.h"

namespace kl
{
/**
 * @brief StringUtility class.
 */
class StringUtility
{
public:
    /**
     * @brief Deleted constructor.
     */
    StringUtility() = delete;

    /**
     * @brief Deleted copy constructor.
     */
    StringUtility(const StringUtility &) = delete;

    /**
     * @brief Deleted move constructor.
     */
    StringUtility(StringUtility &&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    StringUtility &operator=(const StringUtility &) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    StringUtility &operator=(StringUtility &&) = delete;

    /**
     * @brief Deleted destructor.
     */
    ~StringUtility() = delete;

    //----------------------------------------------------------------------------------------------

    /**
     * @brief Trim the whitespace off the start and end of a string.
     *
     * @param stringToTrim The string to trim.
     */
    static void TrimWhitespace(std::string &stringToTrim) noexcept;

    /**
     * @brief Replace every instance of a substring within a string with another string.
     *
     * @param fullString The original string.
     * @param fromString The substring to replace.
     * @param toString The substring with which to replace.
     *
     * @return Whether something has changed.
     */
    static bool ReplaceInString(std::string &fullString, const std::string_view &fromString,
                                const std::string_view &toString) noexcept;

    /**
     * @brief Replace every instance of a regex with a string.
     *
     * @param fullString The original string.
     * @param fromRegexString The string representing the regex to replace.
     * @param toString The substring with which to replace.
     *
     * @return Whether something has changed.
     */
    static bool ReplaceRegexInString(std::string &fullString, const std::string &fromRegexString,
                                     const std::string &toString) noexcept;

    /**
     * @brief Split a given string at every instance of a given character and return a vector of
     * the split string parts.
     *
     * @param inputString The input string.
     * @param splitChar The character at which to split the string.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static StringVector SplitStringAt(const std::string_view &inputString, const char splitChar,
                                      const bool trimWhitespace = false) noexcept;

    /**
     * @brief Split a given string at every instance of a given character and the split element at
     * the given index.
     *
     * @param inputString The input string.
     * @param splitChar The character at which to split the string.
     * @param elementIndex The index of the element to return.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static std::string SplitStringAtAndGetElement(const std::string_view &inputString,
                                                  const char splitChar,
                                                  const std::size_t elementIndex,
                                                  const bool trimWhitespace = false);

    /**
     * @brief Split a given string at every instance of a given string and return a vector of the
     * split string parts.
     *
     * @param inputString The input string.
     * @param splitString The string at which to split the string.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static StringVector SplitStringAt(const std::string_view &inputString,
                                      const std::string_view &splitString,
                                      const bool trimWhitespace = false) noexcept;

    /**
     * @brief Split a given string at every instance of a given string and return the split element
     * at the given index.
     *
     * @param inputString The input string.
     * @param splitString The string at which to split the string.
     * @param elementIndex The index of the element to return.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static std::string SplitStringAtAndGetElement(const std::string_view &inputString,
                                                  const std::string_view &splitString,
                                                  const std::size_t elementIndex,
                                                  bool trimWhitespace = false);

    /**
     * @brief Split a given string after every instance of a given character and return a vector of
     * the split string parts.
     *
     * @param inputString The input string.
     * @param splitChar The character after which to split the string.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static StringVector SplitStringAfter(const std::string_view &inputString, const char splitChar,
                                         const bool trimWhitespace = false) noexcept;

    /**
     * @brief Split a given string after every instance of a given character and return the split
     * element at the given index.
     *
     * @param inputString The input string.
     * @param splitChar The character after which to split the string.
     * @param elementIndex The index of the element to return.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static std::string SplitStringAfterAndGetElement(const std::string_view &inputString,
                                                     const char &splitChar,
                                                     const std::size_t elementIndex,
                                                     const bool trimWhitespace = false);

    /**
     * @brief Split a given string after every instance of a given string and return a vector of
     * the split string parts.
     *
     * @param inputString The input string.
     * @param splitString The string after which to split the string.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static StringVector SplitStringAfter(const std::string_view &inputString,
                                         const std::string_view &splitString,
                                         const bool trimWhitespace = false) noexcept;

    /**
     * @brief Split a given string after every instance of a given string and return the split
     * element at the given index.
     *
     * @param inputString The input string.
     * @param splitString The string after which to split the string.
     * @param elementIndex The index of the element to return.
     * @param trimWhitespace Whether to trim any whitespace surrounding the resulting substrings.
     *
     * @return The StringVector object of resulting substrings.
     */
    static std::string SplitStringAfterAndGetElement(const std::string_view &inputString,
                                                     const std::string_view &splitString,
                                                     const std::size_t elementIndex,
                                                     const bool trimWhitespace = false);

    /**
     * @brief Strip the quotation marks from around a string if they exist.
     *
     * @param inputString The input string.
     *
     * @return The new string.
     */
    static std::string StripQuotationMarks(const std::string &inputString) noexcept;

    /**
     * @brief Find out whether a given string starts with some other string.
     *
     * @param inputString The input string.
     * @param startString The starting string to use.
     *
     * @return Whether the string starts with the starting string.
     */
    static bool DoesStringStartWith(const std::string_view &inputString,
                                    const std::string_view &startString) noexcept;
};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

inline void StringUtility::TrimWhitespace(std::string &stringToTrim) noexcept
{
    // We want to replace any tabs with spaces.
    ReplaceInString(stringToTrim, "\t", " ");

    // We want to replace any double spaces with single spaces.
    ReplaceInString(stringToTrim, "  ", " ");

    // Trim whitespace from the start of the string.
    stringToTrim.erase(stringToTrim.cbegin(),
                       std::find_if(stringToTrim.cbegin(), stringToTrim.cend(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));

    // Trim whitespace from the end of the string.
    stringToTrim.erase(std::find_if(stringToTrim.crbegin(), stringToTrim.crend(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace)))
                           .base(),
                       stringToTrim.cend());
}

//--------------------------------------------------------------------------------------------------

inline bool StringUtility::ReplaceInString(std::string &fullString,
                                           const std::string_view &fromString,
                                           const std::string_view &toString) noexcept
{
    auto somethingChanged = false;
    auto startPosition = SIZE_T(0UL);

    // Loop through the string and find every instance of the from-string.
    while ((startPosition = fullString.find(fromString, startPosition)) != std::string::npos)
    {
        // If found, replace it and jump ahead.
        somethingChanged = true;
        fullString.replace(startPosition, fromString.length(), toString);
        startPosition += toString.length();  // in case toString contains fromString
    }

    return somethingChanged;
}

//--------------------------------------------------------------------------------------------------

inline bool StringUtility::ReplaceRegexInString(std::string &fullString,
                                                const std::string &fromRegexString,
                                                const std::string &toString) noexcept
{
    auto oldString = fullString;
    fullString = std::regex_replace(fullString, std::regex{fromRegexString}, toString);
    return (fullString == oldString);
}

//--------------------------------------------------------------------------------------------------

inline StringVector StringUtility::SplitStringAt(const std::string_view &inputString,
                                                 const char splitChar,
                                                 const bool trimWhitespace) noexcept
{
    auto resultVector = StringVector{};

    // Every time we reach the split character, split the string (dropping the split character).
    auto item = std::string{};
    for (auto i = SIZE_T(0UL); i < inputString.length(); ++i)
    {
        if (inputString.at(i) == splitChar && !item.empty())
        {
            if (trimWhitespace) TrimWhitespace(item);

            if (!item.empty()) resultVector.push_back(item);

            item.clear();
        }

        else
            item += inputString.at(i);
    }

    // Add to the result vector even if the item string is empty (by convention).
    if (trimWhitespace) TrimWhitespace(item);

    if (!item.empty()) resultVector.push_back(item);

    return resultVector;
}

//--------------------------------------------------------------------------------------------------

inline std::string StringUtility::SplitStringAtAndGetElement(const std::string_view &inputString,
                                                             const char splitChar,
                                                             const std::size_t elementIndex,
                                                             const bool trimWhitespace)
{
    auto resultVector = StringUtility::SplitStringAt(inputString, splitChar, trimWhitespace);
    if (elementIndex >= resultVector.size())
    {
        KL_THROW("Could not get element " << KL_WHITE_BOLD << elementIndex << KL_NORMAL
                                          << " after splitting string because vector "
                                          << "was of size " << KL_WHITE_BOLD
                                          << resultVector.size());
    }

    return resultVector.at(elementIndex);
}

//--------------------------------------------------------------------------------------------------

inline StringVector StringUtility::SplitStringAt(const std::string_view &inputString,
                                                 const std::string_view &splitString,
                                                 const bool trimWhitespace) noexcept
{
    auto resultVector = StringVector{};

    // Every time we reach the split character, split the string (dropping the split character).
    auto item = std::string{};
    for (auto i = SIZE_T(0UL); (inputString.length() < splitString.length())
                                   ? false
                                   : i < SIZE_T(1UL) + inputString.length() - splitString.length();
         ++i)
    {
        if (inputString.substr(i, splitString.length()) == splitString && !item.empty())
        {
            if (trimWhitespace) TrimWhitespace(item);

            if (!item.empty()) resultVector.push_back(item);

            item.clear();
            i += splitString.length() - SIZE_T(1UL);
        }

        else
            item += inputString.at(i);
    }

    // Add to the result vector even if the item string is empty (by convention).
    if (trimWhitespace) TrimWhitespace(item);

    if (!item.empty()) resultVector.push_back(item);

    return resultVector;
}

//--------------------------------------------------------------------------------------------------

inline std::string StringUtility::SplitStringAtAndGetElement(const std::string_view &inputString,
                                                             const std::string_view &splitString,
                                                             const std::size_t elementIndex,
                                                             bool trimWhitespace)
{
    auto resultVector = StringUtility::SplitStringAt(inputString, splitString, trimWhitespace);
    if (elementIndex >= resultVector.size())
    {
        KL_THROW("Could not get element " << KL_WHITE_BOLD << elementIndex << KL_NORMAL
                                          << " after splitting string because vector "
                                          << "was of size " << KL_WHITE_BOLD
                                          << resultVector.size());
    }

    return resultVector.at(elementIndex);
}

//--------------------------------------------------------------------------------------------------

inline StringVector StringUtility::SplitStringAfter(const std::string_view &inputString,
                                                    const char splitChar,
                                                    const bool trimWhitespace) noexcept
{
    auto resultVector = StringVector{};

    // Every time we reach the split character, split the string (keeping the split character in the
    // previous string).
    auto item = std::string{};
    for (auto i = SIZE_T(0UL); i < inputString.length(); ++i)
    {
        if (inputString.at(i) == splitChar)
        {
            item += inputString.at(i);

            if (trimWhitespace) TrimWhitespace(item);

            resultVector.push_back(item);
            item.clear();
        }
        else
            item += inputString.at(i);
    }

    // If the item string is empty, don't add it to the result vector (by convention).
    if (trimWhitespace) TrimWhitespace(item);

    if (!item.empty()) resultVector.push_back(item);

    return resultVector;
}

//--------------------------------------------------------------------------------------------------

inline std::string StringUtility::SplitStringAfterAndGetElement(const std::string_view &inputString,
                                                                const char &splitChar,
                                                                const std::size_t elementIndex,
                                                                const bool trimWhitespace)
{
    auto resultVector = StringUtility::SplitStringAfter(inputString, splitChar, trimWhitespace);
    if (elementIndex >= resultVector.size())
    {
        KL_THROW("Could not get element " << KL_WHITE_BOLD << elementIndex << KL_NORMAL
                                          << " after splitting string because vector "
                                          << "was of size " << KL_WHITE_BOLD
                                          << resultVector.size());
    }

    return resultVector.at(elementIndex);
}

//--------------------------------------------------------------------------------------------------

inline StringVector StringUtility::SplitStringAfter(const std::string_view &inputString,
                                                    const std::string_view &splitString,
                                                    const bool trimWhitespace) noexcept
{
    auto resultVector = StringVector{};

    // Every time we reach the split character, split the string (keeping the split character in the
    // previous string).
    auto item = std::string{};
    for (auto i = SIZE_T(0UL); inputString.length() < splitString.length()
                                   ? false
                                   : i < SIZE_T(1UL) + inputString.length() - splitString.length();
         ++i)
    {
        if (inputString.substr(i, splitString.length()) == splitString)
        {
            item += splitString;

            if (trimWhitespace) TrimWhitespace(item);

            resultVector.push_back(item);
            item.clear();
            i += splitString.length() - SIZE_T(1UL);
        }
        else
            item += inputString.at(i);
    }

    // If the item string is empty, don't add it to the result vector (by convention).
    if (trimWhitespace) TrimWhitespace(item);

    if (!item.empty()) resultVector.push_back(item);

    return resultVector;
}

//--------------------------------------------------------------------------------------------------

inline std::string StringUtility::SplitStringAfterAndGetElement(const std::string_view &inputString,
                                                                const std::string_view &splitString,
                                                                const std::size_t elementIndex,
                                                                const bool trimWhitespace)
{
    auto resultVector = StringUtility::SplitStringAfter(inputString, splitString, trimWhitespace);
    if (elementIndex >= resultVector.size())
    {
        KL_THROW("Could not get element " << KL_WHITE_BOLD << elementIndex << KL_NORMAL
                                          << " after splitting string because vector "
                                          << "was of size " << KL_WHITE_BOLD
                                          << resultVector.size());
    }

    return resultVector.at(elementIndex);
}

//--------------------------------------------------------------------------------------------------

inline std::string StringUtility::StripQuotationMarks(const std::string &inputString) noexcept
{
    auto inputStringLength = inputString.length();
    if (inputStringLength < SIZE_T(2UL)) return inputString;

    if (inputString.front() == '"' && inputString.back() == '"')
        return inputString.substr(SIZE_T(1UL), inputStringLength - SIZE_T(2UL));

    return inputString;
}

//--------------------------------------------------------------------------------------------------

inline bool StringUtility::DoesStringStartWith(const std::string_view &inputString,
                                               const std::string_view &startString) noexcept
{
    if (inputString.size() >= startString.size())
    {
        if (inputString.substr(SIZE_T(0UL), startString.size()) == startString) return true;
    }

    return false;
}
}  // namespace kl

#endif  // #ifndef KL_STRING_UTILITY_H