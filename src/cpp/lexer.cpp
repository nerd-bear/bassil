#include "C:/coding-projects/CPP-Dev/bassil/src/headers/lexer.h"

logBool = true; // Whether to log or not 

void parse_string(size_t *pos, const std::string &str)
{
    Utils::general_log("[parse_string] Starting parse at position: " + std::to_string(*pos), logBool);
    for (; (*pos < str.size() && str[*pos] != '"'); (*pos)++)
    {
        if (str[*pos] == '\\')
        {
            (*pos)++;
        }
    }

    if (*pos >= str.size() || str[*pos] != '"')
    {
        Utils::general_log("[parse_string] Error: Expected closing double quote at position: " + std::to_string(*pos), logBool);
        exit(1);
    }

    Utils::general_log("[parse_string] Finished parsing string. Ending position: " + std::to_string(*pos), logBool);
}

void parse_identefier(size_t *pos, const std::string &str)
{
    size_t start = *pos;
    while (*pos < str.size() && str[*pos] != ' ' && str[*pos] != '\n' && str[*pos] != ';')
    {
        (*pos)++;
    }
}

void parse_argument(size_t *pos, const std::string &str)
{
    while (*pos < str.size() && str[*pos] != ' ' && str[*pos] != '\n' && str[*pos] != ';')
    {
        (*pos)++;
    }
}

void parse_integer(size_t *pos, const std::string &str)
{
    size_t start = *pos;
    while (*pos < str.size() && isdigit(str[*pos]))
    {
        (*pos)++;
    }
}

void display_tokens(const std::vector<Token> &tokens)
{
    Utils::general_log("[display_tokens] Displaying tokens:", logBool);
    for (const auto &token : tokens)
    {
        std::string logMessage = "Token at line " + std::to_string(token.line) +
                                 ", columns " + std::to_string(token.start_column) +
                                 "-" + std::to_string(token.end_column) + ": ";
        switch (token.type)
        {
        case TK_Identifier:
            logMessage += "Identifier: " + std::string(token.as_identifier);
            break;
        case TK_Argument:
            logMessage += "Argument: " + std::string(token.as_identifier);
            break;
        case TK_String:
            logMessage += "String: " + std::string(token.as_string);
            break;
        case TK_Integer:
            logMessage += "Integer: " + std::string(token.as_string);
            break;
        case TK_Semicolon:
            logMessage += "Semicolon";
            break;
        default:
            logMessage += "Unknown Token Type";
            break;
        }
        Utils::general_log(logMessage, logBool);
    }
}

void save_tokens(const std::vector<Token> &tokens)
{
    Utils::general_log("[save_tokens] Saving tokens:", logBool);
    std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/after_lex.txt", std::ios::app);
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
        switch (token.type)
        {
        case TK_Identifier:
            outputFile << "    \"type\": \"Identifier\",\n";
            outputFile << "    \"value\": \"" << token.as_identifier << "\"\n";
            break;
        case TK_Argument:
            outputFile << "    \"type\": \"Argument\",\n";
            outputFile << "    \"value\": \"" << token.as_identifier << "\"\n";
            break;
        case TK_String:
            outputFile << "    \"type\": \"String\",\n";
            outputFile << "    \"value\": \"" << token.as_string << "\"\n";
            break;
        case TK_Integer:
            outputFile << "    \"type\": \"Integer\",\n";
            outputFile << "    \"value\": \"" << token.as_string << "\"\n";
            break;
        case TK_Semicolon:
            outputFile << "    \"type\": \"Semicolon\"\n";
            break;
        default:
            continue;
        }
        outputFile << "  },\n";
        Utils::general_log("[save_tokens] Finished writing token to file.", logBool);
    }
    outputFile << "]\n";
    outputFile.close();
    Utils::general_log("[save_tokens] Successfully closed the file.", logBool);
}

std::vector<Token> lex(std::string inputString)
{
    size_t pos = 0;
    int line = 1;
    int column = 1;
    std::vector<Token> lexerOut; // Accumulate all tokens here

    Utils::general_log("[lex] Starting lexing process for input: \"" + inputString + "\" at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);

    while (pos < inputString.length())
    {
        Utils::general_log("[lex] Processing character at pos: " + std::to_string(pos) + " character: " + std::string(1, inputString[pos]), logBool);

        size_t start = pos;
        int start_column = column;

        // Handle Semicolon - This marks the end of a command
        if (inputString[pos] == ';')
        {
            Utils::general_log("[lex] Detected semicolon at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);
            std::string TheSemicolon = inputString.substr(pos, 1);
            Token token = {TK_Semicolon, .as_string = strdup(TheSemicolon.c_str()), line, column, column};
            lexerOut.push_back(token);

            pos++;
            column++;
            continue;
        }

        // Skip over any whitespace or newline characters
        if (isspace(inputString[pos]))
        {
            if (inputString[pos] == '\n')
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

        // Detect Command or Integer
        if (isdigit(inputString[pos]))
        {
            parse_integer(&pos, inputString);
            std::string TheInteger = inputString.substr(start, pos - start);
            Token token = {TK_Integer, .as_string = strdup(TheInteger.c_str()), line, start_column, column + (int)(pos - start) - 1};
            lexerOut.push_back(token);
            column += (int)(pos - start);
        }
        else
        {
            parse_identefier(&pos, inputString);
            std::string commandName = inputString.substr(start, pos - start);
            if (!commandName.empty())
            {
                Token cmdToken = {TK_Identifier, .as_identifier = strdup(commandName.c_str()), line, start_column, column + (int)(pos - start) - 1};
                lexerOut.push_back(cmdToken);
            }
            column += (int)(pos - start);
        }

        // Handle Argument
        if (pos < inputString.size() && !isspace(inputString[pos]) && inputString[pos] != ';')
        {
            parse_argument(&pos, inputString);
            std::string TheArgument = inputString.substr(start, pos - start);
            if (!TheArgument.empty())
            {
                Token token = {TK_Argument, .as_string = strdup(TheArgument.c_str()), line, start_column, column + (int)(pos - start) - 1};
                lexerOut.push_back(token);
            }
            column += (int)(pos - start);
        }
    }

    // Display and save all tokens after processing the entire input
    Utils::general_log("[lex] Lexing process completed. Displaying and saving all tokens.", logBool);
    display_tokens(lexerOut);
    save_tokens(lexerOut);

    return lexerOut; // Return the list of all tokens
}