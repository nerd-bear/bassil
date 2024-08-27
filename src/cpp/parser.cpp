#include <iostream>
#include <vector>
#include <string>

// Assuming the Token structure is already defined as in your lexer.
enum TokenType { TK_Identifier, TK_Argument, TK_String, TK_Flag };

struct Token {
    TokenType type;
    std::string value;  // Change from char* to std::string for easier management
};

struct Command {
    std::string name;
    std::vector<std::string> flags;
    std::vector<std::string> arguments;
};

Command parse(const std::vector<Token>& tokens) {
    Command cmd;
    size_t i = 0;

    // The first token should be the command name (identifier)
    if (tokens[i].type == TK_Identifier) {
        cmd.name = tokens[i].value;
        i++;
    } else {
        std::cerr << "[parse] Error: Expected command identifier at start." << std::endl;
        exit(1);
    }

    // Process remaining tokens
    while (i < tokens.size()) {
        if (tokens[i].type == TK_Flag) {
            cmd.flags.push_back(tokens[i].value);
        } else if (tokens[i].type == TK_Argument || tokens[i].type == TK_String) {
            cmd.arguments.push_back(tokens[i].value);
        } else {
            std::cerr << "[parse] Error: Unexpected token type." << std::endl;
            exit(1);
        }
        i++;
    }

    return cmd;
}

int main() {
    // Example input
    std::string input = "command -flag1 -flag2 \"argument with spaces\" arg2";

    // Lex the input string
    std::vector<Token> tokens;
    lex(input, tokens);  // Assuming lex fills the tokens vector

    // Parse the tokens into a command structure
    Command cmd = parse(tokens);

    // Output the parsed command
    std::cout << "Command: " << cmd.name << std::endl;
    std::cout << "Flags: ";
    for (const auto& flag : cmd.flags) {
        std::cout << flag << " ";
    }
    std::cout << std::endl;

    std::cout << "Arguments: ";
    for (const auto& arg : cmd.arguments) {
        std::cout << arg << " ";
    }
    std::cout << std::endl;

    return 0;
}
