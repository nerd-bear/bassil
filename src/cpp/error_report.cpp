#include "C:/coding-projects/CPP-Dev/bassil/src/headers/error_repport.h"
#include <iostream>
#include <fstream>
#include <limits>

void printColoredError(const std::string& filePath, int lineNumber, int startColumn, int endColumn, const std::string& line, const std::string& msg) {
    std::cout << "\n ---> " << Utils::boldText("File: ") << Utils::italicText(filePath) << ":" << lineNumber << ":" << startColumn
              << "\n|    " << Utils::boldText(Utils::colorText("Error on line: ", "#fc0313")) << Utils::boldText(std::to_string(lineNumber))
              << " " << Utils::colorText(Utils::boldText("Start column: "), "#ff9752") << Utils::boldText(std::to_string(startColumn))
              << " " << Utils::colorText(Utils::boldText("End column: "), "#ff9752") << Utils::boldText(std::to_string(endColumn))
              << "\n|    " << Utils::colorText(line, "#a8ff94")
              << "\n|    " << std::string(startColumn - 1, ' ') << std::string(endColumn - startColumn, '^')
              << "\n|    \n|    " << Utils::colorText(msg, "#94b0ff") << "\n" << std::endl;
}

void printPlainError(const std::string& filePath, int lineNumber, int startColumn, int endColumn, const std::string& line, const std::string& msg) {
    std::cout << "Error in file: " << filePath << ":" << lineNumber << ":" << startColumn
              << "\nError on line: " << lineNumber << " Start column: " << startColumn << " End column: " << endColumn
              << "\n    " << line
              << "\n    " << std::string(startColumn - 1, ' ') << std::string(endColumn - startColumn, '^')
              << "\n\n" << msg << std::endl;
}

int reportError(const std::string& filePath, int lineNumber, int startColumn, int endColumn, const std::string& msg) {
    if (startColumn > endColumn) {
        std::cerr << "[reportError] Start Column is bigger than end column" << std::endl;
        exit(1);
        return 1;
    }

    std::fstream file(filePath);
    std::string line;

    try {
        line = Utils::readLineFromFile(file, lineNumber);
    } catch (const std::exception& e) {
        std::cerr << "Issue while reporting error: " << e.what() << "\n    Error reading from file path: " << filePath << "\n";
        exit(1);
        return 1;
    }

    if (!Utils::isAnsiEnabledInConsole()) {
        if (Utils::enableAnsiInConsole() != 0) {
            std::cout << "ANSI is not enabled in console. For better and more readable error messages, please enable ANSI!\n";
            Utils::general_log("ANSI is not enabled in console. For better and more readable error messages, please enable ANSI!");
            printPlainError(filePath, lineNumber, startColumn, endColumn, line, msg);
        } else {
            printColoredError(filePath, lineNumber, startColumn, endColumn, line, msg);
        }
    } else {
        printColoredError(filePath, lineNumber, startColumn, endColumn, line, msg);
    }

    return 0;
}