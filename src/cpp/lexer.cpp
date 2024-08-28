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

void parse_integer(size_t *pos, const std::string &str) {
    size_t start = *pos;
    while (*pos < str.size() && isdigit(str[*pos])) {
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

    // Try to open the output file and log the attempt
    std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/after_lex.txt", std::ios::app);
    if (!outputFile.is_open()) {
        Utils::general_log("[save_command] Failed to open file.", logBool);
        return;
    }
    Utils::general_log("[save_command] Successfully opened file.", logBool);

    // Start writing tokens to the file
    outputFile << "[\n"; // Start of the list of tokens

    for (const auto& token : command) {
        Utils::general_log("[save_command] Writing token to file:", logBool);

        outputFile << "  {\n"; // Start of an individual token
        outputFile << "    \"line\": " << token.line << ",\n";
        outputFile << "    \"start_column\": " << token.start_column << ",\n";
        outputFile << "    \"end_column\": " << token.end_column << ",\n";

        // Add more detailed logging for each case
        switch (token.type) {
            case TK_Identifier:
                Utils::general_log("[save_command] Token Type: Identifier, Value: " + std::string(token.as_identifier), logBool);
                outputFile << "    \"type\": \"Identifier\",\n";
                outputFile << "    \"value\": \"" << token.as_identifier << "\"\n";
                break;
            case TK_Argument:
                Utils::general_log("[save_command] Token Type: Argument, Value: " + std::string(token.as_identifier), logBool);
                outputFile << "    \"type\": \"Argument\",\n";
                outputFile << "    \"value\": \"" << token.as_identifier << "\"\n";
                break;
            case TK_String:
                Utils::general_log("[save_command] Token Type: String, Value: " + std::string(token.as_string), logBool);
                outputFile << "    \"type\": \"String\",\n";
                outputFile << "    \"value\": \"" << token.as_string << "\"\n";
                break;
            case TK_Flag:
                Utils::general_log("[save_command] Token Type: Flag, Value: " + std::string(token.as_string), logBool);
                outputFile << "    \"type\": \"Flag\",\n";
                outputFile << "    \"value\": \"" << token.as_string << "\"\n";
                break;
            case TK_Semicolon:
                Utils::general_log("[save_command] Token Type: Semicolon, Value: " + std::string(token.as_string), logBool);
                outputFile << "    \"type\": \"Semicolon\",\n";
                outputFile << "    \"value\": \"" << token.as_string << "\"\n";
                break;
            case TK_Integer:
                Utils::general_log("[save_command] Token Type: Integer, Value: " + std::string(token.as_string), logBool);
                outputFile << "    \"type\": \"Integer\",\n";
                outputFile << "    \"value\": \"" << token.as_string << "\"\n";
                break;
            default:
                Utils::general_log("[save_command] Unknown Token Type", logBool);
                continue; // Skip adding an unknown token to the file
        }

        outputFile << "  },\n"; // End of an individual token
        Utils::general_log("[save_command] Finished writing token to file.", logBool);
    }

    outputFile << "]\n"; // End of the list of tokens

    outputFile.close();
    Utils::general_log("[save_command] Successfully closed the file.", logBool);
}

std::vector<Token> lex(const std::string &inputString) {
    bool commandFound = false;
    size_t i = 0;
    int line = 1;
    int column = 1;
    std::vector<Token> command;

    Utils::general_log("[lex] Starting lexing process for input: \"" + inputString + "\" at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);

    while (i < inputString.length()) {
        Utils::general_log("[lex] i is " + i, logBool);
        if (i > inputString.size()) {
            break;
        }

        size_t start = i;
        int start_column = column;

        // Skip initial whitespace or newlines
        if (inputString[i] == ' ' || inputString[i] == '\n') {
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
            Utils::general_log("[lex] Detecting command or integer at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);
            // Detect command name (first token) or an integer if it is at the beginning of a line
            if (isdigit(inputString[i])) {
                parse_integer(&i, inputString);
                std::string TheInteger = inputString.substr(start, i - start);
                Token token = {TK_Integer, .as_string = strdup(TheInteger.c_str()), line, start_column, column + (int)(i - start) - 1};
                command.push_back(token);
            } else {
                parse_command(&i, inputString);
                std::string commandName = inputString.substr(start, i - start);
                if (!commandName.empty()) {
                    Token cmdToken = {TK_Identifier, .as_identifier = strdup(commandName.c_str()), line, start_column, column + (int)(i - start) - 1};
                    command.push_back(cmdToken);
                    commandFound = true;
                }
            }
            column += (int)(i - start);
            Utils::general_log("[lex] Finished detecting command or integer at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);
            line++;
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
                Utils::general_log("[lex] Detected semicolon at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);
                std::string TheSemicolon = inputString.substr(i, 1);
                Token token = {TK_Semicolon, .as_string = strdup(TheSemicolon.c_str()), line, start_column, column};
                command.push_back(token);
                i++;
                column++;

                // Log, save, and clear the command after a semicolon
                display_tokens(command);
                save_command(command);
                command.clear();
                commandFound = false;
                break;
            }

            case '\n': {
                Utils::general_log("[lex] Detected newline at line: " + std::to_string(line), logBool);
                // Log, save, and clear the command when encountering a newline
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
                std::string TheString = inputString.substr(start + 1, i - start - 1);
                Token token = {TK_String, .as_string = strdup(TheString.c_str()), line, start_column, column + (int)(i - start) - 2};
                command.push_back(token);
                column += (int)(i - start);
                break;
            }

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
                parse_integer(&i, inputString);
                std::string TheInteger = inputString.substr(start, i - start);
                Token token = {TK_Integer, .as_string = strdup(TheInteger.c_str()), line, start_column, column + (int)(i - start) - 1};
                command.push_back(token);
                column += (int)(i - start);
                break;
            }

            default: {
                parse_argument(&i, inputString);
                std::string TheArgument = inputString.substr(start, i - start);
                if (!TheArgument.empty()) {
                    Token token = {TK_Argument, .as_string = strdup(TheArgument.c_str()), line, start_column, column + (int)(i - start) - 1};
                    command.push_back(token);
                }
                column += (int)(i - start);
                break;
            }
        }

        // After processing each token, ensure we advance correctly
        if (commandFound && (i >= inputString.size() || inputString[i] == ';' || inputString[i] == '\n')) {
            Utils::general_log("[lex] Processing end of command or input at line: " + std::to_string(line), logBool);
            display_tokens(command);
            save_command(command);
            command.clear();
            commandFound = false;

            // Correctly advance the index after processing
            if (i < inputString.size() && (inputString[i] == ';' || inputString[i] == '\n')) {
                i++;
                if (inputString[i - 1] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
            }
        }
    }

    // Clean up any remaining tokens if the input didn't end with a semicolon or newline
    if (!command.empty()) {
        Utils::general_log("[lex] Cleaning up remaining tokens at line: " + std::to_string(line), logBool);
        display_tokens(command);
        save_command(command);
    }

    // Free any dynamically allocated memory for strings in tokens
    for (auto &tok : command) {
        if (tok.type == TK_String || tok.type == TK_Identifier ||
            tok.type == TK_Argument || tok.type == TK_Flag || tok.type == TK_Semicolon || tok.type == TK_Integer) {
            Utils::general_log("[lex] Freeing token memory for type at line: " + std::to_string(line), logBool);
            free(tok.as_string);
        }
    }

    Utils::general_log("[lex] Lexing process completed at line: " + std::to_string(line) + ", column: " + std::to_string(column), logBool);

    return command;  // Return the list of tokens
}
