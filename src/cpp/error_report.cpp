#include "C:/coding-projects/CPP-Dev/bassil/src/headers/error_repport.h"
#include <iostream>
#include <fstream>
#include <limits>

int reportError(std::string filePath, int lineNumber, int start_column, int end_column, std::string msg)
{
    if (start_column > end_column) // Early return if invalid column integers were provided
    {
        std::cerr << "[reportError] Start Column is bigger than end column" << std::endl;
        exit(1);
        return 1;
    }

    std::fstream file(filePath);
    std::string line;

    try
    {
        line = Utils::readLineFromFile(file, lineNumber);
    } catch (std::exception exception) {
        std::cerr << "Issue while reporting error:\n    Error reading from file path:" << filePath << "\n";
        exit(1);
        return 1;
    }

    if (!Utils::isAnsiEnabledInConsole())
    {
        bool tryEnableANSI = Utils::enableAnsiInConsole(); // return 0 on success and 1 on fail

        if (tryEnableANSI == 1)
        {
            std::cout << "ANSI in not enabled in console, for better and more readable error messages, please enable ANSI!\n";
            Utils::general_log("ANSI in not enabled in console, for better and more readable error messages, please enable ANSI!");

            // print error msg without ANSI
            std::cout << "Error on line: " << lineNumber << " " << "Start column: " << start_column << " " << "End column: " << end_column << "\n    " << line << "\n    " << std::string(start_column - 1, ' ') << std::string(end_column - start_column, '^') << "\n\n"
                      << msg << std::endl;

            return 0;
        }
    }

    // print error msg normaly with ANSI
    std::cout << Utils::boldText("File path: ") << Utils::italicText(filePath) << "\n"
              << Utils::boldText(Utils::colorText("Error on line: ", "#fc0313")) << Utils::boldText(std::to_string(lineNumber)) << " " << Utils::colorText(Utils::boldText("Start column: "), "#ff9752") << Utils::boldText(std::to_string(start_column)) << " " << Utils::colorText(Utils::boldText("End column: "), "#ff9752") << Utils::boldText(std::to_string(end_column)) << "\n    " << Utils::colorText(line, "#a8ff94") << "\n    " << std::string(start_column - 1, ' ') << std::string(end_column - start_column, '^') << "\n\n"
              << Utils::colorText(msg, "#94b0ff") << std::endl;
    return 0;
}