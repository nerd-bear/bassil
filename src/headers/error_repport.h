#ifndef ERROR_REPORT_H
#define ERROR_REPORT_H

#include <string>
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"

int reportError(std::string filePath, int lineNum, int column, std::string msg);

#endif