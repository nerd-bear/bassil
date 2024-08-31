#include "C:/coding-projects/CPP-Dev/bassil/src/headers/error_repport.h"
#include <iostream>
#include <fstream>
#include <limits>

int reportError(std::string filePath, int lineNum, int column, std::string msg)
{   
    std::fstream file(filePath);

    std::string line = Utils::readLineFromFile(file, lineNum);

    std::cout << "Error on line: " << lineNum << "\n    " << line << "\n    " << std::string(column - 1, '-') << "^\n\n" << msg << std::endl;
    return 0;
}