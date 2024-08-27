#include "C:/coding-projects/CPP-Dev/bassil/src/headers/lexer.h"

bool logBool = true;  // Whether to log or not

void parse_string(size_t *pos, const std::string &str) {
    Utils::general_log("[parse_string] Starting parse at position: " + std::to_string(*pos), logBool);
    for (; (*pos < str.size() && str[*pos] != '"'); (*pos)++) {
        if (str[*pos] == '\\') {
            (*pos)++;
        }
    }

    if (*pos >= str.size() || str[*pos] != '"') {
        Utils::general_log("[parse_string] Error: Expected closing double quote at position: " + std::to_string(*pos), logBool);
        exit(1);
    }

    Utils::general_log("[parse_string] Finished parsing string. Ending position: " + std::to_string(*pos), logBool);
}

void parse_flag(size_t *pos, const std::string &str) {
    size_t start = *pos;
    while (*pos < str.size() && str[*pos] != ' ' && str[*pos] != '\n' && str[*pos] != ';') {
        (*pos)++;
    }
}

void parse_command(size_t *pos, const std::string &str) {
    size_t start = *pos;
    while (*pos < str.size() && str[*pos] != ' ' && str[*pos] != '\n' && str[*pos] != ';') {
        (*pos)++;
    }
}

void parse_argument(size_t *pos, const std::string &str) {
    while (*pos < str.size() && str[*pos] != ' ' && str[*pos] != '\n' && str[*pos] != ';') {
        (*pos)++;
    }
}

void display_tokens(const std::vector<Token>& command) {
    Utils::general_log("[display_tokens] Displaying tokens:", logBool);
    for (const auto& token : command) {
        std::string logMessage = "Token at line " + std::to_string(token.line) +
                                 ", columns " + std::to_string(token.start_column) + 
                                 "-" + std::to_string(token.end_column) + ": ";
        switch (token.type) {
            case TK_Identifier:
                logMessage += "Identifier: " + std::string(token.as_identifier);
                break;
            case TK_Argument:
                logMessage += "Argument: " + std::string(token.as_identifier);
                break;
            case TK_String:
                logMessage += "String: " + std::string(token.as_string);
                break;
            case TK_Flag:
                logMessage += "Flag: " + std::string(token.as_string);
                break;
            default:
                logMessage += "Unknown Token Type";
                break;
        }
        Utils::general_log(logMessage, logBool);
    }
}

void save_command(const std::vector<Token>& command) {
    Utils::general_log("[save_command] Saving tokens:", logBool);
    std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/after_lex.txt", std::ios::app);

    if (!outputFile.is_open()) {
        Utils::general_log("[save_command] Failed to open file.", logBool);
        return;
    }

    outputFile << "[\n"; // Start of the list of tokens

    for (const auto& token : command) {
        outputFile << "  {\n"; // Start of an individual token

        outputFile << "    \"line\": " << token.line << ",\n";
        outputFile << "    \"start_column\": " << token.start_column << ",\n";
        outputFile << "    \"end_column\": " << token.end_column << ",\n";

        switch (token.type) {
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
            case TK_Flag:
                outputFile << "    \"type\": \"Flag\",\n";
                outputFile << "    \"value\": \"" << token.as_string << "\"\n";
                break;
            case TK_Semicolon:
                outputFile << "    \"type\": \"Semicolon\",\n";
                outputFile << "    \"value\": \"" << token.as_string << "\"\n";
                break;
            default:
                Utils::general_log("Unknown Token Type", logBool);
                continue; // Skip adding an unknown token to the file
        }

        outputFile << "  },\n"; // End of an individual token
    }

    outputFile << "]\n"; // End of the list of tokens

    outputFile.close();
}

void lex(const std::string &inputString) {
    bool commandFound = false;
    std::string commandName;
    size_t i = 0;
    int line = 1;
    int column = 1;
    std::vector<Token> command;

    Utils::general_log("[lex] Starting lexing process for input: \"" + inputString + "\" at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);

    while (i < inputString.size()) {
        size_t start = i;
        int start_column = column;
        std::string TheString, TheFlag, TheArgument, TheSemicolonToken;

        // Skip initial whitespace or newlines
        if (!commandFound && (inputString[i] == ' ' || inputString[i] == '\n')) {
            if (inputString[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }

        if (!commandFound) {
            // Detect command name (first token)
            parse_command(&i, inputString);
            commandName = inputString.substr(start, i - start);
            if (!commandName.empty()) {
                Token cmdToken = {TK_Identifier, .as_identifier = strdup(commandName.c_str()), line, start_column, column + (int)(i - start) - 1};
                command.push_back(cmdToken);
                commandFound = true;
            }
            column += (int)(i - start);
            continue;
        }

        // Skip any whitespace after command detection
        if (inputString[i] == ' ') {
            column++;
            i++;
            continue;
        }

        switch (inputString[i]) {
            case ';': {
                TheSemicolonToken = inputString.substr(i, 1);
                Token SemicolonToken = {TK_Semicolon, .as_string = strdup(TheSemicolonToken.c_str()), line, start_column, column + (int)(i - start) - 1};
                command.push_back(SemicolonToken);
                display_tokens(command);
                save_command(command);
                command.clear();
                commandFound = false;
                i++;
                column++;
                break;
            }

            case '\n': {
                display_tokens(command);
                save_command(command);
                command.clear();
                commandFound = false;
                i++;
                line++;
                column = 1;
                break;
            }

            case '"': {
                i++;
                column++;
                parse_string(&i, inputString);
                TheString = inputString.substr(start + 1, i - start - 1);
                Token strToken = {TK_String, .as_string = strdup(TheString.c_str()), line, start_column, column + (int)(i - start) - 2};
                command.push_back(strToken);
                column += (int)(i - start);
                break;
            }

            case '-': {
                parse_flag(&i, inputString);
                TheFlag = inputString.substr(start, i - start);
                Token flagToken = {TK_Flag, .as_string = strdup(TheFlag.c_str()), line, start_column, column + (int)(i - start) - 1};
                command.push_back(flagToken);
                column += (int)(i - start);
                break;
            }

            default:
                parse_argument(&i, inputString);
                TheArgument = inputString.substr(start, i - start);
                if (!TheArgument.empty()) {
                    Token argToken = {TK_Argument, .as_string = strdup(TheArgument.c_str()), line, start_column, column + (int)(i - start) - 1};
                    command.push_back(argToken);
                }
                column += (int)(i - start);
                break;
        }
    }

    if (!command.empty()) {
        display_tokens(command);
        save_command(command);
    }

    for (auto &tok : command) {
        if (tok.type == TK_String || tok.type == TK_Identifier || tok.type == TK_Argument || tok.type == TK_Flag) {
            free(tok.as_string);
        }
    }

    Utils::general_log("[lex] Lexing process completed at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);
}
