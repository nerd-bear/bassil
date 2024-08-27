#include "C:\coding-projects\CPP-Dev\Orion-Shell\src\headers\lexer.h"

bool logBool = true;  // Whether to log or not

void parse_string(size_t *pos, const std::string &str) {
    Utils::general_log("[parse_string] Starting parse at position: " + std::to_string(*pos), logBool);
    for (; (*pos < str.size() && str[*pos] != '"'); (*pos)++) {
        if (str[*pos] == '\\') {
            Utils::general_log("[parse_string] Escape character found at position: " + std::to_string(*pos), logBool);
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
    Utils::general_log("[parse_flag] Starting parse at position: " + std::to_string(*pos), logBool);
    size_t start = *pos;
    while (*pos < str.size() && str[*pos] != ' ') {
        (*pos)++;
    }
    std::string flag = str.substr(start, *pos - start);
    Utils::general_log("[parse_flag] Parsed flag: " + flag, logBool);
}

void parse_command(size_t *pos, const std::string &str) {
    Utils::general_log("[parse_command] Starting parse at position: " + std::to_string(*pos), logBool);
    while (*pos < str.size() && str[*pos] != ' ' && str[*pos] != '\n') {
        (*pos)++;
    }
    Utils::general_log("[parse_command] Finished parsing command. Ending position: " + std::to_string(*pos), logBool);
}

void parse_argument(size_t *pos, const std::string &str) {
    Utils::general_log("[parse_argument] Starting parse at position: " + std::to_string(*pos), logBool);
    while (*pos < str.size() && str[*pos] != ' ' && str[*pos] != '\n') {
        (*pos)++;
    }
    Utils::general_log("[parse_argument] Finished parsing argument. Ending position: " + std::to_string(*pos), logBool);
}

void display_tokens(const std::vector<Token>& command) {
    Utils::general_log("[display_tokens] Displaying tokens:", logBool);
    for (const auto& token : command) {
        switch (token.type) {
            case TK_Identifier:
                Utils::general_log("Identifier: " + std::string(token.as_identifier), logBool);
                break;
            case TK_Argument:
                Utils::general_log("Argument: " + std::string(token.as_identifier), logBool);
                break;
            case TK_String:
                Utils::general_log("String: " + std::string(token.as_string), logBool);
                break;
            case TK_Flag:
                Utils::general_log("Flag: " + std::string(token.as_string), logBool);
                break;
            default:
                Utils::general_log("Unknown Token Type", logBool);
                break;
        }
    }
}

void save_command(const std::vector<Token>& command) {
    Utils::general_log("[save_command] Saving tokens:", logBool);
    std::ofstream outputFile("C:/coding-projects/CPP-Dev/Orion-Shell/assets/after_lex.txt", std::ios::app);

    if (!outputFile.is_open()) {
        Utils::general_log("Failed to open file.", logBool);
        return;
    }

    outputFile << "[\n"; // Start of the list of tokens

    for (const auto& token : command) {
        outputFile << "  {\n"; // Start of an individual token

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

    std::vector<Token> command;

    Utils::general_log("[lex] Starting lexing process for input: \"" + inputString + "\"", logBool);

    while (i < inputString.size()) {
        size_t start = i;
        std::string TheString, TheFlag, TheArgument;

        // Skip initial whitespace or newlines
        if (!commandFound && (inputString[i] == ' ')) {
            Utils::general_log("[lex] Skipping whitespace at position: " + std::to_string(i), logBool);
            i++;
            continue;
        }

        if (!commandFound) {
            // Detect command name (first token)
            Utils::general_log("[lex] Parsing command starting at position: " + std::to_string(i), logBool);
            parse_command(&i, inputString);
            commandName = inputString.substr(start, i - start);
            Utils::general_log("[lex] Command found: " + commandName, logBool);
            Token cmdToken = {TK_Identifier, .as_identifier = strdup(commandName.c_str())};
            command.push_back(cmdToken);
            commandFound = true;
            continue;
        }

        // Skip any whitespace after command detection
        if (inputString[i] == ' ' || inputString[i] == '\n') {
            Utils::general_log("[lex] Skipping whitespace at position: " + std::to_string(i), logBool);
            i++;
            continue;
        }

        if (inputString[i] == ';') {

        }

        if (inputString[i] == char(0x0A)) {
            Utils::general_log("found new line!");
        }

        switch (inputString[i]) {
            case '\n':
                Utils::general_log("[lex] Newline encountered at position: " + std::to_string(i), logBool);
                break;

            case '"': {
                Utils::general_log("[lex] Quoted string detected starting at position: " + std::to_string(i), logBool);
                i++;
                parse_string(&i, inputString);
                TheString = inputString.substr(start + 1, i - start - 1);
                Utils::general_log("[lex] Parsed string: " + TheString, logBool);
                Token strToken = {TK_String, .as_string = strdup(TheString.c_str())};
                command.push_back(strToken);
                break;
            }

            case '-': {
                Utils::general_log("[lex] Flag detected at position: " + std::to_string(i), logBool);
                parse_flag(&i, inputString);
                TheFlag = inputString.substr(start, i - start);
                Utils::general_log("[lex] Parsed flag: " + TheFlag, logBool);
                Token flagToken = {TK_Flag, .as_string = strdup(TheFlag.c_str())};
                command.push_back(flagToken);
                break;
            } 

            default:
                Utils::general_log("[lex] Default case at position: " + std::to_string(i) + " with character: " + inputString[i], logBool);

                parse_argument(&i, inputString);
                TheArgument = inputString.substr(start, i - start);
                if (!TheArgument.empty()) {
                    Utils::general_log("[lex] Parsed argument: " + TheArgument, logBool);
                    Token argToken = {TK_Argument, .as_string = strdup(TheArgument.c_str())};
                    command.push_back(argToken);
                } else {
                    Utils::general_log("[lex] Ignored empty argument.", logBool);
                }
                break;
        }

        i++;  // Move to the next character after processing
        Utils::general_log("[lex] Moving to next character at position: " + std::to_string(i), logBool);
    }

    // Display all tokens
    display_tokens(command);
    save_command(command);

    // lineCount++

    // Cleanup (free allocated memory)
    Utils::general_log("[lex] Cleaning up allocated memory for tokens.", logBool);
    for (auto &tok : command) {
        if (tok.type == TK_String || tok.type == TK_Identifier || tok.type == TK_Argument || tok.type == TK_Flag) {
            free(tok.as_string);  // Since both as_string and as_identifier are char* pointing to dynamically allocated memory
        }
    }
    Utils::general_log("[lex] Lexing process completed.", logBool);
}