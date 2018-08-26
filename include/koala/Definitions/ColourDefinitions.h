/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Definitions/ColourDefinitions.h
 *
 * @brief Header file for koala colour definitions.
 */

#ifndef KL_COLOUR_DEFINITIONS_H
#define KL_COLOUR_DEFINITIONS_H 1

/**
 * @brief Macro for initializing a string.
 *
 * @param x The thing with which to initialize the string.
 */
#define KL_PRINT_AS_STRING(x) \
    std::string { x }

//--------------------------------------------------------------------------------------------------

#ifdef KL_NO_COLOURS

/**
 * @brief String definition for printing normal text to console.
 */
#define KL_NORMAL ""

/**
 * @brief String definition for printing bold text to console.
 */
#define KL_BOLD ""

/**
 * @brief String definition for printing underlined text to console.
 */
#define KL_UNDERLINED ""

/**
 * @brief String definition for printing red text to console.
 */
#define KL_RED ""

/**
 * @brief String definition for printing green text to console.
 */
#define KL_GREEN ""

/**
 * @brief String definition for printing yellow text to console.
 */
#define KL_YELLOW ""

/**
 * @brief String definition for printing blue text to console.
 */
#define KL_BLUE ""

/**
 * @brief String definition for printing magenta text to console.
 */
#define KL_MAGENTA ""

/**
 * @brief String definition for printing cyan text to console.
 */
#define KL_CYAN ""

/**
 * @brief String definition for printing white text to console.
 */
#define KL_WHITE ""

/**
 * @brief String definition for printing light grey text to console.
 */
#define KL_LIGHT_GREY ""

/**
 * @brief String definition for printing dark grey text to console.
 */
#define KL_DARK_GREY ""

/**
 * @brief String definition for printing bold, red text to console.
 */
#define KL_RED_BOLD ""

/**
 * @brief String definition for printing bold, green text to console.
 */
#define KL_GREEN_BOLD ""

/**
 * @brief String definition for printing bold, yellow text to console.
 */
#define KL_YELLOW_BOLD ""

/**
 * @brief String definition for printing bold, blue text to console.
 */
#define KL_BLUE_BOLD ""

/**
 * @brief String definition for printing bold, magenta text to console.
 */
#define KL_MAGENTA_BOLD ""

/**
 * @brief String definition for printing bold, cyan text to console.
 */
#define KL_CYAN_BOLD ""

/**
 * @brief String definition for printing bold, white text to console.
 */
#define KL_WHITE_BOLD ""

/**
 * @brief String definition for printing bold, light grey text to console.
 */
#define KL_LIGHT_GREY_BOLD ""

/**
 * @brief String definition for printing bold, dark grey text to console.
 */
#define KL_DARK_GREY_BOLD ""

/**
 * @brief String definition for printing underlined, red text to console.
 */
#define KL_RED_UNDERLINED ""

/**
 * @brief String definition for printing underlined, green text to console.
 */
#define KL_GREEN_UNDERLINED ""

/**
 * @brief String definition for printing underlined, yellow text to console.
 */
#define KL_YELLOW_UNDERLINED ""

/**
 * @brief String definition for printing underlined, blue text to console.
 */
#define KL_BLUE_UNDERLINED ""

/**
 * @brief String definition for printing underlined, magenta text to console.
 */
#define KL_MAGENTA_UNDERLINED ""

/**
 * @brief String definition for printing underlined, cyan text to console.
 */
#define KL_CYAN_UNDERLINED ""

/**
 * @brief String definition for printing underlined, white text to console.
 */
#define KL_WHITE_UNDERLINED ""

/**
 * @brief String definition for printing underlined, light grey text to console.
 */
#define KL_LIGHT_GREY_UNDERLINED ""

/**
 * @brief String definition for printing underlined, dark grey text to console.
 */
#define KL_DARK_GREY_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, red text to console.
 */
#define KL_RED_BOLD_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, green text to console.
 */
#define KL_GREEN_BOLD_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, yellow text to console.
 */
#define KL_YELLOW_BOLD_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, blue text to console.
 */
#define KL_BLUE_BOLD_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, magenta text to console.
 */
#define KL_MAGENTA_BOLD_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, cyan text to console.
 */
#define KL_CYAN_BOLD_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, white text to console.
 */
#define KL_WHITE_BOLD_UNDERLINED ""

/**
 * @brief String definition for printing bold, underlined, light grey text to console.
 */
#define KL_LIGHT_GREY_BOLD_UNDERLINED ""

//--------------------------------------------------------------------------------------------------

#else

/**
 * @brief String definition for printing normal text to console.
 */
#define KL_NORMAL "\033[0m"

/**
 * @brief String definition for printing bold text to console.
 */
#define KL_BOLD "\033[1m"

/**
 * @brief String definition for printing underlined text to console.
 */
#define KL_UNDERLINED "\033[4m"

/**
 * @brief String definition for printing red text to console.
 */
#define KL_RED "\033[0;31m"

/**
 * @brief String definition for printing green text to console.
 */
#define KL_GREEN "\033[0;32m"

/**
 * @brief String definition for printing yellow text to console.
 */
#define KL_YELLOW "\033[0;33m"

/**
 * @brief String definition for printing blue text to console.
 */
#define KL_BLUE "\033[0;34m"

/**
 * @brief String definition for printing magenta text to console.
 */
#define KL_MAGENTA "\033[0;35m"

/**
 * @brief String definition for printing cyan text to console.
 */
#define KL_CYAN "\033[0;36m"

/**
 * @brief String definition for printing white text to console.
 */
#define KL_WHITE "\033[0;37m"

/**
 * @brief String definition for printing light grey text to console.
 */
#define KL_LIGHT_GREY "\033[0;37m"

/**
 * @brief String definition for printing dark grey text to console.
 */
#define KL_DARK_GREY "\033[0;90m"

/**
 * @brief String definition for printing bold, red text to console.
 */
#define KL_RED_BOLD "\033[1;31m"

/**
 * @brief String definition for printing bold, green text to console.
 */
#define KL_GREEN_BOLD "\033[1;32m"

/**
 * @brief String definition for printing bold, yellow text to console.
 */
#define KL_YELLOW_BOLD "\033[1;33m"

/**
 * @brief String definition for printing bold, blue text to console.
 */
#define KL_BLUE_BOLD "\033[1;34m"

/**
 * @brief String definition for printing bold, magenta text to console.
 */
#define KL_MAGENTA_BOLD "\033[1;35m"

/**
 * @brief String definition for printing bold, cyan text to console.
 */
#define KL_CYAN_BOLD "\033[1;36m"

/**
 * @brief String definition for printing bold, white text to console.
 */
#define KL_WHITE_BOLD "\033[1;37m"

/**
 * @brief String definition for printing bold, light grey text to console.
 */
#define KL_LIGHT_GREY_BOLD "\033[1;37m"

/**
 * @brief String definition for printing bold, dark grey text to console.
 */
#define KL_DARK_GREY_BOLD "\033[1;90m"

/**
 * @brief String definition for printing underlined, red text to console.
 */
#define KL_RED_UNDERLINED "\033[4;31m"

/**
 * @brief String definition for printing underlined, green text to console.
 */
#define KL_GREEN_UNDERLINED "\033[4;32m"

/**
 * @brief String definition for printing underlined, yellow text to console.
 */
#define KL_YELLOW_UNDERLINED "\033[4;33m"

/**
 * @brief String definition for printing underlined, blue text to console.
 */
#define KL_BLUE_UNDERLINED "\033[4;34m"

/**
 * @brief String definition for printing underlined, magenta text to console.
 */
#define KL_MAGENTA_UNDERLINED "\033[4;35m"

/**
 * @brief String definition for printing underlined, cyan text to console.
 */
#define KL_CYAN_UNDERLINED "\033[4;36m"

/**
 * @brief String definition for printing underlined, white text to console.
 */
#define KL_WHITE_UNDERLINED "\033[4;37m"

/**
 * @brief String definition for printing underlined, light grey text to console.
 */
#define KL_LIGHT_GREY_UNDERLINED "\033[4;37m"

/**
 * @brief String definition for printing underlined, dark grey text to console.
 */
#define KL_DARK_GREY_UNDERLINED "\033[4;90m"

/**
 * @brief String definition for printing bold, underlined, red text to console.
 */
#define KL_RED_BOLD_UNDERLINED "\033[4;1;31m"

/**
 * @brief String definition for printing bold, underlined, green text to console.
 */
#define KL_GREEN_BOLD_UNDERLINED "\033[4;1;32m"

/**
 * @brief String definition for printing bold, underlined, yellow text to console.
 */
#define KL_YELLOW_BOLD_UNDERLINED "\033[4;1;33m"

/**
 * @brief String definition for printing bold, underlined, blue text to console.
 */
#define KL_BLUE_BOLD_UNDERLINED "\033[4;1;34m"

/**
 * @brief String definition for printing bold, underlined, magenta text to console.
 */
#define KL_MAGENTA_BOLD_UNDERLINED "\033[4;1;35m"

/**
 * @brief String definition for printing bold, underlined, cyan text to console.
 */
#define KL_CYAN_BOLD_UNDERLINED "\033[4;1;36m"

/**
 * @brief String definition for printing bold, underlined, white text to console.
 */
#define KL_WHITE_BOLD_UNDERLINED "\033[4;1;37m"

/**
 * @brief String definition for printing bold, underlined, light grey text to console.
 */
#define KL_LIGHT_GREY_BOLD_UNDERLINED "\033[4;1;37m"

/**
 * @brief String definition for printing bold, underlined, dark grey text to console.
 */
#define KL_DARK_GREY_BOLD_UNDERLINED "\033[4;1;90m"

#endif  // #ifdef KL_NO_COLOURS

//--------------------------------------------------------------------------------------------------

/**
 * @brief String definition for printing normal text to console.
 */
#define KL_NORMAL_STR KL_PRINT_AS_STRING(KL_NORMAL)

/**
 * @brief String definition for printing bold text to console.
 */
#define KL_BOLD_STR KL_PRINT_AS_STRING(KL_BOLD)

/**
 * @brief String definition for printing underlined text to console.
 */
#define KL_UNDERLINED_STR KL_PRINT_AS_STRING(KL_UNDERLINED)

/**
 * @brief String definition for printing red text to console.
 */
#define KL_RED_STR KL_PRINT_AS_STRING(KL_RED)

/**
 * @brief String definition for printing green text to console.
 */
#define KL_GREEN_STR KL_PRINT_AS_STRING(KL_GREEN)

/**
 * @brief String definition for printing yellow text to console.
 */
#define KL_YELLOW_STR KL_PRINT_AS_STRING(KL_YELLOW)

/**
 * @brief String definition for printing blue text to console.
 */
#define KL_BLUE_STR KL_PRINT_AS_STRING(KL_BLUE)

/**
 * @brief String definition for printing magenta text to console.
 */
#define KL_MAGENTA_STR KL_PRINT_AS_STRING(KL_MAGENTA)

/**
 * @brief String definition for printing cyan text to console.
 */
#define KL_CYAN_STR KL_PRINT_AS_STRING(KL_CYAN)

/**
 * @brief String definition for printing white text to console.
 */
#define KL_WHITE_STR KL_PRINT_AS_STRING(KL_WHITE)

/**
 * @brief String definition for printing light grey text to console.
 */
#define KL_LIGHT_GREY_STR KL_PRINT_AS_STRING(KL_LIGHT_GREY)

/**
 * @brief String definition for printing dark grey text to console.
 */
#define KL_DARK_GREY_STR KL_PRINT_AS_STRING(KL_DARK_GREY)

/**
 * @brief String definition for printing bold, red text to console.
 */
#define KL_RED_BOLD_STR KL_PRINT_AS_STRING(KL_RED_BOLD)

/**
 * @brief String definition for printing bold, green text to console.
 */
#define KL_GREEN_BOLD_STR KL_PRINT_AS_STRING(KL_GREEN_BOLD)

/**
 * @brief String definition for printing bold, yellow text to console.
 */
#define KL_YELLOW_BOLD_STR KL_PRINT_AS_STRING(KL_YELLOW_BOLD)

/**
 * @brief String definition for printing bold, blue text to console.
 */
#define KL_BLUE_BOLD_STR KL_PRINT_AS_STRING(KL_BLUE_BOLD)

/**
 * @brief String definition for printing bold, magenta text to console.
 */
#define KL_MAGENTA_BOLD_STR KL_PRINT_AS_STRING(KL_MAGENTA_BOLD)

/**
 * @brief String definition for printing bold, cyan text to console.
 */
#define KL_CYAN_BOLD_STR KL_PRINT_AS_STRING(KL_CYAN_BOLD)

/**
 * @brief String definition for printing bold, white text to console.
 */
#define KL_WHITE_BOLD_STR KL_PRINT_AS_STRING(KL_WHITE_BOLD)

/**
 * @brief String definition for printing bold, light grey text to console.
 */
#define KL_LIGHT_GREY_BOLD_STR KL_PRINT_AS_STRING(KL_LIGHT_GREY_BOLD)

/**
 * @brief String definition for printing bold, dark grey text to console.
 */
#define KL_DARK_GREY_BOLD_STR KL_PRINT_AS_STRING(KL_DARK_GREY_BOLD)

/**
 * @brief String definition for printing underlined, red text to console.
 */
#define KL_RED_UNDERLINED_STR KL_PRINT_AS_STRING(KL_RED_UNDERLINED)

/**
 * @brief String definition for printing underlined, green text to console.
 */
#define KL_GREEN_UNDERLINED_STR KL_PRINT_AS_STRING(KL_GREEN_UNDERLINED)

/**
 * @brief String definition for printing underlined, yellow text to console.
 */
#define KL_YELLOW_UNDERLINED_STR KL_PRINT_AS_STRING(KL_YELLOW_UNDERLINED)

/**
 * @brief String definition for printing underlined, blue text to console.
 */
#define KL_BLUE_UNDERLINED_STR KL_PRINT_AS_STRING(KL_BLUE_UNDERLINED)

/**
 * @brief String definition for printing underlined, magenta text to console.
 */
#define KL_MAGENTA_UNDERLINED_STR KL_PRINT_AS_STRING(KL_MAGENTA_UNDERLINED)

/**
 * @brief String definition for printing underlined, cyan text to console.
 */
#define KL_CYAN_UNDERLINED_STR KL_PRINT_AS_STRING(KL_CYAN_UNDERLINED)

/**
 * @brief String definition for printing underlined, white text to console.
 */
#define KL_WHITE_UNDERLINED_STR KL_PRINT_AS_STRING(KL_WHITE_UNDERLINED)

/**
 * @brief String definition for printing underlined, light grey text to console.
 */
#define KL_LIGHT_GREY_UNDERLINED_STR KL_PRINT_AS_STRING(KL_LIGHT_GREY_UNDERLINED)

/**
 * @brief String definition for printing underlined, dark grey text to console.
 */
#define KL_DARK_GREY_UNDERLINED_STR KL_PRINT_AS_STRING(KL_DARK_GREY_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, red text to console.
 */
#define KL_RED_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_RED_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, green text to console.
 */
#define KL_GREEN_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_GREEN_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, yellow text to console.
 */
#define KL_YELLOW_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_YELLOW_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, blue text to console.
 */
#define KL_BLUE_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_BLUE_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, magenta text to console.
 */
#define KL_MAGENTA_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_MAGENTA_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, cyan text to console.
 */
#define KL_CYAN_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_CYAN_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, white text to console.
 */
#define KL_WHITE_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_WHITE_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, light grey text to console.
 */
#define KL_LIGHT_GREY_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_LIGHT_GREY_BOLD_UNDERLINED)

/**
 * @brief String definition for printing bold, underlined, dark grey text to console.
 */
#define KL_DARK_GREY_BOLD_UNDERLINED_STR KL_PRINT_AS_STRING(KL_DARK_GREY_BOLD_UNDERLINED)

#endif  // #ifndef KL_COLOUR_DEFINITIONS_H