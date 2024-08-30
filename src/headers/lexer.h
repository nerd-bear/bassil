/**
 * @file lexer.h
 * @brief Header file for the lexical analyzer (lexer) of the Bassil language.
 */

#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"
#include <fstream>

/**
 * @brief Enumeration for Token types
 */
typedef enum {
    TK_Identifier,    ///< Identifiers (variable names, function names, etc.)
    TK_Argument,      ///< Command arguments
    TK_String,        ///< String literals
    TK_Semicolon,     ///< Semicolon (;)
    TK_Integer,       ///< Integer literals
    TK_Float,         ///< Floating-point literals
    TK_MathOperator,  ///< Mathematical operators (+, -, *, /, %)
    TK_EqualsSign,    ///< Equals sign (=)
    TK_TypeInteger,   ///< Integer type keyword
    TK_TypeChar,      ///< Character type keyword
    TK_TypeFloat,     ///< Float type keyword
    TK_TypeString,    ///< String type keyword
    TK_OpenParen,     ///< Opening parenthesis (
    TK_CloseParen,    ///< Closing parenthesis )
    TK_OpenBrace,     ///< Opening brace {
    TK_CloseBrace,    ///< Closing brace }
    TK_Comma,         ///< Comma (,)
    TK_LogicalOperator, ///< Logical operators (&&, ||, !)
    TK_ComparisonOperator, ///< Comparison operators (==, !=, <, >, <=, >=)
    TK_Unknown        ///< Unknown token type
} TokenKind;

/**
 * @brief Structure to represent a token
 */
typedef struct {
    TokenKind type;   ///< Type of the token
    std::string value; ///< Value of the token
    int line;         ///< Line number where the token is found
    int start_column; ///< Start column of the token
    int end_column;   ///< End column of the token
} Token;

extern bool logBool;  ///< Global flag to control logging

/**
 * @brief Check if a character is a valid identifier start.
 * @param c The character to check.
 * @return bool True if the character is a valid identifier start, false otherwise.
 */
inline bool isIdentifierStart(char c) {
    return std::isalpha(c) || c == '_';
}

/**
 * @brief Check if a character is a valid identifier continuation.
 * @param c The character to check.
 * @return bool True if the character is a valid identifier continuation, false otherwise.
 */
inline bool isIdentifierContinuation(char c) {
    return std::isalnum(c) || c == '_';
}

/**
 * @brief Lexically analyze the input string and generate tokens
 * @param inputString The input string to be analyzed
 * @return Vector of tokens
 */
std::vector<Token> lex(const std::string& inputString);

/**
 * @brief Display the generated tokens
 * @param tokens Vector of tokens to be displayed
 */
void display_tokens(const std::vector<Token>& tokens);

/**
 * @brief Save the generated tokens to a file
 * @param tokens Vector of tokens to be saved
 */
void save_tokens(const std::vector<Token>& tokens);

#endif // LEXER_H