/**
 * @file lexer.cpp
 * @brief Implementation of the lexical analyzer for the Bassil language.
 */

#include "C:/coding-projects/CPP-Dev/bassil/src/headers/lexer.h"
#include <cctype>
#include <unordered_map>
#include <stdexcept>

bool logBool = true; // Define logBool here

/**
 * @brief Lexically analyze the input string and generate tokens
 * @param inputString The input string to be analyzed
 * @return Vector of tokens
 */
std::vector<Token> lex(const std::string &inputString)
{
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenKind> keywords = {
        {"int", TK_TypeInteger},
        {"char", TK_TypeChar},
        {"float", TK_TypeFloat},
        {"string", TK_TypeString}};
    std::unordered_map<std::string, TokenKind> operators = {
        {"+", TK_MathOperator},
        {"-", TK_MathOperator},
        {"*", TK_MathOperator},
        {"/", TK_MathOperator},
        {"%", TK_MathOperator},
        {"=", TK_EqualsSign},
        {"==", TK_ComparisonOperator},
        {"!=", TK_ComparisonOperator},
        {"<", TK_ComparisonOperator},
        {">", TK_ComparisonOperator},
        {"<=", TK_ComparisonOperator},
        {">=", TK_ComparisonOperator},
        {"&&", TK_LogicalOperator},
        {"||", TK_LogicalOperator},
        {"!", TK_LogicalOperator}};

    size_t pos = 0;
    int line = 1;
    int column = 1;

    auto addToken = [&](TokenKind type, const std::string &value, int startColumn)
    {
        tokens.push_back({type, value, line, startColumn, column - 1});
    };

    while (pos < inputString.length())
    {
        char currentChar = inputString[pos];

        // Skip whitespace
        if (std::isspace(currentChar))
        {
            if (currentChar == '\n')
            {
                line++;
                column = 1;
            }
            else
            {
                column++;
            }
            pos++;
            continue;
        }

        // Identifiers and keywords
        if (isIdentifierStart(currentChar))
        {
            size_t start = pos;
            int startColumn = column;
            while (pos < inputString.length() && isIdentifierContinuation(inputString[pos]))
            {
                pos++;
                column++;
            }
            std::string identifier = inputString.substr(start, pos - start);
            auto it = keywords.find(identifier);
            if (it != keywords.end())
            {
                addToken(it->second, identifier, startColumn);
            }
            else
            {
                addToken(TK_Identifier, identifier, startColumn);
            }
            continue;
        }

        // Numbers (integers and floats)
        if (std::isdigit(currentChar))
        {
            size_t start = pos;
            int startColumn = column;
            bool isFloat = false;
            while (pos < inputString.length() && (std::isdigit(inputString[pos]) || inputString[pos] == '.'))
            {
                if (inputString[pos] == '.')
                {
                    if (isFloat)
                    {
                        Utils::general_log("Error: Multiple decimal points in number at line " + std::to_string(line) + ", column " + std::to_string(column), logBool);
                        break;
                    }
                    isFloat = true;
                }
                pos++;
                column++;
            }
            std::string number = inputString.substr(start, pos - start);
            addToken(isFloat ? TK_Float : TK_Integer, number, startColumn);
            continue;
        }

        // Strings
        if (currentChar == '"')
        {
            size_t start = pos;
            int startColumn = column;
            pos++;
            column++;
            while (pos < inputString.length() && inputString[pos] != '"')
            {
                if (inputString[pos] == '\\' && pos + 1 < inputString.length())
                {
                    pos += 2;
                    column += 2;
                }
                else
                {
                    pos++;
                    column++;
                }
            }
            if (pos >= inputString.length())
            {
                Utils::general_log("Error: Unterminated string at line " + std::to_string(line) + ", column " + std::to_string(startColumn), logBool);
                break;
            }
            pos++;
            column++;
            std::string str = inputString.substr(start, pos - start);
            addToken(TK_String, str, startColumn);
            continue;
        }

        // Operators and punctuation
        std::string op(1, currentChar);
        if (pos + 1 < inputString.length())
        {
            std::string doubleOp = op + inputString[pos + 1];
            auto it = operators.find(doubleOp);
            if (it != operators.end())
            {
                addToken(it->second, doubleOp, column);
                pos += 2;
                column += 2;
                continue;
            }
        }
        auto it = operators.find(op);
        if (it != operators.end())
        {
            addToken(it->second, op, column);
            pos++;
            column++;
            continue;
        }

        // Other single-character tokens
        switch (currentChar)
        {
        case ';':
            addToken(TK_Semicolon, ";", column);
            break;
        case '(':
            addToken(TK_OpenParen, "(", column);
            break;
        case ')':
            addToken(TK_CloseParen, ")", column);
            break;
        case '{':
            addToken(TK_OpenBrace, "{", column);
            break;
        case '}':
            addToken(TK_CloseBrace, "}", column);
            break;
        case ',':
            addToken(TK_Comma, ",", column);
            break;
        default:
            Utils::general_log("Error: Unknown character '" + std::string(1, currentChar) + "' at line " + std::to_string(line) + ", column " + std::to_string(column), logBool);
            addToken(TK_Unknown, std::string(1, currentChar), column);
        }
        pos++;
        column++;
    }

    return tokens;
}

/**
 * @brief Display the generated tokens
 * @param tokens Vector of tokens to be displayed
 */
void display_tokens(const std::vector<Token> &tokens)
{
    Utils::general_log("[display_tokens] Displaying tokens:", logBool);
    for (const auto &token : tokens)
    {
        std::string tokenType;
        switch (token.type)
        {
        case TK_Identifier:
            tokenType = "Identifier";
            break;
        case TK_Argument:
            tokenType = "Argument";
            break;
        case TK_String:
            tokenType = "String";
            break;
        case TK_Semicolon:
            tokenType = "Semicolon";
            break;
        case TK_Integer:
            tokenType = "Integer";
            break;
        case TK_Float:
            tokenType = "Float";
            break;
        case TK_MathOperator:
            tokenType = "MathOperator";
            break;
        case TK_EqualsSign:
            tokenType = "EqualsSign";
            break;
        case TK_TypeInteger:
            tokenType = "TypeInteger";
            break;
        case TK_TypeChar:
            tokenType = "TypeChar";
            break;
        case TK_TypeFloat:
            tokenType = "TypeFloat";
            break;
        case TK_TypeString:
            tokenType = "TypeString";
            break;
        case TK_OpenParen:
            tokenType = "OpenParen";
            break;
        case TK_CloseParen:
            tokenType = "CloseParen";
            break;
        case TK_OpenBrace:
            tokenType = "OpenBrace";
            break;
        case TK_CloseBrace:
            tokenType = "CloseBrace";
            break;
        case TK_Comma:
            tokenType = "Comma";
            break;
        case TK_LogicalOperator:
            tokenType = "LogicalOperator";
            break;
        case TK_ComparisonOperator:
            tokenType = "ComparisonOperator";
            break;
        case TK_Unknown:
            tokenType = "Unknown";
            break;
        }
        Utils::general_log("Token at line " + std::to_string(token.line) +
                               ", columns " + std::to_string(token.start_column) +
                               "-" + std::to_string(token.end_column) + ": " +
                               tokenType + ": " + token.value,
                           logBool);
    }
}

/**
 * @brief Save the generated tokens to a file
 * @param tokens Vector of tokens to be saved
 */
void save_tokens(const std::vector<Token> &tokens, const std::string &filename)
{
    Utils::general_log("[save_tokens] Saving tokens:", logBool);
    std::ofstream outputFile(filename, std::ios::app);
    if (!outputFile.is_open())
    {
        Utils::general_log("[save_tokens] Failed to open file.", logBool);
        return;
    }
    Utils::general_log("[save_tokens] Successfully opened file.", logBool);
    outputFile << "[\n";
    for (const auto &token : tokens)
    {
        Utils::general_log("[save_tokens] Writing token to file:", logBool);
        outputFile << "  {\n";
        outputFile << "    \"line\": " << token.line << ",\n";
        outputFile << "    \"start_column\": " << token.start_column << ",\n";
        outputFile << "    \"end_column\": " << token.end_column << ",\n";
        outputFile << "    \"type\": \"" << static_cast<int>(token.type) << "\",\n";
        outputFile << "    \"value\": \"" << token.value << "\"\n";
        outputFile << "  },\n";
        Utils::general_log("[save_tokens] Finished writing token to file.", logBool);
    }
    outputFile << "]\n";
    outputFile.close();
    Utils::general_log("[save_tokens] Successfully closed the file.", logBool);
}