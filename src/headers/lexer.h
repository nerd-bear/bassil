#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "utils.h"
#include <fstream>

// Enumeration for Token types
typedef enum {
    TK_Identifier,
    TK_Argument,
    TK_Flag,
    TK_String,
    TK_Semicolon
} TokenKind;

// Token structure using a union
typedef struct {
    TokenKind type;
    union {
        char *as_string;
        char *as_identifier;
    };
} Token;

// Function declarations (prototypes)
void parse_string(size_t *pos, const std::string &str);
void parse_flag(size_t *pos, const std::string &str);
void parse_command(size_t *pos, const std::string &str);
void parse_argument(size_t *pos, const std::string &str);
void display_tokens(const std::vector<Token>& command);
void lex(const std::string &inputString);

// Declare logBool as external to avoid multiple definitions
extern bool logBool;

#endif // LEXER_H