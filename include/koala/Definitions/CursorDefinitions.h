/// @cond
/**
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
 * source code package.
 */
/// @endcond

/**
 * @file koala/include/koala/Definitions/CursorDefinitions.h
 *
 * @brief Header file for koala cursor definitions.
 */

#ifndef KL_CURSOR_DEFINITIONS_H
#define KL_CURSOR_DEFINITIONS_H 1

/**
 * @brief Escape sequence for moving the cursor up.
 *
 * @param n The number of rows to move.
 */
#define KL_UP(n) "\033[" #n "A"

/**
 * @brief Escape sequence for moving the cursor up.
 *
 * @param n The number of rows to move.
 */
#define KL_UP_STR(n) \
    std::string{"\033["} + std::to_string(n) + std::string { "A" }

/**
 * @brief Escape sequence for moving the cursor down.
 *
 * @param n The number of rows to move.
 */
#define KL_DOWN(n) "\033[" #n "B"

/**
 * @brief Escape sequence for moving the cursor down.
 *
 * @param n The number of rows to move.
 */
#define KL_DOWN_STR(n) \
    std::string{"\033["} + std::to_string(n) + std::string { "B" }

/**
 * @brief Escape sequence for moving the cursor forwards.
 *
 * @param n The number of columns to move.
 */
#define KL_FORWARDS(n) "\033[" #n "C"

/**
 * @brief Escape sequence for moving the cursor forwards.
 *
 * @param n The number of columns to move.
 */
#define KL_FORWARDS_STR(n) \
    std::string{"\033["} + std::to_string(n) + std::string { "C" }

/**
 * @brief Escape sequence for moving the cursor backwards.
 *
 * @param n The number of columns to move.
 */
#define KL_BACKWARDS(n) "\033[" #n "D"

/**
 * @brief Escape sequence for moving the cursor backwards.
 *
 * @param n The number of columns to move.
 */
#define KL_BACKWARDS_STR(n) \
    std::string{"\033["} + std::to_string(n) + std::string { "D" }

/**
 * @brief Escape sequence for clearing the line from the current cursor position to the end.
 */
#define KL_CLEAR "\033[K"

/**
 * @brief Escape sequence for clearing the line from the current cursor position to the end.
 */
#define KL_CLEAR_STR \
    std::string { "\033[K" }

/**
 * @brief Escape sequence for returning and clearing the line.
 */
#define KL_RTN_CLEAR "\r\033[K"

/**
 * @brief Escape sequence for returning and clearing the line.
 */
#define KL_RTN_CLEAR_STR \
    std::string { "\r\033[K" }

#endif  // #ifndef KL_CURSOR_DEFINITIONS_H